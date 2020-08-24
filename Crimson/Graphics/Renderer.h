#ifndef RENDERER_H
#define RENDERER_H

namespace Crimson {
   class Renderer {
   private:
   public:
      void Init();
      void Clear(float r, float g, float b, float a);
      void Draw(unsigned int vertexArray, unsigned int drawCount, unsigned int shader);
   };
}

#endif /* end of include guard: RENDERER_H */
