#include "entity.h"
#include "physics/box2d/wrap_Body.h"
#include "common/Module.h"
#include <vector>
#include <map>

using namespace love;
using namespace love::graphics::opengl;
using namespace love::physics::box2d;
using namespace nersis;

namespace nersis {
	namespace entity {
		Entity::Entity(EntityContainer *c, Skeleton *s) {
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
		
		EntityContainer::EntityContainer() {
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

static int nersis_entity_newEntityContainer(lua_State *L) {
	nersis::entity::EntityContainer *ec = new nersis::entity::EntityContainer();
	nersis::entity::wrapEntityContainer(L, ec);
	return 1;
}

static const luaL_Reg nersis_entitylib[] = {
	{"registerSkeleton", nersis_entity_registerSkeleton},
	{"createEntityFromSkeleton", nersis_entity_createEntityFromSkeleton},
	{"findSkeletonByName", nersis_entity_findSkeletonByName},
	{"newEntityContainer", nersis_entity_newEntityContainer},
	{NULL, NULL}
};

LUALIB_API int luaopen_nersis_entitymodule(lua_State *L) {
	registerModule(new nersis::entity::EntityModule());
	nersis::entity::createEntityContainerMetatable(L);
	nersis::entity::createEntityMetatable(L);
	luaL_register(L, "nersis.entity", nersis_entitylib);
	return 1;
}
