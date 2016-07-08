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
#include "Game.h"

#include "cinder/Rand.h"
#include "cinder/gl/gl.h"

#include <algorithm>
#include <sstream>

using namespace ci;
using namespace ci::app;

const ColorA Game::TRANSPARENT_BLACK = ColorA(0, 0, 0, 0.7f);
const Colorf Game::GREEN = Colorf(75.0f / 255.0f, 170.0f / 255.0f, 66.0f / 255.0f);
const Colorf Game::WHITE = Colorf(1.0f, 1.0f, 1.0f);
const double Game::STREAK_BONUS_EXPIRATION = 1.0;

Game::Game(double duration, double gameSpeedCoef) :
mDuration(duration),
mGameSpeedCoef(gameSpeedCoef)
{
   loadToons();
   loadSounds();
}

void Game::loadToons()
{
   try {
      auto n = 0;
      while (true) {
         std::ostringstream filename;
         filename << "images/toon" << n++ << ".png";
         Toon toon(filename.str(), mGameSpeedCoef);
         mToons.push_back(toon);
      }
   }
   catch (Exception) {
      ; // it's expected; nothing to do
   }

   const auto NbToonsLoaded = static_cast<int>(mToons.size());
   mMaxNbToons = std::min<int>(NbToonsLoaded, mMaxNbToons);
}

void Game::loadSounds()
{
   auto ctx = audio::Context::master();

   loadSound("sounds/grow.mp3"  , 0.3f, ctx, &mToonGrowPlayerNode   , &mToonGrowGain   );
   loadSound("sounds/shrink.mp3", 0.3f, ctx, &mToonShrinkPlayerNode , &mToonShrinkGain );
   loadSound("sounds/boing1.mp3", 1.0f, ctx, &mToonBoing1PlayerNode , &mToonBoing1Gain );
   loadSound("sounds/boing2.mp3", 1.0f, ctx, &mToonBoing2PlayerNode , &mToonBoing2Gain );
   loadSound("sounds/boing3.mp3", 1.0f, ctx, &mToonBoing3PlayerNode , &mToonBoing3Gain );
   loadSound("sounds/buzz.mp3"  , 0.3f, ctx, &mToonBuzzPlayerNode   , &mToonBuzzGain   );
   loadSound("sounds/streak.mp3", 1.0f, ctx, &mStreakBonusPlayerNode, &mStreakBonusGain);

   ctx->enable();
}

void Game::loadSound(
   const fs::path& path,
   float gain,
   audio::Context* ctx, 
   ci::audio::BufferPlayerNodeRef* pPlayerNodeRef, 
   ci::audio::GainNodeRef* pGainNodeRef)
{
   // create a SourceFile and set its output samplerate to match the Context.
   audio::SourceFileRef audioFile = audio::load(loadFile(path));

   // load the entire sound file into a BufferRef, and construct a BufferPlayerNode with this.
   audio::BufferRef audioBuffer = audioFile->loadBuffer();

   *pPlayerNodeRef = ctx->makeNode(new audio::BufferPlayerNode(audioBuffer));

   // add a Gain to adjust the volume
   *pGainNodeRef = ctx->makeNode(new audio::GainNode(gain));

   // connect and enable the Context
   *pPlayerNodeRef >> *pGainNodeRef >> ctx->getOutput();
}

void Game::update(TypingTutorInterface* pApp)
{
   if (mKeyEscapeFlag) {
      mKeyEscapeFlag = false;
      pApp->goBackToPreviousState();
      return; // changing state; make a quick return
   }

   if (mTimer.isStopped()) {
      mTimer.resume();
   }

   if (mTimer.getSeconds() + mTimePenalty > mDuration) {
      mState = GameState::GameOver;
   }
   else {
      if (mTimer.getSeconds() > mSpawningTime) {

         if (mState == GameState::Preset) {
            mState = GameState::Playing;
            mTimer.stop();
            mTimer.start();
         }

         const auto pToon = randSelectToon();
         const auto key = mKb.randSelectKey();
         if (pToon != nullptr && key != KeyEvent::KEY_UNKNOWN) {
            mKeyToonPairs.emplace_back(key, pToon);
            mKb.selectKey(key);
            pToon->spawn();
            mToonGrowPlayerNode->start();
         }
         updateNextSpawingTime();
      }

      for (auto& i : mKeyToonPairs) {
         const auto keyCode = i.first;
         const auto pToon = i.second;
         pToon->update();
      }

      mKeyToonPairs.erase(
         std::remove_if(
            mKeyToonPairs.begin(),
            mKeyToonPairs.end(),
            [](const KeyToonPair& r){ return r.second->isAvailable(); }),
         mKeyToonPairs.end());
   }

   const auto time = mTimer.getSeconds();
   mStreakBonuses.erase(
      std::remove_if(
         mStreakBonuses.begin(),
         mStreakBonuses.end(),
         [time](const StreakBonus& r){ return time > r.expiration; }),
      mStreakBonuses.end());
}

