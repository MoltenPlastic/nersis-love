#pragma once

#define LUA_LIB
extern "C" {
	#include <lua5.1/lua.h>
	#include <lua5.1/lauxlib.h>
	#include <lua5.1/luaconf.h>
}

#include <string>
#include <thread/LuaThread.h>

namespace nersis {
class NModule {
	virtual ~NModule() {}; //"On Unload"
	
	NModule(lua_State *L) {}; //"On Load"
	
	virtual void load() {};
	virtual void update(double dt) {};
	virtual void draw() {};
	
	virtual void focus(bool windowFocus) {};
	virtual void keyPressed(std::string key, bool isRepeat);
	virtual void keyReleased(std::string key);
	virtual void mouseFocus(bool mouseFocus) {};
	virtual void mouseMoved(double x, double y, double dx, double dy) {};
	virtual void mousePressed(double x, double y, int button) {};
	virtual void mouseReleased(double x, double y, int button) {};
	virtual bool quit() {return false;}
	virtual void resize(double w, double h) {};
	virtual void textInput(std::string text) {};
	virtual void threadError(love::thread::LuaThread thread, std::string error) {};
	virtual void visible(bool visibility) {};
};
}
