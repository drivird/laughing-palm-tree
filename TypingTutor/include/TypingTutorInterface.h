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

#include "cinder/app/App.h"

enum class State { 
   MenuApp,
   MenuSkill, 
   MenuDuration,
   GamePosition,
   GameHomeRow,
   GameLetters,
   GameNumbers,
   NewGame,
   Continue,
   GameOver, 
   Quit,
   Last 
};

enum class Skill{ 
   Positioning, 
   HomeRow, 
   Letters, 
   Numbers, 
   Last 
};

class TypingTutorInterface : public ci::app::App {
   public:
      virtual ~TypingTutorInterface() = 0 {}
      virtual void setState(State state) = 0;
      virtual void goBackToPreviousState() = 0;
      virtual void setSkill(Skill skill) = 0;
      virtual void setGameDuration(double seconds) = 0;
      virtual bool isGameRunning() = 0;
};
