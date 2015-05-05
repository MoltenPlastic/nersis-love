return {
	includes = {"../liblove","../liblove/modules","../liblove/libraries","../libnersiscore",
		"../luabridge"},
	buildoptions = {
		--[[android_armv7a = {
			
		},
		android_arm = {
			
		},
		android_aarch64 = {
			
		},
		android_x86 = {
			cc = (os.getenv("ANDROID_NDK") or "").."/toolchains/x86-4.9/prebuilt/*/linux-x86_64/gcc",
			flags = {"-m32","-fPIC","-O2","-std=c++11"},
			ending = ".so"
		},]]
		linux_x86 = {
			includes = {"/usr/include/SDL2","/usr/include/lua5.1"},
			cc = "gcc",
			flags = {"-m32","-fPIC","-O2","-std=c++11"},
			ending = ".so"
		},
		linux_x64 = {
			includes = {"/usr/include/SDL2","/usr/include/lua5.1"},
			cc = "gcc",
			flags = {"-fPIC","-O2","-std=c++11"},
			ending = ".so"
		},
		mac_x86 = {
			cc = "gcc",
			flags = {"-m32","-fPIC","-O2","-std=c++11"},
			ending = ".dylib"
		},
		mac_x64 = {
			cc = "gcc",
			flags = {"-fPIC","-O2","-std=c++11"},
			ending = ".dylib"
		},
		windows_x86 = {
			includes = {"../winderps_includes/SDL2","../winderps_includes/lua5.1"},
			cc = "i586-mingw32msvc-g++",
			flags = {"-O2","-std=c++11","-D_WIN32","-DBUILDING_DLL"},
			ending = ".dll"
		},
		windows_x64 = {
			includes = {"../winderps_includes/SDL2","../winderps_includes/lua5.1"},
			cc = "x86_64-w64-mingw32-g++",
			flags = {"-O2","-std=c++11","-D_WIN32","-DBUILDING_DLL","-lgcc"},
			ending = ".dll"
		}
	}
}
