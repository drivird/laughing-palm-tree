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
#include "Toon.h"

#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

using namespace ci;

const double Toon::SPAWNING_DURATION = 0.15;
const double Toon::INGAME_DURATION = 5.0;
const double Toon::GRAVITY = 1000.0;

Toon::Toon(const fs::path& filename)
{
   mTexture = gl::Texture::create(loadImage(filename));
}

void Toon::spawn()
{
   mState = State::Spawning;
   mTimer.start();
}

void Toon::draw(const TypingTutorInterface& rApp, const ci::Rectf& keyRect)
{
   Rectf toonRect(mTexture->getBounds());

   const auto keyCenter = keyRect.getCenter();
   const auto startingPos = keyCenter + vec2(0, 0.5 * (keyRect.getHeight() + toonRect.getHeight()));

   switch (mState) {

      case State::Spawning: {
         toonRect.offsetCenterTo(startingPos);
         const auto scale = 1.0 - std::max((SPAWNING_DURATION - mTimer.getSeconds()) / SPAWNING_DURATION, 0.0);
         toonRect.scaleCentered(static_cast<float>(scale));
      } break;

      case State::InGame: {
         toonRect.offsetCenterTo(startingPos);
      } break;

      case State::Hurled: {
         // see: https://fr.wikipedia.org/wiki/Balistique
         // zmax = ((vx0 * sin(alpha))^2) / (2g) + z0
         // v0 = sqrt(2g * (zmax - z0)) / sin(alpha)
         const auto zmax = startingPos.y + 0.5 * toonRect.getHeight() + 10;
         const auto g = GRAVITY;
         const auto alpha = mAngle * 3.14159265359 / 180.0;
         const auto v0 = std::sqrt(2 * g * zmax) / std::sin(alpha);
         const auto attenuation = 0.1 * mScore;
         const auto velocity = v0 * attenuation * attenuation;
         // x(t) = vx0 * t + x0
         // z(t) = -0.5 * g * t^2 + vz0 * t + z0
         const auto t = mTimer.getSeconds();
         const auto x0 = startingPos.x;
         const auto vx0 = velocity * std::cos(alpha);
         const auto x = vx0 * t + x0;
         const auto z0 = startingPos.y;
         const auto vz0 = velocity * std::sin(alpha);
         const auto z = 0.5 * g * t * t - vz0 * t + z0;
         const auto currentPos = vec2(x, z);
         toonRect.offsetCenterTo(currentPos);
      } break;
   }

   gl::draw(mTexture, toonRect);

   if (!toonRect.intersects(rApp.getWindowBounds())) {
      mState = State::Available;
      mTimer.stop();
      mScore = 0.0;
      mAngle = 0.0;
   }
}

void Toon::update()
{
   switch (mState) {

      case State::Spawning: {
         if (mTimer.getSeconds() > SPAWNING_DURATION) {
            mState = State::InGame;
            mTimer.stop();
            mTimer.start();
         }
      }
   }
}

double Toon::hurl()
{
   mState = State::Hurled;
   mAngle = static_cast<double>(Rand::randFloat(70.0f, 110.0f));
   mScore = std::ceil(10.0 * std::max((INGAME_DURATION - mTimer.getSeconds()) / INGAME_DURATION, 0.0));
   mTimer.stop();
   mTimer.start();
   return mScore;
}