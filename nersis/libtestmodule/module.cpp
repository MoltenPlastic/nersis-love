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
	}
	
	virtual ~TestModule() {};
	
	virtual void draw();
	virtual void update(double dt);
	
	virtual void keyPressed(love::keyboard::Keyboard::Key key, bool repeat);
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

void TestModule::keyPressed(love::keyboard::Keyboard::Key key, bool repeat) {
	printf("Key pressed!\n");
}

LUALIB_API int luaopen_nersis_testmodule(lua_State *L) {
	registerModule(new TestModule());
	return 0;
}
