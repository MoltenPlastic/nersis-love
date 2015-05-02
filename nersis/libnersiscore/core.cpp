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

static int nersis_core_hello(lua_State *L) {
	printf("Hello from libnersiscore!\n");
	Window::singleton->setWindow(600,200);
	opengl::Graphics *graphics = Module::getInstance<opengl::Graphics>(Module::M_GRAPHICS);
	graphics->setBackgroundColor(Color(255,255,255,255));
	return 0;
}

static int nersis_core_love_load(lua_State *L) {
	
}

static const luaL_Reg nersis_corelib[] = {
	{"hello", nersis_core_hello},
	{"love_load", nersis_core_love_load},
	{NULL, NULL}
};

LUALIB_API int luaopen_nersis_core(lua_State *L) {
	luaL_register(L, "nersis.core", nersis_corelib);
	return 1;
}
