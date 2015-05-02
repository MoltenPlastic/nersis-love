#include "core.h"
#include <map>

#include "window/Window.h"
#include "graphics/opengl/Graphics.h"
#include "graphics/Color.h"
#include "common/Module.h"

#if LUA_VERSION_NUM > 501
#error Not the right version of Lua!
#endif

using namespace love;
using namespace love::window;
using namespace love::graphics;

namespace nersis {
	std::vector<NModule*> modules;
	std::map<std::string, NModule*> moduleMap;
	
	void registerModule(NModule *module) {
		modules.push_back(module);
	}
	
	NModule *findModule(std::string name) {
		return moduleMap[name];
	}
	
	std::vector<NModule*> moduleList() {
		return modules;
	}
}

//Use this as module sample
class FuckCPPModule : public nersis::NModule {
	public:
	FuckCPPModule() {
		name = "FUCKCPPMODULE";
		printf("Hello from modload!\n");
	}
};

static int nersis_core_hello(lua_State *L) {
	printf("Hello from libnersiscore!\n");
	Window::singleton->setWindow(600,200);
	opengl::Graphics *graphics = Module::getInstance<opengl::Graphics>(Module::M_GRAPHICS);
	graphics->setBackgroundColor(Color(255,255,255,255));
	
	nersis::registerModule(new FuckCPPModule());
	return 0;
}

static int nersis_core_love_load(lua_State *L) {
	for (auto module : nersis::moduleList()) {
		module->load();
	}
	return 0;
}

static int nersis_core_love_update(lua_State *L) {
	double dt = lua_tonumber(L, 1);
	for (auto module : nersis::moduleList()) {
		module->update(dt);
	}
	return 0;
}

static int nersis_core_love_draw(lua_State *L) {
	for (auto module : nersis::moduleList()) {
		module->draw();
	}
	return 0;
}

static int nersis_core_love_focus(lua_State *L) {
	bool focus = lua_toboolean(L, 1);
	for (auto module : nersis::moduleList()) {
		module->focus(focus);
	}
	return 0;
}

static int nersis_core_love_keypressed(lua_State *L) {
	love::keyboard::Keyboard::Key key;
	love::keyboard::Keyboard::getConstant(lua_tostring(L, 1), key);
	bool isRepeat = lua_toboolean(L, 2);
	for (auto module : nersis::moduleList()) {
		module->keyPressed(key, isRepeat);
	}
	return 0;
}

static int nersis_core_love_keyreleased(lua_State *L) {
	love::keyboard::Keyboard::Key key;
	love::keyboard::Keyboard::getConstant(lua_tostring(L, 1), key);
	for (auto module : nersis::moduleList()) {
		module->keyReleased(key);
	}
	return 0;
}

static int nersis_core_love_mousefocus(lua_State *L) {
	bool focus = lua_toboolean(L, 1);
	for (auto module : nersis::moduleList()) {
		module->mouseFocus(focus);
	}
	return 0;
}

static const luaL_Reg nersis_corelib[] = {
	{"hello", nersis_core_hello},
	{"love_load", nersis_core_love_load},
	{"love_update", nersis_core_love_update},
	{"love_draw", nersis_core_love_draw},
	{"love_focus", nersis_core_love_focus},
	{"love_keypressed", nersis_core_love_keypressed},
	{"love_keyreleased", nersis_core_love_keyreleased},
	{"love_mousefocus", nersis_core_love_mousefocus},
	{NULL, NULL}
};

LUALIB_API int luaopen_nersis_core(lua_State *L) {
	luaL_register(L, "nersis.core", nersis_corelib);
	return 1;
}
