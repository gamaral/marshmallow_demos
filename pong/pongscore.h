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

#pragma once

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#ifndef MARSHMALLOW_DEMOS_PONGSCORE_H
#define MARSHMALLOW_DEMOS_PONGSCORE_H 1

#include <game/entity.h>
#include <event/ieventlistener.h>

MARSHMALLOW_NAMESPACE_BEGIN
namespace Graphics { struct ITextureData; }

namespace Game { class PositionComponent;
                 class TextComponent;
                 class TilesetComponent; }
MARSHMALLOW_NAMESPACE_END

MARSHMALLOW_NAMESPACE_USE

class PongScore : public Game::Entity
                , public Event::IEventListener
{
	Graphics::ITextureData  *m_texture_data;
	Game::PositionComponent *m_position_component;
	Game::TilesetComponent  *m_tileset_component;
	Game::TextComponent     *m_text_component;
	int m_score;
public:
	PongScore(const Core::Identifier &id, Game::EntitySceneLayer *layer);

	virtual ~PongScore(void);

	Game::PositionComponent * position(void) const
	    { return m_position_component; }
	Game::TextComponent * text(void) const
	    { return m_text_component; }
	Game::TilesetComponent * tileset(void) const
	    { return m_tileset_component; }
	
	void addPoint(void);
	void reset(void);

public: /* reimp */

	VIRTUAL bool handleEvent(const Event::IEvent &event);

private:
	void updateTextComponent(void);
};

#endif
