#ifndef MODELCOMP_H
#define MODELCOMP_H

#include "Graphics/Shader.h"
#include "Graphics/Texture.h"
#include "Graphics/Model.h"
#include "Graphics/Camera.h"
#include "Graphics/Material.h"
#include "SceneManagement/SceneManager.h"
#include "Graphics/Lighting/PointLight.h"
#include "Transform.h"
#include "ECS.h"

#include <glad/glad.h>

namespace Crimson {
   struct ModelComponent {
      Texture texture;
      Shader shader;
      Model model;
      Material material;
   };

   class LightingSystem : public System {
   public:
      void Update(ECS& ecs);
   };

   class GraphicsSystem : public System {
   private:
      void RenderPass(ECS& ecs, Camera& camera, SceneManager& sceneManager);
      void ShadowPass(ECS& ecs, Camera& camera, SceneManager& sceneManager);
   public:
      void RenderShadows(ECS& ecs, SceneManager& sceneManager);
      void Render(ECS& ecs, SceneManager& sceneManager);
   };
}
#endif /* end of include guard: MODELCOMP_H */
