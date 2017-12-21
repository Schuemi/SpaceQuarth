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
 * File:   StarField.cpp
 * Author: Schuemi
 * 
 * Created on 12. Dezember 2017, 11:28
 */

#include "StarField.h"


StarField::StarField(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t direction) {
    g_arduboy = (Arduboy2*)getInstance();
    m_x = x;
    m_width = width;
    m_y = y;
    m_height = height;
    m_direction = direction;
    for (int i = 0; i < MAX_STARS; i++) {
        m_starsX[i] = m_x + random(m_width);
        m_starsY[i] = m_y + random(m_height);
        m_starsSpeed[i] = random(3) + 1;
    }
    
}

void StarField::move() {
   for (int i = 0; i < MAX_STARS; i++) {
       if (m_direction == 1) {
            m_starsY[i] += m_starsSpeed[i];
            if (m_starsY[i] > m_y + m_height){
                m_starsX[i] = m_x + random(m_width);
                m_starsY[i] = m_y;
                m_starsSpeed[i] = random(3) + 1;
            }
       }
      /* if (m_direction == 2) {
            if (m_starsY[i] < 2){
                m_starsX[i] = m_x + random(m_width);
                m_starsY[i] = m_y + m_height;
                m_starsSpeed[i] = random(3) + 1;
            } else m_starsY[i] -= m_starsSpeed[i];
       }*/
       /*if (m_direction == 3) {
            m_starsX[i] += m_starsSpeed[i];
            if (m_starsX[i] > m_x + m_width){
                m_starsY[i] = m_y+ random(m_height);
                m_starsX[i] = m_x;
                m_starsSpeed[i] = random(3) + 1;
            }
       }*/
       if (m_direction == 4) {
            if (m_starsX[i] < 2){
                m_starsY[i] = m_y + random(m_height);
                m_starsX[i] = m_x + m_width;
                m_starsSpeed[i] = random(3) + 1;
            } else m_starsX[i] -= m_starsSpeed[i];
       }
       
   } 
}
void StarField::draw(bool warp) {
    for (int i = 0; i < MAX_STARS; i++) {
         g_arduboy->drawPixel(m_starsX[i], m_starsY[i],1);
         if (warp || m_starsSpeed[i] > 2){
            if (m_direction == 1) g_arduboy->drawPixel(m_starsX[i], m_starsY[i] + 1,1); //  || m_direction == 2
            if (m_direction == 4) g_arduboy->drawPixel(m_starsX[i] + 1, m_starsY[i],1); // m_direction == 3 || 
            
         }
    }
}
StarField::~StarField() {
}

