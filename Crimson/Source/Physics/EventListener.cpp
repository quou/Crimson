#include "EventListener.h"

#include "PhysicsScene.h"

#include "Logger.h"

namespace Crimson {
	EventListener::EventListener(PhysicsScene* physicsScene) : m_physicsScene(physicsScene) {

	}

	void EventListener::onContact(const rp3d::CollisionCallback::CallbackData& callbackData) {
		// For each contact pair
		for (uint p = 0; p < callbackData.getNbContactPairs(); p++) {

			// Get the contact pair
			rp3d::CollisionCallback::ContactPair contactPair = callbackData.getContactPair(p);

			// For each contact point of the contact pair
			for (uint c = 0; c < contactPair.getNbContactPoints(); c++) {
				if (contactPair.getEventType() == rp3d::CollisionCallback::ContactPair::EventType::ContactExit) {
					m_physicsScene->ContactExit(contactPair.getCollider1()->getBody(), contactPair.getCollider2()->getBody());
					CR_PRINTF("%s\n", "hi");
				} else if (contactPair.getEventType() == rp3d::CollisionCallback::ContactPair::EventType::ContactStart) {
					m_physicsScene->ContactEnter(contactPair.getCollider1()->getBody(), contactPair.getCollider2()->getBody());
				} else if (contactPair.getEventType() == rp3d::CollisionCallback::ContactPair::EventType::ContactStay) {
					m_physicsScene->ContactStay(contactPair.getCollider1()->getBody(), contactPair.getCollider2()->getBody());
				}
			}
		}
	}
}