int Game::countInGameToons() const
{
   const auto n = std::count_if(mToons.cbegin(), mToons.cend(),
      [](const Toon& r){ return r.isInGame(); });

   return static_cast<int>(n);
}

int Game::countAvailableToons() const
{
   const auto n = std::count_if(mToons.cbegin(), mToons.cend(),
      [](const Toon& r){ return r.isAvailable(); });

   return static_cast<int>(n);
}

Toon* Game::randSelectToon()
{
   const auto nbToonsInGame = countInGameToons();

   if (nbToonsInGame < mMaxNbToons) {
      const auto nbToons = static_cast<int>(mToons.size());
      const auto nbAvailableToons = countAvailableToons();
      if (nbAvailableToons > 0) {
         auto randToonNumber = Rand::randInt(nbAvailableToons);
         for (auto& rToon : mToons) {
            if (rToon.isAvailable()) {
               if (--randToonNumber < 0) {
                  return &rToon;
               }
            }
         }
      }
   }
   return nullptr;
}

void Game::updateNextSpawingTime()
{
   const auto randNumber = Rand::randFloat(0.3f, 1.0f);
   const auto timeCoef = (mGameSpeedCoef > 0.0) ? mGameSpeedCoef : 1.0;
   mSpawningTime = mTimer.getSeconds() + timeCoef * randNumber;
}

void Game::draw(const TypingTutorInterface& rApp)
{
   gl::drawStringCentered("Esc: main menu, F1: screenshot (saved in screens folder)", vec2(0.5*rApp.getWindowWidth(), 5), TRANSPARENT_BLACK, Font("Comic Sans MS", 20));
   gl::drawStringCentered("Esc: main menu, F1: screenshot (saved in screens folder)", vec2(0.5*rApp.getWindowWidth(), 5), WHITE            , Font("Comic Sans MS", 20));

   mKb.draw(rApp);
   mTimeBar.draw(rApp, computeFillRatio(), mDuration);
   drawScore(rApp);
   drawStreak(rApp);

   for (auto it = mKeyToonPairs.crbegin(); it != mKeyToonPairs.crend(); ++it) {
      const auto keyCode = it->first;
      const auto pToon = it->second;
      const auto escapedFlag = pToon->draw(rApp, mKb.getKeyRectf(it->first));
      if (escapedFlag) {
         resetStreakCount();
         mToonShrinkPlayerNode->start();
         mKb.deselectKey(keyCode);
      }
   }

   for (auto it = mKeyToonPairs.crbegin(); it != mKeyToonPairs.crend(); ++it) {
      const auto pToon = it->second;
      if (pToon->isInGame()) {
         mKb.drawKey(it->first);
      }
   }

   if (mState == GameState::GameOver) {
      drawGameOver(rApp);
      if (mPerfectGameBonusOnceFlag && mPerfectGameFlag) {
         mPerfectGameBonusOnceFlag = false;
         newStreakBonus(mScore, rApp.getWindowCenter());
      }
   }

   for (const auto& i : mStreakBonuses) {
      const auto offset = STREAK_BONUS_EXPIRATION - std::max(i.expiration - mTimer.getSeconds(), 0.0);
      const auto pos = i.pos - vec2(0, 10.0 * offset);
      gl::drawStringCentered(i.bonus, pos, TRANSPARENT_BLACK, Font("Comic Sans MS", 75));
      gl::drawStringCentered(i.bonus, pos, WHITE            , Font("Comic Sans MS", 72));
   }
}

