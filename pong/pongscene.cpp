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

#include "pongscene.h"

/*!
 * @file
 *
 * @author Guillermo A. Amaral B. (gamaral) <g@maral.me>
 */

#include <core/fileio.h>
#include <core/identifier.h>

#include <event/eventmanager.h>

#include <audio/oggtrack.h>
#include <audio/player.h>
#include <audio/wavetrack.h>

#include <event/keyboardevent.h>
#include <event/quitevent.h>
#include <event/viewportevent.h>

#include <graphics/backend.h>
#include <graphics/display.h>

#include <game/collisionscenelayer.h>

#include "ballbounceevent.h"
#include "demoengine.h"
#include "ponglayer.h"
#include "scoreevent.h"

PongScene::PongScene(void)
    : Game::Scene("pong")
    , m_collision_layer(new Game::CollisionSceneLayer("collision", this))
    , m_pong_layer(new PongLayer(this))
{
	using namespace Event;

	Audio::Player *l_audio_player =
	    static_cast<DemoEngine *>(Game::Engine::Instance())->audioPlayer();
	Audio::ITrack *l_track;

	/* background music */

	l_track = new Audio::OggTrack;
	static_cast<Audio::OggTrack *>(l_track)->
	    setData(new Core::FileIO("assets/noragames-tropical_island.ogg"),
	                     true);
	l_audio_player->load("music", l_track);
	l_audio_player->play("music", -1);
	
	/* sfx */

	l_track = new Audio::WaveTrack;
	static_cast<Audio::WaveTrack *>(l_track)->
	    setData(new Core::FileIO("assets/bounce.wav"), true);
	l_audio_player->load("bounce", l_track);

	l_track = new Audio::WaveTrack;
	static_cast<Audio::WaveTrack *>(l_track)->
	    setData(new Core::FileIO("assets/score.wav"), true);
	l_audio_player->load("score", l_track);

	/*
	 * Register events
	 */
	Event::EventManager::Instance()->connect(this, BallBounceEvent::Type());
	Event::EventManager::Instance()->connect(this, ScoreEvent::Type());
	Event::EventManager::Instance()->connect(this, KeyboardEvent::Type());
	Event::EventManager::Instance()->connect(this, ViewportEvent::Type());

	/*
	 * Populate
	 */
	pushLayer(m_collision_layer);
	pushLayer(m_pong_layer);
}

PongScene::~PongScene(void)
{
	using namespace Event;

	/*
	 * Disconnect registered events
	 */
	Event::EventManager::Instance()->disconnect(this, ViewportEvent::Type());
	Event::EventManager::Instance()->disconnect(this, KeyboardEvent::Type());
	Event::EventManager::Instance()->disconnect(this, ScoreEvent::Type());
	Event::EventManager::Instance()->disconnect(this, BallBounceEvent::Type());

	/*
	 * Scene dependent audio
	 */
	DemoEngine *l_engine =
	    static_cast<DemoEngine *>(Game::Engine::Instance());
	Audio::Player *l_audio_player = l_engine->audioPlayer();

	Audio::ITrack *l_track;

	l_track = l_audio_player->eject("music");
	delete l_track, l_track = 0;

	l_track = l_audio_player->eject("bounce");
	delete l_track, l_track = 0;

	l_track = l_audio_player->eject("score");
	delete l_track, l_track = 0;
	removeLayer(m_pong_layer);
	delete m_pong_layer, m_pong_layer = 0;

	removeLayer(m_collision_layer);
	delete m_collision_layer, m_collision_layer = 0;
}

bool
PongScene::handleEvent(const Event::IEvent &event)
{
	using namespace Event;

	Audio::Player *l_audio_player =
	    static_cast<DemoEngine *>(Game::Engine::Instance())->
	        audioPlayer();

	if (event.type() == BallBounceEvent::Type())
		l_audio_player->play("bounce");
	else if (event.type() == ScoreEvent::Type())
		l_audio_player->play("score");
	
	/*
	 * Handle viewport creation
	 */
	else if (event.type() == ViewportEvent::Type()) {
		const ViewportEvent &l_event =
		    static_cast<const ViewportEvent &>(event);

		if (l_event.reason() == ViewportEvent::Destroyed)
			return(false);
	
		m_pong_layer->resetCourt();
	}
	else if (event.type() == KeyboardEvent::Type()) {
		const KeyboardEvent &l_event =
		    static_cast<const KeyboardEvent &>(event);

		if (l_event.action() == Input::Keyboard::KeyReleased)
			return(false);

		/*
		 * Quit game
		 */
		if (l_event.key() == Input::Keyboard::KBK_ESCAPE)
			EventManager::Instance()->queue(new Event::QuitEvent);

		/*
		 * Toggle fullscreen mode
		 */
		else if (l_event.key() == Input::Keyboard::KBK_F11) {
			Graphics::Display dpy = Graphics::Backend::Display();
			dpy.fullscreen = !dpy.fullscreen;
			Graphics::Backend::Setup(dpy);
			return(true);
		}
	}

	return(false);
}

