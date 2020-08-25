#include <Crimson.h>

#include <glad/glad.h>


class Application : public Crimson::Application {
private:
   unsigned int m_vbo;
   Crimson::Shader m_shader;
public:
   void OnBegin() override  {
      float verts[] = {
         0.0f, 0.0f, -10.0f,
         3.0f, 0.0f, -10.0f,
         0.0f, 3.0f, -10.0f
      };

      glGenBuffers(1, &m_vbo);
      glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
      glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
      glEnableVertexAttribArray(0);

      Crimson::Matrix4 ortho = Crimson::Matrix4::Perspective(45.0f, 1.33f, 0.1f, 100.0f);

      m_shader.Init("Resources/Basic.vert", "Resources/Basic.frag");
      m_shader.Bind();
      m_shader.SetUniformMatrix4("proj", ortho);
   }

   void OnUpdate(float delta) override {

   }

   void OnRender(float delta) override {
      glDrawArrays(GL_TRIANGLES, 0, 3);
   }
};

int main(int argc, char const *argv[]) {
   Application app;
   app.Run();

   return 0;
}