void Game::drawStreak(const TypingTutorInterface& rApp) const
{
   std::ostringstream ss;
   ss << mStreakCnt << "/" << mStreakThreshold;
   const auto pos = vec2(rApp.getWindowWidth(), rApp.getWindowHeight() - 45);
   gl::drawStringRight(ss.str(), pos, TRANSPARENT_BLACK, Font("Comic Sans MS", 38.5));
   gl::drawStringRight(ss.str(), pos, WHITE            , Font("Comic Sans MS", 38));
}


void Game::drawScore(const TypingTutorInterface& rApp) const
{
   std::ostringstream scoreStringStream;
   scoreStringStream << static_cast<int>(mScore);
   gl::drawStringCentered(scoreStringStream.str(), vec2(0.5*rApp.getWindowWidth(), 8), TRANSPARENT_BLACK, Font("Comic Sans MS", 75));
   gl::drawStringCentered(scoreStringStream.str(), vec2(0.5*rApp.getWindowWidth(), 8), WHITE            , Font("Comic Sans MS", 72));
}

void Game::drawGameOver(const TypingTutorInterface& rApp) const
{
   std::string text = mPerfectGameFlag ? "PERFECT\nGAME !!!" : "GAME OVER";
   const auto pos = vec2((float)getWindowWidth() * 0.5f, (float)getWindowHeight() * 0.3f);
   gl::drawStringCentered(text, pos, TRANSPARENT_BLACK, Font("Comic Sans MS", 185));
   gl::drawStringCentered(text, pos, WHITE            , Font("Comic Sans MS", 180));
}

double Game::computeFillRatio() const
{
   if (mState == GameState::Preset) {
      const auto ratio = 1.0 - std::min(mTimer.getSeconds() / mSpawningTime, 1.0);
      return ratio; 
   }

   const auto ratio = std::min((mTimer.getSeconds() + mTimePenalty) / mDuration, 1.0);
   return ratio;
}

void Game::keyDown(int code)
{
   if (code == KeyEvent::KEY_ESCAPE) {
      mKeyEscapeFlag = true;
      mTimer.stop();
   }
   else if (code == KeyEvent::KEY_F1) {
      std::ifstream ifs;
      std::ostringstream ss;
      auto fileNum = -1;
      do {
         ifs.close();
         ss.str(std::string());
         ss << "screens/tt" << ++fileNum << ".png";
         ifs.open(ss.str());
      } while (ifs.is_open());
      ifs.close();
      writeImage(ss.str(), copyWindowSurface());
   }
   else if (mState == GameState::Playing) {
      auto WrongKeyPressed = true;

      for (auto& i : mKeyToonPairs) {
         const auto keyCode = i.first;
         auto pToon = i.second;
         if (keyCode == code && pToon->isInGame()) {
            WrongKeyPressed = false;
            mKb.deselectKey(keyCode);
            const auto score = pToon->hurl();
            playBoing(score);
            mScore += score;
            incStreakCount(keyCode);
            break;
         }
      }

      if (WrongKeyPressed) {
         mTimePenalty += 1.0;
         mToonBuzzPlayerNode->start();
         resetStreakCount();
      }
   }
}

void Game::incStreakCount(int keyCode)
{
   if (++mStreakCnt == mStreakThreshold) {
      mStreakCntFloor = mStreakCnt;
      const auto pos = mKb.getKeyRectf(keyCode).getCenter();
      const auto bonus = 10 * mStreakThreshold;
      newStreakBonus(bonus, pos);
      const auto streakAdd = std::min(mStreakThreshold, mStreakMaxInc);
      mStreakThreshold += streakAdd;
   }
}

void Game::newStreakBonus(double bonus, vec2 pos)
{
   std::ostringstream bonusStringStream;
   bonusStringStream << "+" << bonus;

   StreakBonus streakBonus;
   streakBonus.pos = pos;
   streakBonus.bonus = bonusStringStream.str();
   streakBonus.expiration = mTimer.getSeconds() + STREAK_BONUS_EXPIRATION;
   mStreakBonuses.emplace_back(streakBonus);
   
   mScore += bonus;
   mStreakBonusPlayerNode->start();
}

void Game::resetStreakCount()
{
   mStreakCnt = mStreakCntFloor;
   mPerfectGameFlag = false;
}

void Game::playBoing(double score)
{
   if (score > 6.0) {
      mToonBoing1PlayerNode->start();
   }
   else if (score > 3.0) {
      mToonBoing2PlayerNode->start();
   }
   else {
      mToonBoing3PlayerNode->start();
   }
}
