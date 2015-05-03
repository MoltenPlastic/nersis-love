#pragma once

#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_DLL
    #ifdef __GNUC__
      #define NERSIS_API __attribute__ ((dllexport))
    #else
      #define NERSIS_API __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #else
    #ifdef __GNUC__
      #define NERSIS_API __attribute__ ((dllimport))
    #else
      #define NERSIS_API __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #endif
#else
  #if __GNUC__ >= 4
    #define NERSIS_API __attribute__ ((visibility ("default")))
  #else
    #define NERSIS_API
  #endif
#endif

#define LUA_LIB
extern "C" {
	#include <lua.h>
	#include <lauxlib.h>
	#include <luaconf.h>
}

#include <string>
#include <vector>
#include <thread/LuaThread.h>
#include <keyboard/Keyboard.h>

#include <SDL_events.h>

namespace nersis {
	class NERSIS_API NModule {
		public:
		std::string name;
	
		NModule() {name = "daddys_little_failure";}; //"On Load"
		virtual ~NModule() {}; //"On Unload"
	
		virtual void load() {};
		virtual void update(double dt) {};
		virtual void draw() {};
	
		virtual void focus(bool windowFocus) {};
		virtual void keyPressed(love::keyboard::Keyboard::Key key, bool isRepeat) {};
		virtual void keyReleased(love::keyboard::Keyboard::Key key) {};
		virtual void mouseFocus(bool mouseFocus) {};
		virtual void mouseMoved(int x, int y, int dx, int dy) {};
		virtual void mousePressed(int x, int y, int button) {};
		virtual void mouseReleased(int x, int y, int button) {};
		virtual bool quit() {};
		virtual void resize(int w, int h) {};
		virtual void textInput(std::string text) {};
		virtual void threadError(love::thread::LuaThread *thread, std::string error) {};
		virtual void visible(bool visibility) {};
		
		virtual void sdlEvent(SDL_Event e) {};
	};

	NERSIS_API void registerModule(NModule *module);
	NERSIS_API NModule *getModule(std::string name);
	template <typename T>
	static T *getModule(std::string name) {
		return (T*) getModule(name);
	}
	NERSIS_API std::vector<NModule*> moduleList();
}
