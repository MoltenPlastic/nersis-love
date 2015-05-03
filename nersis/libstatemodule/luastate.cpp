//implementation of a lua state (this file is included in the middle of module.cpp)

class LuaState : public State {
	lua_State *L;
	int id;
	
	public:
	LuaState(lua_State *L, int index);
	virtual ~LuaState();
	
	virtual void enter() {
		lua_rawgeti(L, LUA_REGISTRYINDEX, id); /* table */
		lua_getfield(L, -1, "enter"); /* func table */
		lua_remove(L, -2); /* func */
		if (lua_isfunction(L, -1)) {
			if (lua_pcall(L, 0, 0, 0) != 0) { /* error */
				printf("Error in LuaState: %s\n",lua_tostring(L, -1));
				lua_pop(L, 1);
			}
		} else {
			lua_pop(L, 1);
		}
	};
	virtual void leave() {
		lua_rawgeti(L, LUA_REGISTRYINDEX, id); /* table */
		lua_getfield(L, -1, "leave"); /* func table */
		lua_remove(L, -2); /* func */
		if (lua_isfunction(L, -1)) {
			if (lua_pcall(L, 0, 0, 0) != 0) { /* error */
				printf("Error in LuaState: %s\n",lua_tostring(L, -1));
				lua_pop(L, 1);
			}
		} else {
			lua_pop(L, 1);
		}
		
		delete this;
	};
	virtual void suspend() {
		lua_rawgeti(L, LUA_REGISTRYINDEX, id); /* table */
		lua_getfield(L, -1, "suspend"); /* func table */
		lua_remove(L, -2); /* func */
		if (lua_isfunction(L, -1)) {
			if (lua_pcall(L, 0, 0, 0) != 0) { /* error */
				printf("Error in LuaState: %s\n",lua_tostring(L, -1));
				lua_pop(L, 1);
			}
		} else {
			lua_pop(L, 1);
		}
	};
	virtual void resume() {
		lua_rawgeti(L, LUA_REGISTRYINDEX, id); /* table */
		lua_getfield(L, -1, "resume"); /* func table */
		lua_remove(L, -2); /* func */
		if (lua_isfunction(L, -1)) {
			if (lua_pcall(L, 0, 0, 0) != 0) { /* error */
				printf("Error in LuaState: %s\n",lua_tostring(L, -1));
				lua_pop(L, 1);
			}
		} else {
			lua_pop(L, 1);
		}
	};
	
