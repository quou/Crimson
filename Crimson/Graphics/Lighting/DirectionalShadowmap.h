#ifndef DIRECTIONALSHADOWMAP_H
#define DIRECTIONALSHADOWMAP_H

#include "Graphics/Shader.h"

namespace Crimson {
   class DirectionalShadowmap {
   private:
      const unsigned int m_res{10240};

      unsigned int m_depthMap;
      unsigned int m_depthFBO;

      Shader shader;
   public:
      DirectionalShadowmap();

      void Init();

      void BeginRender();
      void EndRender();

      inline unsigned int GetRes() const { return m_res;}
      inline unsigned int GetOutput() const { return m_depthMap;}

      Shader* GetShader() {return &shader;}
   };
}

#endif /* end of include guard: DIRECTIONALSHADOWMAP_H */
