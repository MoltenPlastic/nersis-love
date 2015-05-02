#include "core.h"
#include "window/Window.h"
#include "graphics/opengl/Graphics.h"
#include "graphics/Color.h"
#include "common/Module.h"

using namespace love;
using namespace love::window;
using namespace love::graphics;
using namespace nersis;

class TestModule : public NModule {
	double angle = 0;
	public:
	TestModule() {
		name = "testmodule";
		printf("Hello from libtestmodule!\n");
		opengl::Graphics *graphics = Module::getInstance<opengl::Graphics>(Module::M_GRAPHICS);
		graphics->setBackgroundColor(Color(255,0,0,255));
	}
	
	virtual ~TestModule() {};
	
	virtual void draw();
	virtual void update(double dt);
};

void TestModule::update(double dt) {
	angle += dt;
}

void TestModule::draw() {
	opengl::Graphics *graphics = Module::getInstance<opengl::Graphics>(Module::M_GRAPHICS);
	graphics->push();
	graphics->rotate(angle);
	graphics->rectangle(Graphics::DRAW_FILL, 0, 0, 64, 64);
	graphics->pop();
}

LUALIB_API int luaopen_nersis_testmodule(lua_State *L) {
	registerModule(new TestModule());
	return 0;
}
