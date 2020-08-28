#ifndef MODEL_H
#define MODEL_H

#include "Utils/obj_loader.h"

namespace Crimson {
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
   public:
      Model() {}
      Model(const std::string& fileName);

      void Load(const std::string& fileName);

      void Render();

      virtual ~Model();
   };
}
#endif /* end of include guard: MODEL_H */
