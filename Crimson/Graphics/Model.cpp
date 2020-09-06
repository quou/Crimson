#include "Model.h"

#include <glad/glad.h>

namespace Crimson {
   Model::Model(const std::string& fileName) {
      Load(fileName);
   }

   void Model::Load(const std::string& fileName) {
      m_res = fileName;
      IndexedModel model = OBJModel(fileName).ToIndexedModel();
      Init(model);
   }

   Model::Model(Vertex* vertices, unsigned int vertexCount, unsigned int* indices, unsigned int indexCount) {
      Load(vertices, vertexCount, indices, indexCount);
   }

   void Model::Load(Vertex* vertices, unsigned int vertexCount, unsigned int* indices, unsigned int indexCount) {
      m_drawCount = vertexCount;

      IndexedModel model;

      for (size_t i = 0; i < vertexCount; i++) {
         model.positions.push_back(vertices[i].pos);
         model.texCoords.push_back(vertices[i].uv);
         model.normals.push_back(vertices[i].normal);
      }

      for (size_t i = 0; i < indexCount; i++) {
         model.indices.push_back(indices[i]);
      }

      Init(model);
   }

   Model::~Model() {
      glDeleteVertexArrays(1, &m_vertexArray);
   }

   void Model::Init(const IndexedModel& model) {
      m_drawCount = model.indices.size();

      glGenVertexArrays(1, &m_vertexArray);
      glBindVertexArray(m_vertexArray);

      glGenBuffers(NUM_BUFFERS, m_vertexBuffer);

      /* Vertices */
      glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[POSITION_VB]);
      glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.positions[0]), &model.positions[0], GL_STATIC_DRAW);

      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


      /* Indecies */
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexBuffer[INDEX_VB]);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(model.indices[0]), &model.indices[0], GL_STATIC_DRAW);


      /* UV coords */
      glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[TEXCOORD_VB]);
      glBufferData(GL_ARRAY_BUFFER, model.texCoords.size() * sizeof(model.texCoords[0]), &model.texCoords[0], GL_STATIC_DRAW);

      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);


      /* Normals */
      glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[NORMAL_VB]);
      glBufferData(GL_ARRAY_BUFFER, model.normals.size() * sizeof(model.normals[0]), &model.normals[0], GL_STATIC_DRAW);

      glEnableVertexAttribArray(2);
      glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);


      glBindVertexArray(0);
   }

   void Model::Render() {
      glBindVertexArray(m_vertexArray);

      glDrawElements(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, 0);

      glBindVertexArray(0);
   }
}
