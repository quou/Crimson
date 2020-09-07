#ifndef CUBEMAP_H
#define CUBEMAP_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Model.h"
#include "Shader.h"

#include <vector>
#include <string>

namespace Crimson {
   class Camera;

   class Cubemap {
   private:
      Model m_skyModel;
      Shader m_skyShader;

      unsigned int m_textureID;
   public:
      Cubemap();
      Cubemap(std::vector<std::string> facePaths, const std::string vertShader, const std::string& fragShader);

      void Load(std::vector<std::string> facePaths, const std::string vertShader, const std::string& fragShader);

      void Render(Camera& camera);
   };
}

#endif /* end of include guard: CUBEMAP_H */
