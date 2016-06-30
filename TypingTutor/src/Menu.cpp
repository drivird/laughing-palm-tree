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
#include "Menu.h"
#include "cinder/app/KeyEvent.h"

using namespace ci;
using namespace ci::app;

const Colorf Menu::GREEN = Colorf(75.0f / 255.0f, 170.0f / 255.0f, 66.0f / 255.0f);
const Colorf Menu::YELLOW = Colorf(255.0f / 255.0f, 223.0f / 255.0f, 27.0f / 255.0f);

void Menu::update(TypingTutorInterface* pApp)
{
   assert(pApp != nullptr);

   if (mSelectionFlag){
      mSelectionFlag = false;

      if (mKeyEscapeFlag) {
         mKeyEscapeFlag = false;
         mSelectedOption = mSavedOption;
         pApp->goBackToPreviousState();
      }
      else {
         mSavedOption = mSelectedOption;
         updateImpl(pApp);
      }
   }
}

void Menu::draw(const TypingTutorInterface& rApp)
{
   TextLayout layout;
   layout.clear(Color(0, 0, 0));
   layout.setColor(GREEN);
   layout.setFont(Font("Comic Sans MS", 48));
   layout.setBorder(30, 30);
   layout.addCenteredLine(mHeader);

   for (auto i = 0; i < mOptions.size(); ++i) {
      addLine(i, &layout);
   }

   mMenu = gl::Texture2d::create(layout.render(true, false));

   if (mMenu){
      Rectf menuRect = Rectf(mMenu->getBounds()).getCenteredFit(rApp.getWindowBounds(), false);
      gl::draw(mMenu, menuRect);
   }
}

void Menu::addLine(int iOption, TextLayout* pLayout)
{
   if (mSelectedOption == iOption){
      pLayout->setColor(YELLOW);
   }

   pLayout->addLine(mOptions[iOption]);

   if (mSelectedOption == iOption){
      pLayout->setColor(GREEN);
   }
}

void Menu::keyDown(int code)
{
   if (code == KeyEvent::KEY_ESCAPE) {
      mKeyEscapeFlag = true;
      mSelectionFlag = true;
   }
   else {
      switch (code){
         case KeyEvent::KEY_DOWN    : ++mSelectedOption; break;
         case KeyEvent::KEY_UP      : --mSelectedOption; break;
         case KeyEvent::KEY_RETURN  :
         case KeyEvent::KEY_KP_ENTER: mSelectionFlag = true; break;
      }
      const auto NbOptions = static_cast<int>(mOptions.size());
      if (mSelectedOption < 0){ mSelectedOption = NbOptions - 1; }
      else if (mSelectedOption == NbOptions){ mSelectedOption = 0; }
   }
}
