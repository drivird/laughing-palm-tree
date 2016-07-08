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

#include "TypingTutorInterface.h"
#include "cinder/gl/Texture.h"

class Toon {
   public:
      Toon(const ci::fs::path& filename, double reducedTimeCoef);

      bool isAvailable() const;
      bool isInGame() const;
      bool isHurled() const;
      void spawn();
      bool draw(const TypingTutorInterface& rApp, const ci::Rectf& keyRect);
      void update();
      double hurl();

   private:
      void reset();

      enum class State{ Available, Spawning, InGame, Hurled, Escaping };

      static const double SPAWNING_DURATION;
      static const double ESCAPING_DURATION;
      static const double INGAME_DURATION;
      static const double SHAKING_DURATION;
      static const double GRAVITY;

      State mState{ State::Available };
      ci::gl::TextureRef mTexture;
      ci::Timer mTimer{ false };
      double mScore{ 0.0 };
      double mAngle{ 0.0 };
      double mIngameDuration{ 0.0 };
      double mShakingDuration{ 0.0 };
      bool mCanEscapeFlag{ false };
};

inline
bool Toon::isAvailable() const
{
   return (mState == State::Available);
}

inline 
bool Toon::isInGame() const
{
   return (mState == State::InGame);
}

inline
bool Toon::isHurled() const
{
   return (mState == State::Hurled);
}