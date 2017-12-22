/*
 * The MIT License
 *
 * Copyright 2017 Schuemi.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/* 
 * File:   Highscore.cpp
 * Author: Schuemi
 * 
 * Created on 12. Dezember 2017, 21:05
 */

#include "constants.h"
#include "Highscore.h"
#include <EEPROM.h>
#include "Utils.h"

Highscore::Highscore() {
    g_arduboy = (Arduboy2*)getInstance();
    
    uint32_t eepromMagic;
    EEPROM.get(EEPROM_MAGIC_BYTE + EEPROM_STORAGE_SPACE_START, eepromMagic);
    if (eepromMagic == EEPROM_MAGIC) {
        EEPROM.get(EEPROM_HIGHSCORE_BYTE + EEPROM_STORAGE_SPACE_START, m_highscoreList);
    } else {
        for (int i = 0; i < HIGHSCORE_LIST_SIZE; i++) {
            m_highscoreList[i].name[0] = 0;
            m_highscoreList[i].score = 0;

        }
        saveListEEPROM();
    }
    
    reset();
    
    
}

void Highscore::saveListEEPROM() {
   uint32_t eepromMagic = EEPROM_MAGIC;
   EEPROM.put(EEPROM_MAGIC_BYTE + EEPROM_STORAGE_SPACE_START, eepromMagic);
   EEPROM.put(EEPROM_HIGHSCORE_BYTE + EEPROM_STORAGE_SPACE_START, m_highscoreList);
}

Highscore::~Highscore() {
}

void Highscore::reset() {
   
    m_state = STATE_NORMAL;
    m_cusorBlink = 0;
    m_newNamePlace = HIGHSCORE_LIST_SIZE;
    m_currEditLocation = 0;
    m_currName[0] = '_';
    m_currName[1] = '_';
    m_currName[2] = '_';
    m_closeHighScore = false;
    
}


void Highscore::addNewRecord(uint32_t score) {
    m_newNamePlace = HIGHSCORE_LIST_SIZE;
    for (int i = 0; i < HIGHSCORE_LIST_SIZE; i++) {
        if (m_highscoreList[i].score < score){
            m_newNamePlace = i;
            break;
        }
    }
    if (m_newNamePlace < HIGHSCORE_LIST_SIZE){
        for (int i = HIGHSCORE_LIST_SIZE-2; i >= m_newNamePlace; i--) {
            m_highscoreList[i+1].score =  m_highscoreList[i].score;
            memcpy(m_highscoreList[i+1].name, m_highscoreList[i].name, 3);
        }
        m_state = STATE_INSERT_NAME;
        m_highscoreList[m_newNamePlace].score = score;
    }
    
}


void Highscore::draw() {
    g_arduboy->clear();
    
    g_arduboy->setCursor(40, 0);
    g_arduboy->print(F("HIGHSCORE"));
    
    
    
    
    if (m_state == STATE_INSERT_NAME) {
        g_arduboy->pollButtons();
        char nameStr[10];
        memcpy(nameStr, F(". ---    "), 10);
        memcpy(&nameStr[2], m_currName, 3);
       
        
        
        g_arduboy->setCursor(2, 16);
        m_cusorBlink++; if (m_cusorBlink == 20) { m_cusorBlink = 0; }
        if (m_newNamePlace < 9) g_arduboy->setCursor(7, 16);
        g_arduboy->print(m_newNamePlace+1);
        
        if (m_cusorBlink < 10){
            g_arduboy->setCursor(26 + 6*m_currEditLocation, 18);
            g_arduboy->print('_');
            
        } 
        
        g_arduboy->setCursor(14, 16);
        g_arduboy->print(nameStr);    
        g_arduboy->print(m_highscoreList[m_newNamePlace].score);        
        
        
       
        
        if (g_arduboy->justPressed(UP_BUTTON)){
            m_currName[m_currEditLocation]++;
            if (m_currName[m_currEditLocation] < 0x41 || m_currName[m_currEditLocation] > 0x5A) m_currName[m_currEditLocation] = 0x41;
            
        }
        if (g_arduboy->justPressed(DOWN_BUTTON)){
            m_currName[m_currEditLocation]--;
            if (m_currName[m_currEditLocation] < 0x41 || m_currName[m_currEditLocation] > 0x5A) m_currName[m_currEditLocation] = 0x5A;
            
        }
        if (g_arduboy->justPressed(B_BUTTON) || g_arduboy->justPressed(RIGHT_BUTTON)){
            m_currEditLocation++;
            if (m_currEditLocation == 3) m_currEditLocation = 0;
            
        }
        if (g_arduboy->justPressed(LEFT_BUTTON)){
            if (m_currEditLocation == 0) m_currEditLocation = 2; else m_currEditLocation--;
        }
        if (g_arduboy->justPressed(A_BUTTON)){
            if (m_currName[0] != '_' || m_currName[1] != '_' || m_currName[2] != '_') {
                memcpy(m_highscoreList[m_newNamePlace].name, m_currName, 3);
                for(int i = 0; i < 3; i++) if (m_highscoreList[m_newNamePlace].name[i] == '_') m_highscoreList[m_newNamePlace].name[i] = 0x20;
                saveListEEPROM();
                m_state = STATE_NORMAL;
                
                
            }
        }
        
        
    }
    
    if (m_state == STATE_NORMAL) {
        g_arduboy->pollButtons();
        
        for (int i = 0; i < HIGHSCORE_LIST_SIZE; i++) {
                int16_t cusory = (i*9 + 12);
                if (cusory > 128) break;
                if (cusory < -6 ) continue;
                
                g_arduboy->setCursor(2, cusory);
                if (i < 9) g_arduboy->setCursor(7, cusory);
                g_arduboy->print(i+1);
                g_arduboy->setCursor(14, cusory);
                if (m_highscoreList[i].score == 0){
                    g_arduboy->print(F(". ---    ----------"));
                } else {
                    char name[4]; 
                    memcpy(name, m_highscoreList[i].name, 3);
                    name[3] = 0;
                    g_arduboy->print(". ");
                    g_arduboy->print(name);
                    g_arduboy->setCursor(68 + ((10 - Utils::countdigits(m_highscoreList[i].score)) * 6), cusory);
                    
                    g_arduboy->print(m_highscoreList[i].score);
                }
                        
        }
        
        if (g_arduboy->justPressed(A_BUTTON) || g_arduboy->justPressed(B_BUTTON)){
            m_closeHighScore = true;
        }
        
       
    }
    
    g_arduboy->display();
}
