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

#include "pongball.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <core/identifier.h>

#include <graphics/backend.h>
#include <graphics/quadmesh.h>

#include <game/collidercomponent.h>
#include <game/movementcomponent.h>
#include <game/positioncomponent.h>
#include <game/rendercomponent.h>
#include <game/sizecomponent.h>

PongBall::PongBall(Game::EntitySceneLayer *l)
    : Game::Entity("ball", l)
    , m_movement_component(new Game::MovementComponent("movement", this))
    , m_position_component(new Game::PositionComponent("position", this))
{
	/* position */
	pushComponent(m_position_component);

	/* movement */
	m_movement_component->setVelocity(100, 180);
	pushComponent(m_movement_component);

	/* size */
	Game::SizeComponent *l_size_component =
	    new Game::SizeComponent("size", this);
	l_size_component->set(10, 10);
	pushComponent(l_size_component);

	/* collider */
	pushComponent(new Game::BounceColliderComponent("collider", this));

	/* render */
	Graphics::QuadMesh *l_mesh =
	    new Graphics::QuadMesh(l_size_component->size());
	l_mesh->setColor(Graphics::Color::White());
	Game::RenderComponent *l_render_component =
	    new Game::RenderComponent("render", this);
	l_render_component->setMesh(l_mesh);
	pushComponent(l_render_component);

}

PongBall::~PongBall(void)
{
}

Game::MovementComponent *
PongBall::movement(void) const
{
	return(m_movement_component);
}

Game::PositionComponent *
PongBall::position(void) const
{
	return(m_position_component);
}

void
PongBall::update(float d)
{
	Entity::update(d);

	const float l_whalf_width = Graphics::Backend::Size().width / 2.f;
	if ((m_position_component->positionX() < -l_whalf_width)
	    || (m_position_component->positionX() > l_whalf_width))
		m_position_component->setPosition(0, 0);
}

