#include "core.h"
#include <dlfcn.h>

LUALIB_API int luaopen_nersis_fucklimitations(lua_State *L) {
	dlopen("./nersis/core.so",RTLD_GLOBAL|RTLD_NOW);
	return 0;
}
