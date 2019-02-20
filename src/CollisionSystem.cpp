#include "CollisionSystem.hpp"
#include "Components/Component.hpp"
#include "common.hpp"

CollisionSystem::CollisionSystem(ObjectManager om, CollisionCmp cc, TransformCmp tc)
{
	collisionComponent = cc;
	transformComponent = tc;
}

void CollisionSystem::update(float elapsed_ms)
{
	Transform *tr1;
	Transform *tr2;

	//printf("updating");

	for (auto& it1 : collisionComponent.getmap()) {
		
		tr1 = transformComponent.getTransform(it1.first);

		for (auto& it2 : collisionComponent.getmap())
		{
			if (it1.first.id != it2.first.id) {

				tr2 = transformComponent.getTransform(it2.first);
				AABB(it1.first, tr1, it2.first, tr2);
			}
		}
	}
}

void CollisionSystem::AABB(Entity e1, Transform *tr1, Entity e2, Transform *tr2) {
	
	vec2 size1 = { tr1->width*tr1->m_scale.x, tr1->height*tr1->m_scale.y };
	vec2 size2 = { tr2->width*tr2->m_scale.x, tr2->height*tr2->m_scale.y };

	//printf("CHECKING COLLISION");

	bool collisionX = (tr1->m_position.x ) - (tr1->width*tr1->m_scale.x) >= (tr2->m_position.x );

	if (collisionX) {
		printf("COLLISIONX \n");
	}
}