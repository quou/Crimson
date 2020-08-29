#ifndef RENDERER_H
#define RENDERER_H

#include <vector>


namespace Crimson {
   class Shader;
   class SceneManager;

   class Renderer {
   private:
      std::vector<Shader*> m_shaders;
   public:
      void Init();
      void Clear(float r, float g, float b, float a);

      void AddShader(Shader* shader);

      void UpdateLighting(SceneManager& sceneManager);
   };
}

#endif /* end of include guard: RENDERER_H */
