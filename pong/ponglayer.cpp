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

#include <audio/player.h>
#include <audio/oggtrack.h>

#include <game/positioncomponent.h>
#include <game/scene.h>

#include "demoengine.h"
#include "pongpaddle.h"

PongLayer::PongLayer(Game::IScene *s)
    : Game::EntitySceneLayer("pong", s)
    , m_player_entity(new PongPaddle(this))
    , m_computer_entity(new PongPaddle(this))
{
	/* background music */

	Audio::OggTrack *l_track = new Audio::OggTrack;
	l_track->setData(new Core::FileIO("assets/noragames-tropical_island.ogg"),
	                 true);

	Audio::Player *l_audio_player =
	    static_cast<DemoEngine *>(Game::Engine::Instance())->audioPlayer();
	l_audio_player->load("music", l_track);
	l_audio_player->play("music", -1);
	
	/* position player paddle */

	Game::PositionComponent *l_position;

	l_position = static_cast<Game::PositionComponent *>
	    (m_player_entity->getComponentType(Game::PositionComponent::Type()));
	l_position->setPosition(-300, 0);
	addEntity(m_player_entity);

	/* position computer paddle */

	l_position = static_cast<Game::PositionComponent *>
	    (m_computer_entity->getComponentType(Game::PositionComponent::Type()));
	l_position->setPosition(300, 0);
	addEntity(m_computer_entity);
}

PongLayer::~PongLayer(void)
{
	DemoEngine *l_engine =
	    static_cast<DemoEngine *>(Game::Engine::Instance());
	Audio::Player *l_audio_player = l_engine->audioPlayer();

	Audio::ITrack *l_track = l_audio_player->eject("music");
	delete l_track, l_track = 0;

	removeEntity(m_player_entity);

	delete m_player_entity, m_player_entity = 0;
	delete m_computer_entity, m_computer_entity = 0;
}

