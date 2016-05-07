#pragma once

//
//class Selector {
//public:
//	struct Selected {
//		bool selected = true;
//	};
//
//	ecs::EntityManager* manager;
//	std::vector<ecs::Entity::Id*> selectedEntities;
//
//	Selector(ecs::EntityManager* manager) : manager{ manager } {};
//
//	void Register() {
//		Input::leftClicks.subscribe([this](MouseInfo& info) {
//			HandleClick(info);
//		});
//		Input::rightClicks.subscribe([this](MouseInfo& info) {
//			for (auto& id : selectedEntities) {
//				auto pos = TranslatePos(glm::dvec2(info.position));
//				manager->Add<Destination>(*id, Destination{ glm::dvec3(pos, 0.) });
//			}
//		});
//	}
//
//	glm::dvec2 TranslatePos(glm::dvec2 screenPos) {
//		glfwGetCursorPos(window, &screenPos.x, &screenPos.y);
//		screenPos /= 640;
//		screenPos.x -= 1;
//		screenPos.y -= 1;
//		screenPos.y *= -1;
//		return screenPos;
//	}
//
//	void HandleClick(MouseInfo& info) {
//		FindEntity(TranslatePos(info.position));
//	}
//
//	void FindEntity(glm::dvec2 screenPos) {
//		for (auto &id : selectedEntities) {
//			manager->Remove<Selected>(*id);
//		}
//		selectedEntities.clear();
//		manager->ForAll([this, &screenPos](ecs::Entity::Id *id, Transform *t) {
//			auto delta = -screenPos + glm::dvec2(t->pos);
//			auto distSquared = glm::dot(delta, delta);
//			if (distSquared < .1f) {
//				manager->Add<Selected>(*id);
//				selectedEntities.push_back(id);
//			}
//		});
//	}
//};
