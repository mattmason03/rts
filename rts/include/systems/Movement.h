#pragma once
//
//class MovementSystem {
//public:
//	ecs::EntityManager* manager;
//	MovementSystem(ecs::EntityManager* manager) : manager{ manager } {};
//
//	void Update() {
//		manager->ForAll([this](ecs::Entity::Id* id, Transform* t, Destination* d) {
//			auto orientation = t->dir * glm::dvec3(1., 0., 0.);
//			auto angle = glm::angle(glm::normalize(orientation), glm::normalize(d->pos - t->pos));
//
//			if (angle > 0.001) {
//				if (angle > .1)
//					angle = .1;
//				t->dir *= glm::angleAxis(angle, glm::dvec3(0., 0., 1.));
//			}
//			else {
//				if (glm::distance(t->pos, d->pos) < .05) {
//					t->pos = d->pos;
//					manager->Remove<Destination>(*id);
//				}
//				else {
//					t->pos += .05 * orientation;
//				}
//			}
//		});
//	}
//
//};