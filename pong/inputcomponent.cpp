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

#include "inputcomponent.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <core/logger.h>
#include <core/type.h>

#include <event/eventmanager.h>
#include <event/keyboardevent.h>

#include <game/entity.h>
#include <game/movementcomponent.h>

#define PADDLE_ACCEL 500

InputComponent::InputComponent(const Core::Identifier &i, Game::IEntity *e)
    : Component(i, e)
    , m_movement_component(0)
{
	Event::EventManager *l_manager = Event::EventManager::Instance();
	l_manager->connect(this, Event::KeyboardEvent::Type());
}

InputComponent::~InputComponent(void)
{
	Event::EventManager *l_manager = Event::EventManager::Instance();
	l_manager->disconnect(this, Event::KeyboardEvent::Type());
}

void
InputComponent::update(float)
{
	if (!m_movement_component) {
		m_movement_component = static_cast<Game::MovementComponent *>
		        (entity()->getComponentType(Game::MovementComponent::Type()));
		return;
	}

	if (m_state == Stopping) {
		m_movement_component->
		    setAccelerationY(-4 * m_movement_component->velocityY());
	}
}

bool
InputComponent::handleEvent(const Event::IEvent &e)
{
	if (!m_movement_component)
	    return(false);

	if (e.type() == Event::KeyboardEvent::Type()) {
		const Event::KeyboardEvent &l_kevent =
		    static_cast<const Event::KeyboardEvent &>(e);

		switch (l_kevent.key()) {
		case Input::Keyboard::KBK_UP:
			if (l_kevent.action() == Input::Keyboard::KeyPressed)
				moveUp();
			else if (l_kevent.action() == Input::Keyboard::KeyReleased) {
				stop();
			}
			return(true);

		case Input::Keyboard::KBK_DOWN:
			if (l_kevent.action() == Input::Keyboard::KeyPressed)
				moveDown();
			else if (l_kevent.action() == Input::Keyboard::KeyReleased) {
				stop();
			}
			return(true);
		}
	}
	return(false);
}

const Core::Type &
InputComponent::Type(void)
{
	static const Core::Type s_type("InputComponent");
	return(s_type);
}

void
InputComponent::moveUp(void)
{
	m_state = Moving;
	m_movement_component->setAccelerationY(PADDLE_ACCEL);
}

void
InputComponent::moveDown(void)
{
	m_state = Moving;
	m_movement_component->setAccelerationY(-PADDLE_ACCEL);
}

void
InputComponent::stop(void)
{
	if (m_movement_component->velocityY() != 0) {
		m_movement_component->
		    setAccelerationY(-m_movement_component->velocityY());
		m_state = Stopping;
	} else m_state = Idle;
}

