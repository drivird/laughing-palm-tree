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
#include "MenuSkill.h"
#include "cinder/app/KeyEvent.h"

using namespace ci;
using namespace ci::app;

MenuSkill::MenuSkill()
{
   mHeader = "SKILL LEVEL";
   mOptions.emplace_back("Basic positioning");
   mOptions.emplace_back("Home row");
   mOptions.emplace_back("Letters only");
   mOptions.emplace_back("Letter and numbers");
}

void MenuSkill::updateImpl(TypingTutorInterface* pApp)
{
   assert(pApp != nullptr);

   const auto skill = static_cast<Skill>(mSelectedOption);

   switch (skill) {
      case Skill::Positioning: pApp->setSkill(Skill::Positioning); break;
      case Skill::HomeRow    : pApp->setSkill(Skill::HomeRow    ); break;
      case Skill::Letters    : pApp->setSkill(Skill::Letters    ); break;
      case Skill::Numbers    : pApp->setSkill(Skill::Numbers    ); break;
   }

   pApp->setState(State::MenuApp);
}
