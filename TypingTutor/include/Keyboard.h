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
#pragma once

#include "cinder/gl/Texture.h"
#include "TypingTutorInterface.h"

#include <map>

class Keyboard {
   public:
      Keyboard();

      void loadImageFile(const ci::fs::path& ImagePath);
      void draw(const TypingTutorInterface& rApp);
      void drawKey(int keyCode);
      void enableRow(int iRow);
      void enableCol(int iRow);
      void disableRow(int iCol);
      void disableCol(int iCol);
      void selectKey(int keyCode);
      void deselectKey(int keyCode);
      int randSelectKey() const;
      const ci::Rectf getKeyRectf(int key) const;
      const ci::gl::TextureRef getKeyTexture(int key) const;

   private:
      struct Key {
         Key() = default;
         Key(std::string c_, int row_, int col_, bool enabled_, bool available_) :
            c(c_), row(row_), col(col_), enabled(enabled_), available(available_) {}

         std::string c;
         int row{ 0 };
         int col{ 0 };
         bool enabled{ false };
         bool available{ false };
         ci::gl::TextureRef texture;
      };

      using KeyMap = std::map<int, Key>;

      static ci::Color getColumnColor(int col);
         
      template<bool b> void setEnableRow(int iRow);
      template<bool b> void setEnableCol(int iCol);
      int countAvailableKeys() const;
      void generateTextures();
      void drawAllKeys();

      KeyMap mKeyMap;
      ci::gl::TextureRef mTexture;
      ci::vec2 mTopLeft;
};

template<bool b>
void Keyboard::setEnableRow(int iRow)
{
   for (auto& i : mKeyMap) {
      if (i.second.row == iRow) {
         i.second.enabled = b;
      }
   }
}

template<bool b>
void Keyboard::setEnableCol(int iCol)
{
   for (auto& i : mKeyMap) {
      if (i.second.col == iCol) {
         i.second.enabled = b;
      }
   }
}

inline
void Keyboard::enableRow(int iRow)
{
   setEnableRow<true>(iRow);
}

inline
void Keyboard::enableCol(int iCol)
{
   setEnableCol<true>(iCol);
}

inline
void Keyboard::disableRow(int iRow)
{
   setEnableRow<false>(iRow);
}

inline
void Keyboard::disableCol(int iCol)
{
   setEnableCol<false>(iCol);
}
