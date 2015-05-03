#pragma once

#include "core.h"

namespace nersis {
	namespace states {
		class State {
			public:
			virtual ~State();
			
			virtual void enter() {};
			virtual void leave() {};
			virtual void suspend() {};
			virtual void resume() {};
			
			virtual void update(double dt) {};
			virtual void draw() {};
	
			virtual void focus(bool windowFocus) {};
			virtual void keyPressed(love::keyboard::Keyboard::Key key, bool isRepeat) {};
			virtual void keyReleased(love::keyboard::Keyboard::Key key) {};
			virtual void mouseFocus(bool mouseFocus) {};
			virtual void mouseMoved(int x, int y, int dx, int dy) {};
			virtual void mousePressed(int x, int y, int button) {};
			virtual void mouseReleased(int x, int y, int button) {};
			virtual bool quit() {return false;};
			virtual void resize(int w, int h) {};
			virtual void textInput(std::string text) {};
			virtual void threadError(love::thread::LuaThread *thread, std::string error) {};
			virtual void visible(bool visibility) {};
		
			virtual void sdlEvent(SDL_Event e) {};
		};
		
		NERSIS_API void go(State *state);
		NERSIS_API void push(State *state);
		NERSIS_API void pop();
		NERSIS_API State *current();
	}
}
