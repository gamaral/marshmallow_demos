/*
 * Copyright 2013 Marshmallow Engine. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are
 * permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this list of
 *      conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright notice, this list
 *      of conditions and the following disclaimer in the documentation and/or other materials
 *      provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY MARSHMALLOW ENGINE ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL MARSHMALLOW ENGINE OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those of the
 * authors and should not be interpreted as representing official policies, either expressed
 * or implied, of Marshmallow Engine.
 */

#ifndef ENGINE_H
#define ENGINE_H 1

#include <game/enginebase.h>

#include <core/shared.h>

#include <audio/pcm.h>

#include <cstdio>
#include <string>

MARSHMALLOW_NAMESPACE_USE

class Demo : public Game::EngineBase
{
	/* Audio */

	Audio::SharedPCM m_audio_pcm;

	/* Levels */

	char m_current_level[FILENAME_MAX + 1];

	NO_ASSIGN_COPY(Demo);
public:
	Demo(void);
	virtual ~Demo(void);

	VIRTUAL bool initialize(void);
	VIRTUAL void finalize(void);
	VIRTUAL void tick(float delta);
	VIRTUAL bool handleEvent(const Event::IEvent &e);

	void loadLevel(const std::string &level = std::string());

	const Audio::WeakPCM pcm(void) const;
};

#endif

