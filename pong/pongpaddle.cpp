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

#include "pongpaddle.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <core/identifier.h>

#include <graphics/backend.h>
#include <graphics/quadmesh.h>

#include <game/collidercomponent.h>
#include <game/entityscenelayer.h>
#include <game/movementcomponent.h>
#include <game/positioncomponent.h>
#include <game/rendercomponent.h>
#include <game/sizecomponent.h>

#include "pongball.h"

#include <cstdio>

#define PADDLE_SPEED 166.f /* world-pixel's per second */

PongPaddle::PongPaddle(const Core::Identifier &i, Game::EntitySceneLayer *l)
    : Game::Entity(i, l)
    , m_position_component(new Game::PositionComponent("position", this))
{
	/* position */
	pushComponent(m_position_component);

	/* size */
	Game::SizeComponent *l_size_component = new Game::SizeComponent("size", this);
	l_size_component->set(10, 60);
	pushComponent(l_size_component);

	/* collider */
	pushComponent(new Game::BounceColliderComponent("collider", this));

	/* render */
	Graphics::QuadMesh *l_mesh = new Graphics::QuadMesh(l_size_component->size());
	Game::RenderComponent *l_render_component = new Game::RenderComponent("render", this);
	l_render_component->setMesh(l_mesh);
	pushComponent(l_render_component);

}

PongPaddle::~PongPaddle(void)
{
}

void
PongPaddle::update(float d)
{
	Entity::update(d);

	PongBall *l_ball =
	    static_cast<PongBall *>(layer()->getEntity("ball"));
	if (!l_ball) return;

	Game::MovementComponent *l_ball_mov = l_ball->movement();

	if ((l_ball_mov->velocity().x < 0 && m_position_component->positionX() > 0)
	    || (l_ball_mov->velocity().x > 0 && m_position_component->positionX() < 0))
		return;

	Game::SizeComponent *l_size_component =
	    static_cast<Game::SizeComponent *>(getComponent("size"));

	const float l_half_height = l_size_component->height() / 2.f;
	const float l_whalf_width = Graphics::Backend::Size().width / 2.f;
	const float l_whalf_height = Graphics::Backend::Size().height / 2.f;
	Game::PositionComponent *l_ball_pos = l_ball->position();

	if (l_ball_pos->positionY() > m_position_component->positionY()) {
		m_position_component->translateY(PADDLE_SPEED * d);

		if ((m_position_component->positionY() + l_half_height) > l_whalf_height)
			m_position_component->setPositionY(l_whalf_height - l_half_height);
	}

	if (l_ball_pos->positionY() < m_position_component->positionY()) {
		m_position_component->translateY(-PADDLE_SPEED * d);

		if ((m_position_component->positionY() - l_half_height) < -l_whalf_height)
			m_position_component->setPositionY(-l_whalf_height + l_half_height);
	}
}

