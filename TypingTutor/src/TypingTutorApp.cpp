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
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"
#include "cinder/audio/audio.h"
#include "cinder/Rand.h"

#include "TypingTutorInterface.h"
#include "MenuApp.h"
#include "MenuSkill.h"
#include "MenuDuration.h"
#include "GamePositioning.h"
#include "GameHomeRow.h"
#include "GameLetters.h"
#include "GameNumbers.h"

using namespace ci;
using namespace ci::app;

class TypingTutorApp : public TypingTutorInterface {
  public:
   virtual ~TypingTutorApp() = default;

	virtual void setup() override;
	virtual void update() override;
	virtual void draw() override;
   virtual void keyDown(KeyEvent event) override;

   virtual void setState(State state) override final;
   virtual void goBackToPreviousState() override final;
   virtual void setSkill(Skill skill) override final;
   virtual void setGameDuration(double seconds) override final;
   virtual bool isGameRunning() override final;

   private:
      using UniqueAppState = std::unique_ptr<AppState>;
      using VUniqueAppState = std::vector<UniqueAppState>;

      void drawBg();
      void setupSoundTrack();
      void resetGame();

      gl::TextureRef mBg;
      audio::GainNodeRef mSoundTrackGain;
      audio::BufferPlayerNodeRef mSoundTrackPlayerNode;
      std::unique_ptr<AppState> mMenu;
      AppState* mAppStatePtr{ nullptr };
      State mCurrentState{ State::Last };
      VUniqueAppState mAppStates;
      double mGameDuration{ 180.0 };
      Skill mSkill{ Skill::Positioning };
      State mCurrentGame{ State::Last };
};

void TypingTutorApp::setup()
{
   try {
      mBg = gl::Texture::create(loadImage(L"images/background.png"));
      setupSoundTrack();
      
      mAppStates.clear();
      mAppStates.resize(static_cast<int>(State::Last));
      mAppStates[static_cast<int>(State::MenuApp     )].reset(new MenuApp                       );
      mAppStates[static_cast<int>(State::MenuSkill   )].reset(new MenuSkill                     );
      mAppStates[static_cast<int>(State::MenuDuration)].reset(new MenuDuration                  );
   }
   catch (Exception &exc) {
      CI_LOG_EXCEPTION("failed to load resources.", exc);
   }

   Rand::randomize();

   setState(State::MenuApp);
}

void TypingTutorApp::setupSoundTrack()
{
   auto ctx = audio::Context::master();

   // create a SourceFile and set its output samplerate to match the Context.
   audio::SourceFileRef soundTrackFile = audio::load(loadFile(L"sounds/soundtrack.mp3"));
   
   // load the entire sound file into a BufferRef, and construct a BufferPlayerNode with this.
   audio::BufferRef soundTrackBuffer = soundTrackFile->loadBuffer();
   mSoundTrackPlayerNode = ctx->makeNode(new audio::BufferPlayerNode(soundTrackBuffer));
   
   // add a Gain to reduce the volume
   mSoundTrackGain = ctx->makeNode(new audio::GainNode(0.2f));
   
   // connect and enable the Context
   mSoundTrackPlayerNode >> mSoundTrackGain >> ctx->getOutput();
   ctx->enable();
   
   mSoundTrackPlayerNode->setLoopEnabled();
   mSoundTrackPlayerNode->start();
}

void TypingTutorApp::update()
{
   if (mAppStatePtr){ mAppStatePtr->update(this); }
}

void TypingTutorApp::draw()
{
   hideCursor();

   gl::clear(Color(0, 0, 0));
   gl::enableAlphaBlending();

   static bool OnceFlag = true;
   if (OnceFlag) {
      OnceFlag = false;
      setWindowSize(mBg->getSize());
      const auto displayWidth = getWindow()->getDisplay()->getWidth();
      const auto displayHeight = getWindow()->getDisplay()->getHeight();
      const auto posX = (displayWidth - getWindowWidth()) / 2;
      const auto posY = (displayHeight - getWindowHeight()) / 2;
      setWindowPos(posX, posY);
   }

   drawBg();

   if (mAppStatePtr) { mAppStatePtr->draw(*this); }
}

void TypingTutorApp::drawBg()
{
   if (mBg) {
      Rectf destRect = Rectf(mBg->getBounds()).getCenteredFit(getWindowBounds(), false);

      const auto GameRunningNow = 
         (mCurrentState == State::GamePosition ||
          mCurrentState == State::GameHomeRow ||
          mCurrentState == State::GameLetters ||
          mCurrentState == State::GameNumbers);

      if (GameRunningNow) { 
         gl::color(ColorA(1.0f, 1.0f, 1.0f, 0.7f)); 
      }

      gl::draw(mBg, destRect);

      if (GameRunningNow) { 
         gl::color(Colorf(1.0f, 1.0f, 1.0f)); 
      }
   }
}

void TypingTutorApp::keyDown(KeyEvent event)
{
   const auto code = event.getCode();
   if (mAppStatePtr) { mAppStatePtr->keyDown(code); }
}

void TypingTutorApp::setState(State state)
{
   assert(state != State::Last);
   mCurrentState = state;
   
   switch (state) {
      case State::NewGame : resetGame()           ; break;
      case State::Continue: setState(mCurrentGame); break;
      case State::Quit    : quit()                ; break;
      default:
         const auto iState = static_cast<int>(mCurrentState);
         mAppStatePtr = mAppStates[iState].get();
   }
}

void TypingTutorApp::resetGame()
{
   if (mCurrentGame != State::Last) {
      auto& rState = mAppStates[static_cast<int>(mCurrentGame)];
      rState.reset(nullptr);
   }

   switch (mSkill) {
      case Skill::Positioning: mCurrentGame = State::GamePosition; break;
      case Skill::HomeRow    : mCurrentGame = State::GameHomeRow ; break;
      case Skill::Letters    : mCurrentGame = State::GameLetters ; break;
      case Skill::Numbers    : mCurrentGame = State::GameNumbers ; break;
   }

   auto& rState = mAppStates[static_cast<int>(mCurrentGame)];

   switch (mCurrentGame) {
      case State::GamePosition: rState.reset(new GamePositioning(mGameDuration)); break;
      case State::GameHomeRow : rState.reset(new GameHomeRow    (mGameDuration)); break;
      case State::GameLetters : rState.reset(new GameLetters    (mGameDuration)); break;
      case State::GameNumbers : rState.reset(new GameNumbers    (mGameDuration)); break;
   }

   auto pGame = static_cast<Game*>(rState.get());

   setState(mCurrentGame);
}

void TypingTutorApp::goBackToPreviousState()
{
   if (mCurrentState == State::MenuApp) {
      if ( isGameRunning() ) {
         setState(mCurrentGame);
      }
   }
   else {
      setState(State::MenuApp);
   }
}

void TypingTutorApp::setSkill(Skill skill)
{
   assert(skill != Skill::Last);
   mSkill = skill;
}

void TypingTutorApp::setGameDuration(double seconds)
{
   mGameDuration = seconds;
}

bool TypingTutorApp::isGameRunning()
{
 return (mCurrentGame == State::GamePosition ||
         mCurrentGame == State::GameHomeRow  || 
         mCurrentGame == State::GameLetters  ||
         mCurrentGame == State::GameNumbers);
}

CINDER_APP(TypingTutorApp, RendererGl, [](App::Settings *settings) {
   settings->setResizable(false);
})
