#pragma once

#include "core.h"
#include <LuaBridge.h>
#include "physics/box2d/Physics.h"
#include "graphics/opengl/Graphics.h"
#include "physics/box2d/wrap_Body.h"
#include "common/Module.h"
#include <vector>
#include <map>

using namespace love;
using namespace love::physics::box2d;
using namespace love::graphics::opengl;
using namespace luabridge;

namespace nersis {
	namespace entity {
		class EntityContainer;
		class Entity;
		
		class EntityCallback {
			public:
			virtual void run(Entity *entity) = 0;
		};
		
		class Skeleton {
			public:
			std::string name;
			int id;
			
			EntityCallback *create = NULL;
			EntityCallback *update = NULL;
			EntityCallback *draw = NULL;
			EntityCallback *destroy = NULL;
		};
		
		class Entity {
			public:
			EntityContainer *container;
			Body *body;
			Skeleton *skeleton;
			LuaRef data;
			
			// these are for interpolation
			float renderX;
			float renderY;
			float renderAngle;
			
			Entity(EntityContainer *c, Skeleton *s, float x, float y);
			~Entity();
			
			void destroy() {
				
			}
			
			int getId() {
				return skeleton->id;
			}
			
			std::string getName() {
				return skeleton->name;
			}
			
			int getLBody(lua_State *L) {
				luax_pushtype(L, "Body", PHYSICS_BODY_T, body);
				return 1;
			}
			
			int setLBody(lua_State *L) {
				body = luax_checkbody(L, 1);
				return 0;
			}
			
			int getLData(lua_State *L) {
				data.push(L);
				return 1;
			}
			
			EntityContainer *getContainer() {
				return container;
			}
		};
		
		class EntityContainer {
			public:
			lua_State *L;
			World *world;
			std::vector<Entity*> entities;
			int simulationSteps = 0; // track simulation steps
			int renderSteps = 0; // track rendered steps
			float timeSinceLastSimulationTick = 0;
			
			EntityContainer(lua_State *L);
			~EntityContainer();
			
			void update(float dt) {
				//update world at constant rate
				timeSinceLastSimulationTick += dt;
				
				if (timeSinceLastSimulationTick >= 1.0/30.0) {
					timeSinceLastSimulationTick -= 1.0/30.0;
					world->update(1.0/30.0);
					for (auto entity : entities) {
						if (entity->skeleton->update)
							entity->skeleton->update->run(entity);
					}
					simulationSteps++;
				}
			}
			
			void draw() {
				Graphics *graphics = Module::getInstance<Graphics>(Module::M_GRAPHICS);
				for (auto entity : entities) {
					if (entity->skeleton->draw) {
						graphics->push();
						graphics->translate((entity->renderX+entity->body->getX())/2.0,(entity->renderY+entity->body->getY())/2.0);
						graphics->rotate((entity->renderAngle+entity->body->getAngle())/2.0);
						//graphics->translate(entity->body->getX(),entity->body->getY());
						//graphics->rotate(entity->body->getAngle());
						entity->skeleton->draw->run(entity);
						graphics->pop();
					
						entity->body->getPosition(entity->renderX, entity->renderY);
						entity->renderAngle = entity->body->getAngle();
					}
				}
				renderSteps++;
			}
			
			void addEntity(Entity *e) {
				entities.push_back(e);
			}
			
			int getLWorld(lua_State *L) {
				luax_pushtype(L, "World", PHYSICS_WORLD_T, world);
				return 1;
			}
		};
		
		NERSIS_API int registerSkeleton(Skeleton *skeleton);
		NERSIS_API Entity *createEntityFromSkeleton(EntityContainer *container, int id, float x, float y);
		NERSIS_API int findSkeletonByName(std::string name);
	}
}
