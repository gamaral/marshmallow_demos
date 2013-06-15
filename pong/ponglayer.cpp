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

#include <core/identifier.h>

#include <graphics/backend.h>

#include <game/movementcomponent.h>
#include <game/positioncomponent.h>
#include <game/sizecomponent.h>

#include "aicomponent.h"
#include "inputcomponent.h"
#include "pongball.h"
#include "pongcourt.h"
#include "pongpaddle.h"
#include "pongscore.h"
#include "pongwall.h"

PongLayer::PongLayer(Game::IScene *s)
    : Game::EntitySceneLayer("pong", s)
{
	Game::PositionComponent *l_position;
	PongPaddle *l_paddle;

	addEntity(new PongCourt(this));

	/* position top/bottom walls */

	addEntity(new PongWall("top-wall", this));
	addEntity(new PongWall("bottom-wall", this));

	/* position computer paddle */

	l_paddle = new PongPaddle("computer", this);
	l_paddle->addComponent(new AIComponent("ai", l_paddle));
	addEntity(l_paddle);

	/* position player paddle */

	l_paddle = new PongPaddle("player", this);
	l_paddle->addComponent(new InputComponent("input", l_paddle));
	l_paddle->addComponent(new Game::MovementComponent("movement", l_paddle));
	addEntity(l_paddle);

	/* scores */
	
	addEntity(new PongScore("p1score", this));
	addEntity(new PongScore("p2score", this));

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

	resetCourt();
}

PongLayer::~PongLayer(void)
{
}

void
PongLayer::resetCourt(void)
{
	Game::PositionComponent *l_position;
	Game::SizeComponent *l_size;
	PongPaddle *l_paddle;
	PongScore *l_score;
	PongWall *l_wall;
	
	const Math::Size2f &l_world_size = Graphics::Backend::Size();

	l_wall = static_cast<PongWall *>(getEntity("top-wall"));
	l_position = static_cast<Game::PositionComponent *>
	    (l_wall->getComponentType(Game::PositionComponent::Type()));
	l_position->setPosition(0, l_world_size.height / 2);
	l_size = static_cast<Game::SizeComponent *>
	    (l_wall->getComponentType(Game::SizeComponent::Type()));
	l_size->setWidth(l_world_size.width);

	l_wall = static_cast<PongWall *>(getEntity("bottom-wall"));
	l_position = static_cast<Game::PositionComponent *>
	    (l_wall->getComponentType(Game::PositionComponent::Type()));
	l_position->setPosition(0, l_world_size.height / -2);
	l_size = static_cast<Game::SizeComponent *>
	    (l_wall->getComponentType(Game::SizeComponent::Type()));
	l_size->setWidth(l_world_size.width);

	l_paddle = static_cast<PongPaddle *>(getEntity("computer"));
	l_position = l_paddle->position();
	l_position->setPosition(l_world_size.width / 2, 0);
	l_position->translateX(-20);

	l_paddle = static_cast<PongPaddle *>(getEntity("player"));
	l_position = l_paddle->position();
	l_position->setPosition(l_world_size.width / -2, 0);
	l_position->translateX(20);

	l_score = static_cast<PongScore *>(getEntity("p1score"));
	l_score->position()->setPosition(-180, 230);

	l_score = static_cast<PongScore *>(getEntity("p2score"));
	l_score->position()->setPosition(180, 230);
}
