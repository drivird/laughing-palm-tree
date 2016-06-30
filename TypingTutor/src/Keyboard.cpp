/*
Copyright(c) 2016, Dominique Rivard
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "Keyboard.h"

#include "cinder/gl/gl.h"
#include "cinder/app/KeyEvent.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace ci::app;

Keyboard::Keyboard()
{
   mKeyMap[KeyEvent::KEY_1        ] = Key("1", 0, 0, false, true);
   mKeyMap[KeyEvent::KEY_2        ] = Key("2", 0, 1, false, true);
   mKeyMap[KeyEvent::KEY_3        ] = Key("3", 0, 2, false, true);
   mKeyMap[KeyEvent::KEY_4        ] = Key("4", 0, 3, false, true);
   mKeyMap[KeyEvent::KEY_5        ] = Key("5", 0, 4, false, true);
   mKeyMap[KeyEvent::KEY_6        ] = Key("6", 0, 5, false, true);
   mKeyMap[KeyEvent::KEY_7        ] = Key("7", 0, 6, false, true);
   mKeyMap[KeyEvent::KEY_8        ] = Key("8", 0, 7, false, true);
   mKeyMap[KeyEvent::KEY_9        ] = Key("9", 0, 8, false, true);
   mKeyMap[KeyEvent::KEY_0        ] = Key("0", 0, 9, false, true);
   mKeyMap[KeyEvent::KEY_q        ] = Key("Q", 1, 0, false, true);
   mKeyMap[KeyEvent::KEY_w        ] = Key("W", 1, 1, false, true);
   mKeyMap[KeyEvent::KEY_e        ] = Key("E", 1, 2, false, true);
   mKeyMap[KeyEvent::KEY_r        ] = Key("R", 1, 3, false, true);
   mKeyMap[KeyEvent::KEY_t        ] = Key("T", 1, 4, false, true);
   mKeyMap[KeyEvent::KEY_y        ] = Key("Y", 1, 5, false, true);
   mKeyMap[KeyEvent::KEY_u        ] = Key("U", 1, 6, false, true);
   mKeyMap[KeyEvent::KEY_i        ] = Key("I", 1, 7, false, true);
   mKeyMap[KeyEvent::KEY_o        ] = Key("O", 1, 8, false, true);
   mKeyMap[KeyEvent::KEY_p        ] = Key("P", 1, 9, false, true);
   mKeyMap[KeyEvent::KEY_a        ] = Key("A", 2, 0, false, true);
   mKeyMap[KeyEvent::KEY_s        ] = Key("S", 2, 1, false, true);
   mKeyMap[KeyEvent::KEY_d        ] = Key("D", 2, 2, false, true);
	mKeyMap[KeyEvent::KEY_f        ] = Key("F", 2, 3, false, true);
	mKeyMap[KeyEvent::KEY_g        ] = Key("G", 2, 4, false, true);
	mKeyMap[KeyEvent::KEY_h        ] = Key("H", 2, 5, false, true);
	mKeyMap[KeyEvent::KEY_j        ] = Key("J", 2, 6, false, true);
	mKeyMap[KeyEvent::KEY_k        ] = Key("K", 2, 7, false, true);
   mKeyMap[KeyEvent::KEY_l        ] = Key("L", 2, 8, false, true);
   mKeyMap[KeyEvent::KEY_SEMICOLON] = Key(";", 2, 9, false, true);
	mKeyMap[KeyEvent::KEY_z        ] = Key("Z", 3, 0, false, true);
	mKeyMap[KeyEvent::KEY_x        ] = Key("X", 3, 1, false, true);
   mKeyMap[KeyEvent::KEY_c        ] = Key("C", 3, 2, false, true);
	mKeyMap[KeyEvent::KEY_v        ] = Key("V", 3, 3, false, true);
   mKeyMap[KeyEvent::KEY_b        ] = Key("B", 3, 4, false, true);
	mKeyMap[KeyEvent::KEY_n        ] = Key("N", 3, 5, false, true);
	mKeyMap[KeyEvent::KEY_m        ] = Key("M", 3, 6, false, true);
   mKeyMap[KeyEvent::KEY_COMMA    ] = Key(",", 3, 7, false, true);
   mKeyMap[KeyEvent::KEY_PERIOD   ] = Key(".", 3, 8, false, true);
   mKeyMap[KeyEvent::KEY_SLASH    ] = Key("/", 3, 9, false, true);

   generateTextures();
}

void Keyboard::loadImageFile(const fs::path& imagePath)
{
   mTexture = gl::Texture::create(loadImage(imagePath));
}

void Keyboard::draw(const TypingTutorInterface& rApp)
{
   if (mTexture){
      Rectf rect(mTexture->getBounds());
      const auto center = vec2(0.5f * rApp.getWindowWidth(), rApp.getWindowHeight() - 0.5f * rect.getHeight());
      rect.offsetCenterTo(center);
      mTopLeft = rect.getUpperLeft();
      gl::draw(mTexture, rect);
   }

   //debug: drawAllKeys();
}

int Keyboard::countAvailableKeys() const
{
   const auto cnt = std::count_if(mKeyMap.cbegin(), mKeyMap.cend(), 
      [](const std::pair<int, Key>& i) {
         return (i.second.enabled && i.second.available); 
   });

   return static_cast<int>(cnt);
}

int Keyboard::randSelectKey() const
{
   const auto NbAvailableKeys = countAvailableKeys();
   if (NbAvailableKeys > 0) {
      auto randKeyNumber = Rand::randInt(NbAvailableKeys);
      for (auto& i : mKeyMap) {
         if (i.second.enabled && i.second.available) {
            if (--randKeyNumber < 0) {
               return i.first;
            }
         }
      }
   }
   return KeyEvent::KEY_UNKNOWN;
}

ci::Color Keyboard::getColumnColor(int col)
{
   switch (col) {
   case 0:  return Color(238.0f/255.0f,  52.0f/255.0f,  66.0f/255.0f);
   case 1:  return Color(249.0f/255.0f, 166.0f/255.0f,  28.0f/255.0f);
   case 2:  return Color(112.0f/255.0f, 194.0f/255.0f, 122.0f/255.0f);
   case 3:  return Color(  2.0f/255.0f, 174.0f/255.0f, 236.0f/255.0f);
   case 4:  return Color(  2.0f/255.0f, 174.0f/255.0f, 236.0f/255.0f);
   case 5:  return Color( 89.0f/255.0f, 134.0f/255.0f, 193.0f/255.0f);
   case 6:  return Color( 89.0f/255.0f, 134.0f/255.0f, 193.0f/255.0f);
   case 7:  return Color(151.0f/255.0f, 214.0f/255.0f, 245.0f/255.0f);
   case 8:  return Color(255.0f/255.0f, 233.0f/255.0f, 160.0f/255.0f);
   case 9:  return Color(191.0f/255.0f, 225.0f/255.0f, 192.0f/255.0f);
   default: return Color(            0,             0,              0);
   }
}

void Keyboard::generateTextures()
{
   for (auto& i : mKeyMap) {
      auto& key = i.second;
      TextLayout layout;
      layout.clear(Color(0, 0, 0));
      layout.setColor(getColumnColor(key.col));
      layout.setFont(Font("Consolas", 72));
      layout.addLine(key.c);
      key.texture = gl::Texture2d::create(layout.render(true, false));
   }
}

const ci::Rectf Keyboard::getKeyRectf(int keyCode) const
{
   const auto it = mKeyMap.find(keyCode);
   assert(it != mKeyMap.end());

   const auto key = it->second;
   Rectf rect(key.texture->getBounds());
   const auto x = key.row * 15 + key.col * 70 + 45;
   const auto y = key.row * 75 - 390;
   rect.offsetCenterTo(mTopLeft + vec2(x, y));
   return rect;
}

const ci::gl::TextureRef Keyboard::getKeyTexture(int keyCode) const
{
   const auto it = mKeyMap.find(keyCode);
   assert(it != mKeyMap.end());

   return it->second.texture;
}

void Keyboard::selectKey(int keyCode)
{
   const auto it = mKeyMap.find(keyCode);
   assert(it != mKeyMap.end());

   it->second.available = false;
}

void Keyboard::deselectKey(int keyCode)
{
   const auto it = mKeyMap.find(keyCode);
   assert(it != mKeyMap.end());

   it->second.available = true;
}


void Keyboard::drawKey(int keyCode)
{
   const auto it = mKeyMap.find(keyCode);
   assert(it != mKeyMap.end());

   gl::draw(it->second.texture, getKeyRectf(keyCode));
}

void Keyboard::drawAllKeys()
{
   for (auto& i : mKeyMap) {
      drawKey(i.first);
   }
}