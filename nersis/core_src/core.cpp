#define LUA_LIB
extern "C" {
	#include <lua5.1/lua.h>
	#include <lua5.1/lauxlib.h>
	#include <lua5.1/luaconf.h>
}
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

static int nersis_core_hello(lua_State *L) {
	printf("Hello from Nersis Core!\n");
	Window::singleton->setWindow(600,200);
	opengl::Graphics *graphics = Module::getInstance<opengl::Graphics>(Module::M_GRAPHICS);
	graphics->setBackgroundColor(Color(255,255,255,255));
	return 0;
}

static const luaL_Reg nersis_corelib[] = {
	{"hello", nersis_core_hello},
	{NULL, NULL}
};

LUALIB_API int luaopen_nersis_core(lua_State *L) {
	luaL_register(L, "nersis.core", nersis_corelib);
	return 1;
}
