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
#include "AppState.h"
#include "Keyboard.h"
#include "TimeBar.h"
#include "Toon.h"

#include "cinder/audio/audio.h"

class Game : public AppState {
   public:
      Game(double duration);
      virtual ~Game() = 0 {}

      virtual void update(TypingTutorInterface* pApp) override;
      virtual void draw(const TypingTutorInterface& rApp) override;
      virtual void keyDown(int code) override;

   protected:
      struct StreakBonus {
         std::string bonus;
         ci::vec2 pos;
         double expiration;
      };

      enum class GameState{ Preset, Playing, GameOver };

      static const ci::ColorA TRANSPARENT_BLACK;
      static const ci::Colorf GREEN;
      static const ci::Colorf WHITE;
      static const double STREAK_BONUS_EXPIRATION;

      using VToon = std::vector<Toon>;
      using KeyToonPair = std::pair<int, Toon*>;
      using VKeyToon = std::vector<KeyToonPair>;
      using VStreakBonus = std::vector<StreakBonus>;

      double computeFillRatio() const;
      void loadToons();
      void updateNextSpawingTime();
      int countAvailableToons() const;
      int countInGameToons() const;
      Toon* randSelectToon();
      void drawScore(const TypingTutorInterface& rApp) const;
      void drawStreak(const TypingTutorInterface& rApp) const;
      void drawGameOver(const TypingTutorInterface& rApp) const;
      void loadSounds();
      void playBoing(double score);
      void incStreakCount(int keyCode);
      void resetStreakCount();
      void newStreakBonus(double bonus, ci::vec2 pos);
      
      void loadSound(const ci::fs::path& path, float gain, ci::audio::Context* ctx,
         ci::audio::BufferPlayerNodeRef* pPlayerNodeRef, ci::audio::GainNodeRef* pGainNodeRef);

      GameState mState{ GameState::Preset };
      bool mKeyEscapeFlag{ false };
      bool mPerfectGameBonusOnceFlag{ true };
      bool mPerfectGameFlag{ true };
      double mDuration{ 0.0 };
      double mTimePenalty{ 0.0 };
      double mSpawningTime{ 3.0 };
      double mSpawningTimeCoef{ 1.0 };
      double mScore{ 0.0 };
      int mStreakCnt{ 0 };
      int mStreakCntFloor{ 0 };
      int mStreakThreshold{ 5 };
      int mStreakMaxInc{ 10 };
      int mMaxNbToons{ 1 };
      ci::Timer mTimer{ true };
      Keyboard mKb;
      TimeBar mTimeBar;
      VToon mToons;
      VKeyToon mKeyToonPairs;
      VStreakBonus mStreakBonuses;
      ci::audio::GainNodeRef mToonGrowGain;
      ci::audio::GainNodeRef mToonBoing1Gain;
      ci::audio::GainNodeRef mToonBoing2Gain;
      ci::audio::GainNodeRef mToonBoing3Gain;
      ci::audio::GainNodeRef mToonBuzzGain;
      ci::audio::GainNodeRef mStreakBonusGain;
      ci::audio::BufferPlayerNodeRef mToonGrowPlayerNode;
      ci::audio::BufferPlayerNodeRef mToonBoing1PlayerNode;
      ci::audio::BufferPlayerNodeRef mToonBoing2PlayerNode;
      ci::audio::BufferPlayerNodeRef mToonBoing3PlayerNode;
      ci::audio::BufferPlayerNodeRef mToonBuzzPlayerNode;
      ci::audio::BufferPlayerNodeRef mStreakBonusPlayerNode;
};
