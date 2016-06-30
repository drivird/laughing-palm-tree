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
#include "MenuApp.h"
#include "cinder/app/KeyEvent.h"

using namespace ci;
using namespace ci::app;

MenuApp::MenuApp()
{
   mHeader = "MAIN MENU";
   mOptions.emplace_back("New game");
   mOptions.emplace_back("Select skill level");
   mOptions.emplace_back("Select game duration");
   mOptions.emplace_back("Quit");
}

void MenuApp::updateImpl(TypingTutorInterface* pApp)
{
   assert(pApp != nullptr);

   const auto option = static_cast<Option>(mSelectedOption);

   switch (option) {
      case Option::NewGame : pApp->setState(State::NewGame     ); break;
      case Option::Skill   : pApp->setState(State::MenuSkill   ); break;
      case Option::Duration: pApp->setState(State::MenuDuration); break;
      case Option::Quit    : pApp->setState(State::Quit        ); break;
   }
}
