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
#include "MenuDuration.h"
#include "cinder/app/KeyEvent.h"

using namespace ci;
using namespace ci::app;

MenuDuration::MenuDuration()
{
   mHeader = "GAME DURATION";
   mOptions.emplace_back("1 minute");
   mOptions.emplace_back("2 minutes");
   mOptions.emplace_back("3 minutes");
   mOptions.emplace_back("4 minutes");
   mOptions.emplace_back("5 minutes");
}

void MenuDuration::updateImpl(TypingTutorInterface* pApp)
{
   assert(pApp != nullptr);

   const auto duration = static_cast<Duration>(mSelectedOption);

   switch (duration) {
      case Duration::One  : pApp->setGameDuration( 60.0); break;
      case Duration::Two  : pApp->setGameDuration(120.0); break;
      case Duration::Three: pApp->setGameDuration(180.0); break;
      case Duration::Four : pApp->setGameDuration(240.0); break;
      case Duration::Five : pApp->setGameDuration(300.0); break;
   }

   pApp->setState(State::MenuApp);
}
