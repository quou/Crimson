#include "EventListener.h"

#include "PhysicsScene.h"

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
				rp3d::CollisionCallback::ContactPoint contactPoint = contactPair.getContactPoint(c);
				m_physicsScene->Contact(contactPair.getCollider1()->getBody(), contactPair.getCollider2()->getBody());
			}
		}
	}
}
