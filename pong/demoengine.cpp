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

#include "demoengine.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <audio/pcm.h>
#include <audio/player.h>

#include <game/audioenginefeature.h>
#include <game/inputenginefeature.h>
#include <game/scenemanager.h>

#include "pongscene.h"

DemoEngine::DemoEngine(void)
    : Game::Engine()
    , m_audio_pcm(0)
    , m_audio_player(0)
    , m_main_scene(0)
{
	/*
	 * Features have to be added before initialization, they will be
	 * automatically freed unless removed manually.
	 */

	/*
	 * Add audio support (Required for OpenAL)
	 */
	addFeature(new Game::AudioEngineFeature);

	/*
	 * Add extra input support (Required for Raspberry Pi [evdev])
	 */
	addFeature(new Game::InputEngineFeature);
}

DemoEngine::~DemoEngine(void)
{
}

bool
DemoEngine::initialize(void)
{
	m_audio_pcm = new Audio::PCM(44100, 16, 1);

	m_audio_player = new Audio::Player;
	m_audio_player->setPCM(m_audio_pcm);

	m_main_scene = new PongScene;
	sceneManager()->pushScene(m_main_scene);

	return(true);
}

void
DemoEngine::finalize(void)
{
	sceneManager()->popScene();

	delete m_main_scene, m_main_scene = 0;
	delete m_audio_player, m_audio_player = 0;
	delete m_audio_pcm, m_audio_pcm = 0;
}

Audio::Player *
DemoEngine::audioPlayer()
{
	return(m_audio_player);
}

void
DemoEngine::update(float delta)
{
	m_audio_player->tick();
}

