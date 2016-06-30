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
#include "TimeBar.h"

#include <algorithm>
#include "cinder/gl/gl.h"

using namespace ci;

const ColorA TimeBar::TRANSPARENT_BLACK = ColorA(0, 0, 0, 0.7f);
const Colorf TimeBar::GREEN = Colorf(75.0f / 255.0f, 170.0f / 255.0f, 66.0f / 255.0f);
const Colorf TimeBar::WHITE = Colorf(1.0f, 1.0f, 1.0f);

void TimeBar::draw(const TypingTutorInterface& rApp, double fillRatio, double duration)
{
   const auto x1 = 15.0f;
   const auto y1 = 40.0f;
   const auto x2 = 105.0f;
   const auto y2 = rApp.getWindowHeight() - 40.0f;

   const auto rectTimeBg = Rectf(x1, y1, x2, y2);
   gl::color(TRANSPARENT_BLACK);
   gl::drawSolidRect(rectTimeBg);
   auto rectTimeFg = rectTimeBg.inflated(vec2(-2, -2));
   auto inflationY = 0.5 * rectTimeFg.getHeight() * fillRatio;
   rectTimeFg.inflate(vec2(0, -inflationY));
   rectTimeFg.offset(vec2(0, inflationY));
   gl::color(GREEN);
   gl::drawSolidRect(rectTimeFg);

   const auto nbParts = std::floor(duration / 60.0);
   const auto partLen = (y2 - y1) / nbParts;
   const auto offset = vec2(0.0, 1.0);

   for (auto i = 1; i < static_cast<int>(nbParts); ++i) {
      const auto start = vec2(x1, y1 + i * partLen);
      const auto stop  = vec2(0.66*(x1+x2), y1 + i * partLen);
      gl::color(ColorA(1.0f, 1.0f, 1.0f, 0.5f));
      gl::drawLine(start, stop);
      gl::color(ColorA(1.0f, 1.0f, 1.0f, 0.4f));
      gl::drawLine(start + offset, stop + offset);
   }
   
   gl::color(WHITE);
}