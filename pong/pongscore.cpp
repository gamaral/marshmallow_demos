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

#include "pongscore.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <core/identifier.h>

#include <graphics/factory.h>
#include <graphics/itexturedata.h>
#include <graphics/tileset.h>

#include <game/positioncomponent.h>
#include <game/textcomponent.h>
#include <game/tilesetcomponent.h>

#include <cstdio>

PongScore::PongScore(const Core::Identifier &i, Game::EntitySceneLayer *l)
    : Entity(i, l)
    , m_texture_data(0)
    , m_position_component(0)
    , m_tileset_component(0)
    , m_text_component(0)
{
	m_position_component = new Game::PositionComponent("position", this);
	addComponent(m_position_component);

	m_texture_data = Graphics::Factory::CreateTextureData();
	m_texture_data->load("assets/terminus.png");

	Graphics::Tileset *l_tileset = new Graphics::Tileset;
	l_tileset->setTextureData(m_texture_data);
	l_tileset->setTileSize(Math::Size2i(16, 32));

	m_tileset_component = new Game::TilesetComponent("tileset", this);
	m_tileset_component->setTileset(l_tileset);
	addComponent(m_tileset_component);

	m_text_component = new Game::TextComponent("score", this);
	m_text_component->setScale(2);
	addComponent(m_text_component);
	reset();
}

PongScore::~PongScore(void)
{
	delete m_texture_data, m_texture_data = 0;
}

void
PongScore::addPoint(void)
{
	++m_score;
	updateTextComponent();
}

void
PongScore::reset(void)
{
	m_score = 0;
	updateTextComponent();
}

void
PongScore::updateTextComponent(void)
{
	char l_str[4];
	sprintf(l_str, "%d", m_score);
	m_text_component->setText(l_str);
}

