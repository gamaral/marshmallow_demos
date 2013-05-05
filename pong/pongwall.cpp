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

#include "pongwall.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <core/identifier.h>

#include <graphics/backend.h>
#include <graphics/quadmesh.h>

#include <game/collidercomponent.h>
#include <game/positioncomponent.h>
#include <game/sizecomponent.h>

PongWall::PongWall(Game::EntitySceneLayer *l)
    : Game::Entity("wall", l)
    , m_position_component(new Game::PositionComponent("position", this))
    , m_size_component(new Game::SizeComponent("size", this))
    , m_collider_component(new Game::BounceColliderComponent("collider", this))
{
	/* position */
	pushComponent(m_position_component);

	/* size */
	m_size_component->set(Graphics::Backend::Size().width, 10);
	pushComponent(m_size_component);

	/* collider */
	pushComponent(m_collider_component);
}

PongWall::~PongWall(void)
{
	removeComponent(m_collider_component);
	delete m_collider_component, m_collider_component = 0;

	removeComponent(m_size_component);
	delete m_size_component, m_size_component = 0;

	removeComponent(m_position_component);
	delete m_position_component, m_position_component = 0;
}