	virtual void update(double dt) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, id); /* table */
		lua_getfield(L, -1, "update"); /* func table */
		lua_remove(L, -2); /* func */
		if (lua_isfunction(L, -1)) {
			lua_pushnumber(L, dt);
			if (lua_pcall(L, 1, 0, 0) != 0) { /* error */
				printf("Error in LuaState: %s\n",lua_tostring(L, -1));
				lua_pop(L, 1);
			}
		} else {
			lua_pop(L, 1);
		}
	};
	virtual void draw() {
		lua_rawgeti(L, LUA_REGISTRYINDEX, id); /* table */
		lua_getfield(L, -1, "draw"); /* func table */
		lua_remove(L, -2); /* func */
		if (lua_isfunction(L, -1)) {
			if (lua_pcall(L, 0, 0, 0) != 0) { /* error */
				printf("Error in LuaState: %s\n",lua_tostring(L, -1));
				lua_pop(L, 1);
			}
		} else {
			lua_pop(L, 1);
		}
	};

	virtual void focus(bool windowFocus) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, id); /* table */
		lua_getfield(L, -1, "focus"); /* func table */
		lua_remove(L, -2); /* func */
		if (lua_isfunction(L, -1)) {
			lua_pushboolean(L, windowFocus);
			if (lua_pcall(L, 1, 0, 0) != 0) { /* error */
				printf("Error in LuaState: %s\n",lua_tostring(L, -1));
				lua_pop(L, 1);
			}
		} else {
			lua_pop(L, 1);
		}
	};
	virtual void keyPressed(love::keyboard::Keyboard::Key key, bool isRepeat) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, id); /* table */
		lua_getfield(L, -1, "keypressed"); /* func table */
		lua_remove(L, -2); /* func */
		if (lua_isfunction(L, -1)) {
			const char *keystring;
			love::keyboard::Keyboard::getConstant(key, keystring);
			lua_pushstring(L, keystring);
			lua_pushboolean(L, isRepeat);
			if (lua_pcall(L, 2, 0, 0) != 0) { /* error */
				printf("Error in LuaState: %s\n",lua_tostring(L, -1));
				lua_pop(L, 1);
			}
		} else {
			lua_pop(L, 1);
		}
	};
	virtual void keyReleased(love::keyboard::Keyboard::Key key) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, id); /* table */
		lua_getfield(L, -1, "keyreleased"); /* func table */
		lua_remove(L, -2); /* func */
		if (lua_isfunction(L, -1)) {
			const char *keystring;
			love::keyboard::Keyboard::getConstant(key, keystring);
			lua_pushstring(L, keystring);
			if (lua_pcall(L, 1, 0, 0) != 0) { /* error */
				printf("Error in LuaState: %s\n",lua_tostring(L, -1));
				lua_pop(L, 1);
			}
		} else {
			lua_pop(L, 1);
		}
	};
	virtual void mouseFocus(bool mouseFocus) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, id); /* table */
		lua_getfield(L, -1, "mousefocus"); /* func table */
		lua_remove(L, -2); /* func */
		if (lua_isfunction(L, -1)) {
			lua_pushboolean(L, mouseFocus);
			if (lua_pcall(L, 1, 0, 0) != 0) { /* error */
				printf("Error in LuaState: %s\n",lua_tostring(L, -1));
				lua_pop(L, 1);
			}
		} else {
			lua_pop(L, 1);
		}
	};
	virtual void mouseMoved(int x, int y, int dx, int dy) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, id); /* table */
		lua_getfield(L, -1, "mousemoved"); /* func table */
		lua_remove(L, -2); /* func */
		if (lua_isfunction(L, -1)) {
			lua_pushinteger(L, x);
			lua_pushinteger(L, y);
			lua_pushinteger(L, dx);
			lua_pushinteger(L, dy);
			if (lua_pcall(L, 4, 0, 0) != 0) { /* error */
				printf("Error in LuaState: %s\n",lua_tostring(L, -1));
				lua_pop(L, 1);
			}
		} else {
			lua_pop(L, 1);
		}
	};
	virtual void mousePressed(int x, int y, int button) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, id); /* table */
		lua_getfield(L, -1, "mousepressed"); /* func table */
		lua_remove(L, -2); /* func */
		if (lua_isfunction(L, -1)) {
			lua_pushinteger(L, x);
			lua_pushinteger(L, y);
			lua_pushinteger(L, button);
			if (lua_pcall(L, 3, 0, 0) != 0) { /* error */
				printf("Error in LuaState: %s\n",lua_tostring(L, -1));
				lua_pop(L, 1);
			}
		} else {
			lua_pop(L, 1);
		}
	};
	virtual void mouseReleased(int x, int y, int button) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, id); /* table */
		lua_getfield(L, -1, "mousereleased"); /* func table */
		lua_remove(L, -2); /* func */
		if (lua_isfunction(L, -1)) {
			lua_pushinteger(L, x);
			lua_pushinteger(L, y);
			lua_pushinteger(L, button);
			if (lua_pcall(L, 3, 0, 0) != 0) { /* error */
				printf("Error in LuaState: %s\n",lua_tostring(L, -1));
				lua_pop(L, 1);
			}
		} else {
			lua_pop(L, 1);
		}
	};
	virtual bool quit() {
		lua_rawgeti(L, LUA_REGISTRYINDEX, id); /* table */
		lua_getfield(L, -1, "quit"); /* func table */
		lua_remove(L, -2); /* func */
		if (lua_isfunction(L, -1)) {
			if (lua_pcall(L, 0, 1, 0) != 0) { /* error */
				printf("Error in LuaState: %s\n",lua_tostring(L, -1));
				lua_pop(L, 1);
			}
			bool q = lua_toboolean(L, -1);
			lua_pop(L, 1);
			return q;
		} else {
			lua_pop(L, 1);
		}
		return false;
	};
	virtual void resize(int w, int h) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, id); /* table */
		lua_getfield(L, -1, "resize"); /* func table */
		lua_remove(L, -2); /* func */
		if (lua_isfunction(L, -1)) {
			lua_pushinteger(L, w);
			lua_pushinteger(L, h);
			if (lua_pcall(L, 2, 0, 0) != 0) { /* error */
				printf("Error in LuaState: %s\n",lua_tostring(L, 1));
				lua_pop(L, 1);
			}
		} else {
			lua_pop(L, 1);
		}
	};
	virtual void textInput(std::string text) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, id); /* table */
		lua_getfield(L, -1, "textinput"); /* func table */
		lua_remove(L, -2); /* func */
		if (lua_isfunction(L, -1)) {
			lua_pushstring(L, text.c_str());
			if (lua_pcall(L, 1, 0, 0) != 0) { /* error */
				printf("Error in LuaState: %s\n",lua_tostring(L, 1));
				lua_pop(L, 1);
			}
		} else {
			lua_pop(L, 1);
		}
	};
	virtual void threadError(love::thread::LuaThread *thread, std::string error) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, id); /* table */
		lua_getfield(L, -1, "threaderror"); /* func table */
		lua_remove(L, -2); /* func */
		if (lua_isfunction(L, -1)) {
			luax_pushtype(L, THREAD_THREAD_ID, thread);
			lua_pushstring(L, error.c_str());
			if (lua_pcall(L, 2, 0, 0) != 0) { /* error */
				printf("Error in LuaState: %s\n",lua_tostring(L, 1));
				lua_pop(L, 1);
			}
		} else {
			lua_pop(L, 1);
		}
	};
	virtual void visible(bool visibility) {
		lua_rawgeti(L, LUA_REGISTRYINDEX, id); /* table */
		lua_getfield(L, -1, "visible"); /* func table */
		lua_remove(L, -2); /* func */
		if (lua_isfunction(L, -1)) {
			lua_pushboolean(L, visibility);
			if (lua_pcall(L, 1, 0, 0) != 0) { /* error */
				printf("Error in LuaState: %s\n",lua_tostring(L, 1));
				lua_pop(L, 1);
			}
		} else {
			lua_pop(L, 1);
		}
	};

	virtual void sdlEvent(SDL_Event e) {}; //TODO: SDL_Event wrapper maybe?
};

LuaState::LuaState(lua_State *L, int index) {
	this->L = L;
	id = luaL_ref(L, LUA_REGISTRYINDEX);
}

LuaState::~LuaState() {
	luaL_unref(L, LUA_REGISTRYINDEX, id);
}
