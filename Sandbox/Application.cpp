#include <Crimson.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class App : public Crimson::Application {
private:
   /*unsigned int m_vbo;*/
   Crimson::Shader m_shader;
   Crimson::Model m_model;
   Crimson::Texture m_texture;
public:
   App() :
      m_model("Resources/monkey3.obj"),
      m_texture("Resources/Wood.jpg"),
      m_shader("Resources/Basic.vert", "Resources/Basic.frag") {}

   void OnBegin() override  {
      glm::mat4 pers = glm::perspective(45.0f, 1.33f, 0.1f, 100.0f);

      m_texture.Bind(0);

      glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -5));
      model = glm::rotate(model, 90.0f, glm::vec3(0, 1, 0));

      m_shader.Bind();
      m_shader.SetUniform1i("tex", 0);
      m_shader.SetUniformMatrix4("proj", pers);
      m_shader.SetUniformMatrix4("modl", model);
   }

   void OnUpdate(float delta) override {

   }

   void OnRender(float delta) override {
      m_model.Render();
   }
};

int main(int argc, char const *argv[]) {
   App app;
   app.Run();

   return 0;
}
