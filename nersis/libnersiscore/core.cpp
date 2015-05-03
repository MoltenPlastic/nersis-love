#include "core.h"
#include <map>

#include "window/Window.h"
#include "graphics/opengl/Graphics.h"
#include "graphics/Color.h"
#include "math/MathModule.h"
#include "event/sdl/Event.h"
#include "timer/Timer.h"
#include "common/Module.h"

#if LUA_VERSION_NUM > 501
#error Not the right version of Lua!
#endif

using namespace love;
using namespace love::window;
using namespace love::graphics;
using namespace love::math;
using namespace love::event::sdl;
using namespace love::timer;
using namespace love::keyboard;

namespace nersis {
	std::vector<NModule*> modules;
	std::map<std::string, NModule*> moduleMap;
	
	void registerModule(NModule *module) {
		modules.push_back(module);
	}
	
	NModule *getModule(std::string name) {
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
		printf("Hello from core dummy module!\n");
	}
};

static int nersis_core_hello(lua_State *L) {
	printf("Hello from libnersiscore!\n");
	/*Window::singleton->setWindow(600,200);
	opengl::Graphics *graphics = Module::getInstance<opengl::Graphics>(Module::M_GRAPHICS);
	graphics->setBackgroundColor(Color(255,255,255,255));*/
	
	nersis::registerModule(new FuckCPPModule());
	return 0;
}

static int nersis_core_love_run(lua_State *L) {
	Math *math = Module::getInstance<Math>(Module::M_MATH);
	Timer *timer = Module::getInstance<Timer>(Module::M_TIMER);
	Window *window = Module::getInstance<Window>(Module::M_WINDOW);
	opengl::Graphics *graphics = Module::getInstance<opengl::Graphics>(Module::M_GRAPHICS);
	Keyboard *keyboard = Module::getInstance<Keyboard>(Module::M_KEYBOARD);
	
	RandomGenerator::Seed seed; //will be random
	math->setRandomSeed(seed);
	
	//we manually keep track of SDL Events for the sake of Event module conversions that we don't want
	//we can still use part of the event modules to push said conversions to lua

	for (auto module : nersis::moduleList()) {
		module->load();
	}
	
	timer->step();
	
	double dt = 0;
	
	//main loop
	while (true) {
		//process events
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			for (auto module : nersis::moduleList()) {
				module->sdlEvent(e);
			}
			
			switch (e.type)
			{
			case SDL_QUIT:
			case SDL_APP_TERMINATING: {
				bool canQuit = true;
				for (auto module : nersis::moduleList()) {
					bool q = module->quit();
					if (q) {
						printf("module %s requested not to quit! %d\n", module->name.c_str(), q);
						canQuit = false;
						break;
					}
				}
				if (canQuit)
					return 0;
				break;
			}
			case SDL_KEYDOWN: {
				love::keyboard::Keyboard::Key key;
				std::map<SDL_Keycode, love::keyboard::Keyboard::Key>::const_iterator keyit;
				if (e.key.repeat)
				{
					if (!keyboard->hasKeyRepeat())
						break;
				}

				keyit = Event::keys.find(e.key.keysym.sym);
				if (keyit != Event::keys.end())
					key = keyit->second;
				
				for (auto module : nersis::moduleList()) {
					module->keyPressed(key, e.key.repeat);
				}
				break;
			}
			case SDL_KEYUP: {
				love::keyboard::Keyboard::Key key;
				std::map<SDL_Keycode, love::keyboard::Keyboard::Key>::const_iterator keyit;

				keyit = Event::keys.find(e.key.keysym.sym);
				if (keyit != Event::keys.end())
					key = keyit->second;
				
				for (auto module : nersis::moduleList()) {
					module->keyReleased(key);
				}
				break;
			}
			case SDL_TEXTINPUT:
				for (auto module : nersis::moduleList()) {
					module->textInput(e.text.text);
				}
				break;
			case SDL_MOUSEMOTION:
				for (auto module : nersis::moduleList()) {
					module->mouseMoved(e.motion.x,e.motion.y,e.motion.xrel,e.motion.yrel);
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				for (auto module : nersis::moduleList()) {
					module->mousePressed(e.button.x,e.button.y,e.button.button);
				}
				break;
			case SDL_MOUSEBUTTONUP:
				for (auto module : nersis::moduleList()) {
					module->mouseReleased(e.button.x,e.button.y,e.button.button);
				}
				break;
			}
		}
	
		timer->step();
		dt = timer->getDelta();
	
		for (auto module : nersis::moduleList()) {
			module->update(dt);
		}
		
		if (window->isCreated()) {
			graphics->clear(graphics->getBackgroundColor());
			graphics->origin();
			for (auto module : nersis::moduleList()) {
				module->draw();
			}
			graphics->present();
		}
		
		timer->sleep(0.001);
	}
	
	return 0;
}

static const luaL_Reg nersis_corelib[] = {
	{"hello", nersis_core_hello},
	{"love_run", nersis_core_love_run},
	{NULL, NULL}
};

static int nersis_core_panic(lua_State *L) {
	printf("%s\n",lua_tostring(L,1));
	return 0;
}

LUALIB_API int luaopen_nersis_core(lua_State *L) {
	lua_atpanic(L, nersis_core_panic);
	luaL_register(L, "nersis.core", nersis_corelib);
	return 1;
}
