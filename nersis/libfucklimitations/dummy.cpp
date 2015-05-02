#include "core.h"

#if defined(_WIN32)

#include <windows.h>

static void openGlobal(const char *lib) {
	LoadLibraryA(lib);
}

#elif defined(__APPLE__)

#include <mach-o/dyld.h>

static void openGlobal(const char *lib) {
	NSObjectFileImage img;
	NSObjectFileImageReturnCode ret;
	/* this would be a rare case, but prevents crashing if it happens */
	if(!_dyld_present()) {
		return;
	}
	ret = NSCreateObjectFileImageFromFile(path, &img);
	if (ret == NSObjectFileImageSuccess) {
		NSModule mod = NSLinkModule(img, path, NSLINKMODULE_OPTION_RETURN_ON_ERROR);
		NSDestroyObjectFileImage(img);
	}
}

#else

#include <dlfcn.h>

static void openGlobal(const char *lib) {
	dlopen(lib,RTLD_GLOBAL|RTLD_NOW);
}

#endif

static int nersis_fl_dlopen(lua_State *L) {
	const char *file = luaL_checkstring(L, 1);
	openGlobal(file);
	return 0;
}

LUALIB_API int luaopen_nersis_fucklimitations(lua_State *L) {
	lua_pushcfunction(L, nersis_fl_dlopen);
	return 1;
}
