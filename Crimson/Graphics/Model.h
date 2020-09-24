#ifndef MODEL_H
#define MODEL_H

#include "Utils/obj_loader.h"

namespace Crimson {
   struct Vertex {
      glm::vec3 pos;
      glm::vec2 uv;
      glm::vec3 normal;

      Vertex(const glm::vec3& pos, const glm::vec2 uv, const glm::vec3& normal = glm::vec3(0,0,0)) {
         this->pos = pos;
         this->uv = uv;
         this->normal = normal;
      }
   };

   class Model {
   private:
      void Init(const IndexedModel& model);

      enum {
         POSITION_VB,
         TEXCOORD_VB,
         NORMAL_VB,

         INDEX_VB,

         NUM_BUFFERS
      };

      unsigned int m_vertexArray;
      unsigned int m_vertexBuffer[NUM_BUFFERS];

      unsigned int m_drawCount;

      std::string m_res;
   public:
      Model() {}
      Model(const std::string& fileName);
      Model(Vertex* vertices, unsigned int vertexCount, unsigned int* indices, unsigned int indexCount);

      void Load(const std::string& fileName);
      void LoadFromData(Vertex* vertices, unsigned int vertexCount, unsigned int* indices, unsigned int indexCount);

      inline std::string GetRes() const {return m_res;}

      void Render();

      virtual ~Model();
   };
}
#endif /* end of include guard: MODEL_H */
