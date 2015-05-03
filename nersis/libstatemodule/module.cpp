#include "state.h"
#include "window/Window.h"
#include "graphics/opengl/Graphics.h"
#include "graphics/Color.h"
#include "common/Module.h"
#include <stack>

using namespace love;
using namespace love::window;
using namespace love::graphics;
using namespace nersis;

namespace nersis {
	namespace states {
		State::~State() {}
		
		static State *currentState = NULL;
		static std::stack<State*> stateStack;
		
		void go(State *state) {
			if (currentState)
				currentState->leave();
			
			currentState = state;
			
			if (currentState)
				currentState->enter();
		}
		
		void push(State *state) {
			if (currentState)
				currentState->suspend();
			
			stateStack.push(currentState);
			currentState = state;
			
			if (currentState)
				currentState->enter();
		}
		
		void pop() {
			if (currentState)
				currentState->leave();
			
			currentState = stateStack.top();
			stateStack.pop();
			
			if (currentState)
				currentState->resume();
		}
		
		State *current() {return currentState;}
		
		class StateModule : public NModule {
			public:
			StateModule() {
				name = "statemodule";
				printf("Hello from libstatemodule!\n");
			}
	
			virtual ~StateModule() {};
	
			virtual void update(double dt);
			virtual void draw();
	
			virtual void focus(bool windowFocus);
			virtual void keyPressed(love::keyboard::Keyboard::Key key, bool isRepeat);
			virtual void keyReleased(love::keyboard::Keyboard::Key key);
			virtual void mouseFocus(bool mouseFocus);
			virtual void mouseMoved(int x, int y, int dx, int dy);
			virtual void mousePressed(int x, int y, int button);
			virtual void mouseReleased(int x, int y, int button);
			virtual bool quit();
			virtual void resize(int w, int h);
			virtual void textInput(std::string text);
			virtual void threadError(love::thread::LuaThread *thread, std::string error);
			virtual void visible(bool visibility);
		
			virtual void sdlEvent(SDL_Event e);
		};

		void StateModule::update(double dt) {
			if (currentState)
				currentState->update(dt);
		}

		void StateModule::draw() {
			if (currentState)
				currentState->draw();
		}
		
		void StateModule::focus(bool windowFocus) {
			if (currentState)
				currentState->focus(windowFocus);
		}

		void StateModule::keyPressed(love::keyboard::Keyboard::Key key, bool repeat) {
			if (currentState)
				currentState->keyPressed(key, repeat);
		}
		
		void StateModule::keyReleased(love::keyboard::Keyboard::Key key) {
			if (currentState)
				currentState->keyReleased(key);
		}
		
		void StateModule::mouseFocus(bool mouseFocus) {
			if (currentState)
				currentState->mouseFocus(mouseFocus);
		}
		
		void StateModule::mouseMoved(int x, int y, int dx, int dy) {
			if (currentState)
				currentState->mouseMoved(x,y,dx,dy);
		}
		
		void StateModule::mousePressed(int x, int y, int button) {
			if (currentState)
				currentState->mousePressed(x,y,button);
		}
		
		void StateModule::mouseReleased(int x, int y, int button) {
			if (currentState)
				currentState->mouseReleased(x,y,button);
		}
		
		bool StateModule::quit() {
			if (currentState)
				return currentState->quit();
			return false;
		}
		
		void StateModule::resize(int x, int y) {
			if (currentState)
				currentState->resize(x,y);
		}
		
		void StateModule::textInput(std::string text) {
			if (currentState)
				currentState->textInput(text);
		}
		
		void StateModule::threadError(love::thread::LuaThread *thread, std::string error) {
			if (currentState)
				currentState->threadError(thread, error);
		}
		
		void StateModule::visible(bool visibility) {
			if (currentState)
				currentState->visible(visibility);
		}
		
		void StateModule::sdlEvent(SDL_Event event) {
			if (currentState)
				currentState->sdlEvent(event);
		}
		
		#include "luastate.cpp"
	}
}

static int nersis_state_push(lua_State *L) {
	nersis::states::push(new nersis::states::LuaState(L, 1));
	return 0;
}

static int nersis_state_pop(lua_State *L) {
	nersis::states::pop();
	return 0;
}

static int nersis_state_go(lua_State *L) {
	nersis::states::go(new nersis::states::LuaState(L, 1));
	return 0;
}

static const luaL_Reg nersis_statelib[] = {
	{"push", nersis_state_push},
	{"pop", nersis_state_pop},
	{"go", nersis_state_go},
	{NULL, NULL}
};

LUALIB_API int luaopen_nersis_statemodule(lua_State *L) { //A message to module authors: Do not expect lua_State to be defined!
	registerModule(new nersis::states::StateModule());
	luaL_register(L, "nersis.state", nersis_statelib);
	return 1;
}
