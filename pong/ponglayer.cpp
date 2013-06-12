/*
 * Copyright (c) 2013, Guillermo A. Amaral B. (gamaral) <g@maral.me>
 * All rights reserved.
 *
 * This file is part of Marshmallow Game Engine.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of the project as a whole.
 */

#include "ponglayer.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <core/fileio.h>
#include <core/identifier.h>

#include <audio/oggtrack.h>
#include <audio/player.h>
#include <audio/wavetrack.h>

#include <event/eventmanager.h>

#include <graphics/backend.h>

#include <game/movementcomponent.h>
#include <game/positioncomponent.h>
#include <game/scene.h>

#include "aicomponent.h"
#include "ballbounceevent.h"
#include "demoengine.h"
#include "inputcomponent.h"
#include "pongball.h"
#include "pongcourt.h"
#include "pongpaddle.h"
#include "pongscore.h"
#include "pongwall.h"
#include "scoreevent.h"

PongLayer::PongLayer(Game::IScene *s)
    : Game::EntitySceneLayer("pong", s)
{
	Game::PositionComponent *l_position;
	PongPaddle *l_paddle;
	PongScore *l_score;
	PongWall *l_wall;

	const Math::Size2f &l_world_size = Graphics::Backend::Size();

	Audio::Player *l_audio_player =
	    static_cast<DemoEngine *>(Game::Engine::Instance())->audioPlayer();
	Audio::ITrack *l_track;

	/* background music */

	l_track = new Audio::OggTrack;
	static_cast<Audio::OggTrack *>(l_track)->
	    setData(new Core::FileIO("assets/noragames-tropical_island.ogg"),
	                     true);
	l_audio_player->load("music", l_track);
	l_audio_player->play("music", -1);
	
	/* sfx */

	l_track = new Audio::WaveTrack;
	static_cast<Audio::WaveTrack *>(l_track)->
	    setData(new Core::FileIO("assets/bounce.wav"), true);
	l_audio_player->load("bounce", l_track);

	l_track = new Audio::WaveTrack;
	static_cast<Audio::WaveTrack *>(l_track)->
	    setData(new Core::FileIO("assets/score.wav"), true);
	l_audio_player->load("score", l_track);
	
	addEntity(new PongCourt(this));

	/* position top/bottom walls */

	l_wall = new PongWall(this);
	l_position = static_cast<Game::PositionComponent *>
	    (l_wall->getComponentType(Game::PositionComponent::Type()));
	l_position->setPosition(0, l_world_size.height / 2);
	addEntity(l_wall);

	l_wall = new PongWall(this);
	l_position = static_cast<Game::PositionComponent *>
	    (l_wall->getComponentType(Game::PositionComponent::Type()));
	l_position->setPosition(0, l_world_size.height / -2);
	addEntity(l_wall);

	/* position computer paddle */

	l_paddle = new PongPaddle("computer", this);
	l_position = static_cast<Game::PositionComponent *>
	    (l_paddle->getComponentType(Game::PositionComponent::Type()));
	l_position->setPosition(l_world_size.width / 2, 0);
	l_position->translateX(-20);
	l_paddle->addComponent(new AIComponent("ai", l_paddle));
	addEntity(l_paddle);

	/* position player paddle */

	l_paddle = new PongPaddle("player", this);
	l_position = static_cast<Game::PositionComponent *>
	    (l_paddle->getComponentType(Game::PositionComponent::Type()));
	l_position->setPosition(l_world_size.width / -2, 0);
	l_position->translateX(20);
	l_paddle->addComponent(new InputComponent("input", l_paddle));
	l_paddle->addComponent(new Game::MovementComponent("movement", l_paddle));
	addEntity(l_paddle);

	/* scores */
	
	PongScore *p1score = new PongScore("p1score", this);
	p1score->position()->setPosition(-180, 230);
	addEntity(p1score);

	PongScore *p2score = new PongScore("p2score", this);
	p2score->position()->setPosition(180, 230);
	addEntity(p2score);

	/* STRESS TEST
	 *
	 * Pong balls collide with each other and the environment (walls and paddles)
	 *
	 */

#ifdef STRESS_TEST
	for (int i = 0; i < 100; ++i)
		addEntity(new PongBall(this));
#else
	/* drop a ball */

	addEntity(new PongBall(this));
#endif

	/*
	 * Register events
	 */
	Event::EventManager::Instance()->connect(this, BallBounceEvent::Type());
	Event::EventManager::Instance()->connect(this, ScoreEvent::Type());

}

PongLayer::~PongLayer(void)
{
	/*
	 * Disconnect registered events
	 */
	Event::EventManager::Instance()->disconnect(this, ScoreEvent::Type());
	Event::EventManager::Instance()->disconnect(this, BallBounceEvent::Type());

	DemoEngine *l_engine =
	    static_cast<DemoEngine *>(Game::Engine::Instance());
	Audio::Player *l_audio_player = l_engine->audioPlayer();

	Audio::ITrack *l_track;

	l_track = l_audio_player->eject("music");
	delete l_track, l_track = 0;

	l_track = l_audio_player->eject("bounce");
	delete l_track, l_track = 0;

	l_track = l_audio_player->eject("score");
	delete l_track, l_track = 0;
}

bool
PongLayer::handleEvent(const Event::IEvent &event)
{
	Audio::Player *l_audio_player =
	    static_cast<DemoEngine *>(Game::Engine::Instance())->
	        audioPlayer();

	if (event.type() == BallBounceEvent::Type())
		l_audio_player->play("bounce");
	else if (event.type() == ScoreEvent::Type())
		l_audio_player->play("score");

	return(false);
}

