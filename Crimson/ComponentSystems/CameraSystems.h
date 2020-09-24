#ifndef CAMERA_SYSTEMS_H
#define CAMERA_SYSTEMS_H

#include "Graphics/Camera.h"
#include "Graphics/RenderTarget.h"
#include "Transform.h"

#include "ECS.h"

namespace Crimson {
   struct CameraComponent {
      Camera camera;
      bool isCurrent;
   };

   class CameraSystem : public System {
   public:
      inline void Update(ECS& ecs, int renderWidth, int renderHeight) {
         for (const auto& ent : m_entities) {
            Camera* cam = &ecs.GetComponent<CameraComponent>(ent).camera;
            cam->SetPosition(ecs.GetComponent<Transform>(ent).position);
            cam->UpdatePerspective(cam->GetFOV(), (float)renderWidth/(float)renderHeight, cam->GetNear(), cam->GetFar());
            cam->SetRotation(ecs.GetComponent<Transform>(ent).rotation.y, -ecs.GetComponent<Transform>(ent).rotation.x);
         }
      }
   };
}

#endif /* end of include guard: CAMERA_SYSTEMS_H */
