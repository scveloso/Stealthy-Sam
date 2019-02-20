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
	Entity entity;
	entity.id = 0;
		
	tr1 = transformComponent.getTransform(entity);

		for (auto& it2 : collisionComponent.getmap())
		{
			if (it2.first.id != 0) {

				tr2 = transformComponent.getTransform(it2.first);
				AABB(entity, tr1, it2.first, tr2);
			}
		}
}

void CollisionSystem::AABB(Entity e1, Transform *tr1, Entity e2, Transform *tr2) {
	
	vec2 size1 = { tr1->width*tr1->m_scale.x, tr1->height*tr1->m_scale.y };
	vec2 size2 = { tr2->width*tr2->m_scale.x, tr2->height*tr2->m_scale.y };

	vec2 topLeft1 = { tr1->m_position.x - size1.x, tr1->m_position.y - size1.y};
	vec2 topLeft2 = { tr2->m_position.x - size2.x, tr2->m_position.y - size2.y};

	vec2 botRight1 = { tr1->m_position.x + size1.x*0.5, tr1->m_position.y + size1.y*0.5 };
	vec2 botRight2 = { tr2->m_position.x + size2.x*0.5, tr2->m_position.y + size2.y*0.5 };

	

	//printf("CHECKING COLLISION");

	bool collisionX = (topLeft1.x + size1.x) >= (topLeft2.x + size2.x) && (topLeft2.x + size2.x >= topLeft1.x);
	bool collisionY = (topLeft1.y + size1.y) >= (topLeft2.y + size2.y) && (topLeft2.y + size2.y >= topLeft1.y);

	if (collisionX && collisionY) {
		printf("COLLISION  \n");
	}
}