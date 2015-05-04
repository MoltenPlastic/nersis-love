#include "entity.h"
#include <LuaBridge.h>
#include "common/Module.h"
#include <vector>
#include <iostream>
#include <map>

using namespace love;
using namespace love::graphics::opengl;
using namespace love::physics::box2d;
using namespace nersis;
using namespace luabridge;

namespace nersis {
	namespace entity {
		Entity::Entity(EntityContainer *c, Skeleton *s) : data(LuaRef::newTable(c->L)) {
			container = c;
			Physics *physics = Module::getInstance<Physics>(Module::M_PHYSICS);
			body = physics->newBody(container->world, 0, 0, Body::BODY_DYNAMIC);
			container->addEntity(this);
			skeleton = s;
			if (skeleton->create)
				skeleton->create->run(this);
			
			body->getPosition(renderX, renderY);
			renderAngle = body->getAngle();
		}
		Entity::~Entity() {
			body->destroy();
		}
		
		EntityContainer::EntityContainer(lua_State *L) {
			this->L = L;
			Physics *physics = Module::getInstance<Physics>(Module::M_PHYSICS);
			world = physics->newWorld(0, 0, true);
		}
		
		EntityContainer::~EntityContainer() {
			//TODO: Something.
		}
		
		static std::vector<Skeleton*> skeletons;
		static std::map<std::string,int> skeletonNameToID;
		
		int registerSkeleton(Skeleton *skeleton) {
			skeletons.push_back(skeleton);
			int id = skeletons.size()-1;
			skeleton->id = id;
			skeletonNameToID[skeleton->name] = id;
			return id;
		}
		
		Entity *createEntityFromSkeleton(EntityContainer *container, int id) {
			Skeleton *skel = skeletons[id];
			return new Entity(container, skel);
		}
		
		int findSkeletonByName(std::string name) {
			return skeletonNameToID[name]; //TODO: Return -1 if not exist
		}
		
		class EntityModule : public NModule {
			public:
			EntityModule() {
				name = "entitymodule";
				printf("Hello from libentitymodule!\n");
			}
	
			virtual ~EntityModule() {};
		};
		
		#include "luawrap.cpp"
	}
}

static int nersis_entity_registerSkeleton(lua_State *L) {
	lua_pushinteger(L, nersis::entity::registerSkeleton(new nersis::entity::LuaSkeleton(L, 1)));
	return 1;
}

static int nersis_entity_newEntityContainer(lua_State *L) {
	nersis::entity::EntityContainer *ec = new nersis::entity::EntityContainer(L);
	push(L, ec);
	return 1;
}

/*
static int nersis_entity_createEntityFromSkeleton(lua_State *L) {
	nersis::entity::EntityContainer *ec = nersis::entity::unwrapEntityContainer(L, 1);
	int e = luaL_checkinteger(L, 2);
	nersis::entity::Entity *entity = nersis::entity::createEntityFromSkeleton(ec, e);
	nersis::entity::wrapEntity(L, entity);
	return 1;
}

static int nersis_entity_findSkeletonByName(lua_State *L) {
	lua_pushinteger(L, nersis::entity::findSkeletonByName(luaL_checkstring(L, 1)));
	return 1;
}

static const luaL_Reg nersis_entitylib[] = {
	{"registerSkeleton", nersis_entity_registerSkeleton},
	{"createEntityFromSkeleton", nersis_entity_createEntityFromSkeleton},
	{"findSkeletonByName", nersis_entity_findSkeletonByName},
	{"newEntityContainer", nersis_entity_newEntityContainer},
	{NULL, NULL}
};*/

LUALIB_API int luaopen_nersis_entitymodule(lua_State *L) {
	registerModule(new nersis::entity::EntityModule());
	
	getGlobalNamespace(L)
		.beginNamespace("nersis")
			.beginNamespace("entity")
				.addCFunction("registerSkeleton", nersis_entity_registerSkeleton)
				.addFunction("findSkeletonByName", nersis::entity::findSkeletonByName)
				.addFunction("createEntityFromSkeleton", nersis::entity::createEntityFromSkeleton)
				.beginClass<nersis::entity::Entity>("Entity")
					.addFunction("getId", &nersis::entity::Entity::getId)
					.addFunction("getName", &nersis::entity::Entity::getName)
					.addCFunction("getBody", &nersis::entity::Entity::getLBody)
					.addCFunction("setBody", &nersis::entity::Entity::setLBody)
					.addCFunction("getData", &nersis::entity::Entity::getLData)
					.addFunction("getContainer", &nersis::entity::Entity::getContainer)
				.endClass()
				.beginClass<nersis::entity::EntityContainer>("EntityContainer")
					.addStaticCFunction("__call", nersis_entity_newEntityContainer)
					.addFunction("update", &nersis::entity::EntityContainer::update)
					.addFunction("draw", &nersis::entity::EntityContainer::draw)
					.addFunction("getWorld", &nersis::entity::EntityContainer::getLWorld)
				.endClass()
			.endNamespace()
		.endNamespace();
	//nersis::entity::createEntityContainerMetatable(L);
	//nersis::entity::createEntityMetatable(L);
	//luaL_register(L, "nersis.entity", nersis_entitylib);
	return 0;
}
