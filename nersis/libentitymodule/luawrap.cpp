//implementation of a lua skeleton (this file is included in the middle of module.cpp)
/*
static void wrapEntity(lua_State *L, Entity *entity) {
	Entity** pointer = (Entity**)lua_newuserdata(L, sizeof(Entity**));
	*pointer = entity;

	luaL_getmetatable(L, "Nersis_Entity");
	lua_setmetatable(L, -2);
}

static Entity *unwrapEntity(lua_State *L, int id) {
	return *((Entity**)luaL_checkudata(L, id, "Nersis_Entity"));
}

static void wrapEntityContainer(lua_State *L, EntityContainer *container) {
	EntityContainer** pointer = (EntityContainer**)lua_newuserdata(L, sizeof(EntityContainer**));
	*pointer = container;

	luaL_getmetatable(L, "Nersis_EC");
	lua_setmetatable(L, -2);
}

static EntityContainer *unwrapEntityContainer(lua_State *L, int id) {
	return *((EntityContainer**)luaL_checkudata(L, id, "Nersis_EC"));
}
*/
class LuaEntityCallback : public EntityCallback {
	lua_State *L;
	int id;
	const char *name;
	
	public:
	LuaEntityCallback(lua_State *L, int id, const char *name) {
		this->id = id;
		this->L = L;
		this->name = name;
	}
	
	virtual void run(Entity *entity);
};

void LuaEntityCallback::run(Entity *entity) {
	lua_rawgeti(L, LUA_REGISTRYINDEX, id);
	lua_getfield(L, -1, name); 
	lua_remove(L, -2);
	if (lua_isfunction(L, -1)) {
		push(L, entity);
		if (lua_pcall(L, 1, 0, 0) != 0) {
			printf("Error in LuaState: %s\n",lua_tostring(L, -1));
			lua_pop(L, 1);
		}
	} else {
		lua_pop(L, 1);
	}
}

class LuaSkeleton : public Skeleton {
	lua_State *L;
	int ref;
	
	public:
	LuaSkeleton(lua_State *L, int index);
	virtual ~LuaSkeleton();
};

LuaSkeleton::LuaSkeleton(lua_State *L, int index) {
	this->L = L;
	ref = luaL_ref(L, LUA_REGISTRYINDEX);
	
	lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
	lua_getfield(L, -1, "name");
	name = lua_tostring(L, -1);
	lua_pop(L, 2);
	
	create = new LuaEntityCallback(L, ref, "create");
	update = new LuaEntityCallback(L, ref, "update");
	draw = new LuaEntityCallback(L, ref, "draw");
	destroy = new LuaEntityCallback(L, ref, "destroy");
}

LuaSkeleton::~LuaSkeleton() {
	luaL_unref(L, LUA_REGISTRYINDEX, ref);
}
/*
static int nersis_entity_getId(lua_State *L) {
	Entity *entity = unwrapEntity(L, 1);
	lua_pushinteger(L, entity->skeleton->id);
	return 1;
}

static int nersis_entity_getName(lua_State *L) {
	Entity *entity = unwrapEntity(L, 1);
	lua_pushstring(L, entity->skeleton->name.c_str());
	return 1;
}

static int nersis_entity_getBody(lua_State *L) {
	Entity *entity = unwrapEntity(L, 1);
	luax_pushtype(L, PHYSICS_BODY_ID, entity->body);
	return 1;
}

static int nersis_entity_setBody(lua_State *L) {
	Entity *entity = unwrapEntity(L, 1);
	entity->body = luax_checkbody(L, 2);
	return 0;
}

static int nersis_entity_getContainer(lua_State *L) {
	Entity *entity = unwrapEntity(L, 1);
	wrapEntityContainer(L, entity->container);
	return 1;
}
static const luaL_Reg nersis_entity[] = {
	{"getId", nersis_entity_getId},
	{"getName", nersis_entity_getName},
	{"getBody", nersis_entity_getBody},
	{"setBody", nersis_entity_setBody},
	{"getContainer", nersis_entity_getContainer},
	{NULL,NULL}
};

void createEntityMetatable(lua_State *L) {
	luaL_newmetatable(L, "Nersis_Entity");
	luaL_setfuncs(L, nersis_entity, 0);
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	lua_pop(L, 1);
}

static int nersis_entitycontainer_update(lua_State *L) {
	EntityContainer *container = unwrapEntityContainer(L, 1);
	container->update();
	return 0;
}

static int nersis_entitycontainer_draw(lua_State *L) {
	EntityContainer *container = unwrapEntityContainer(L, 1);
	container->draw();
	return 0;
}

static int nersis_entitycontainer_getWorld(lua_State *L) {
	EntityContainer *container = unwrapEntityContainer(L, 1);
	luax_pushtype(L, PHYSICS_WORLD_ID, container->world);
	return 1;
}

static const luaL_Reg nersis_entitycontainer[] = {
	{"update", nersis_entitycontainer_update},
	{"draw", nersis_entitycontainer_draw},
	{"getWorld", nersis_entitycontainer_getWorld},
	{NULL,NULL}
};

void createEntityContainerMetatable(lua_State *L) {
	int n = luaL_newmetatable(L, "Nersis_EC");
	luaL_setfuncs(L, nersis_entitycontainer, 0);
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	lua_pop(L, 1);
}*/
