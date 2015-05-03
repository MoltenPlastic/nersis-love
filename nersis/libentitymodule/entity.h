#pragma once

#include "core.h"
#include "physics/box2d/Physics.h"
#include "graphics/opengl/Graphics.h"
#include "common/Module.h"
#include <vector>

using namespace love;
using namespace love::physics::box2d;
using namespace love::graphics::opengl;

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
			
			// these are for interpolation
			float renderX;
			float renderY;
			float renderAngle;
			
			Entity(EntityContainer *c, Skeleton *s);
			
			~Entity();
			
			void destroy() {
				
			}
		};
		
		class EntityContainer {
			public:
			World *world;
			std::vector<Entity*> entities;
			int simulationSteps = 0; // track simulation steps
			int renderSteps = 0; // track rendered steps
			
			EntityContainer();
			~EntityContainer();
			
			void update() {
				//update world at constant rate
				world->update(1.0/30.0);
				for (auto entity : entities) {
					if (entity->skeleton->update)
						entity->skeleton->update->run(entity);
				}
				simulationSteps++;
			}
			
			void draw() {
				Graphics *graphics = Module::getInstance<Graphics>(Module::M_GRAPHICS);
				for (auto entity : entities) {
					if (entity->skeleton->draw) {
						graphics->push();
						graphics->translate((entity->renderX+entity->body->getX())/2.0,(entity->renderY+entity->body->getY())/2.0);
						graphics->rotate((entity->renderAngle+entity->body->getAngle())/2.0);
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
		};
		
		NERSIS_API int registerSkeleton(Skeleton *skeleton);
		NERSIS_API Entity *createEntityFromSkeleton(EntityContainer *container, int id);
		NERSIS_API int findSkeletonByName(std::string name);
	}
}
