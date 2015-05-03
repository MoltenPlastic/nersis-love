#include <vector>
#include "physics/vector.hpp"

namespace physics {
	typedef PhysNum int64_t;
	typedef void (*)(object,object) collisionCallback;

	class collisionHandle {
		collisionCallback callback;
		PhysNum softness;
	}
	
	class object {
		collisionHandle collision;
		group* parent;
		PhysNum mass;
		std::vector<Vec> poly;
		Vec pos;
		void split
	};
	
	class group {
		state* parent;
		std::vector<object> objects;
		Vec pos;
		Vec vel;
	};

	class state {
		std::vector<group> groups;
		PhysNum velDampening;
		PhysNum angDampening;
		void run(int dt);
	};
}