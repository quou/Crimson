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
   Crimson::Light m_directional;

   glm::mat4 m_modl;

   float m_curAngle = 0.0f;
public:
   App() :
      m_model("Resources/monkey3.obj"),
      m_texture("Resources/Wood.jpg"),
      m_shader("Resources/Basic.vert", "Resources/Basic.frag") {}

   void OnBegin() override  {
      glm::mat4 pers = glm::perspective(45.0f, 1.33f, 0.1f, 100.0f);

      m_texture.Bind(0);

      m_modl = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -5));
      m_modl = glm::rotate(m_modl, 2.5f, glm::vec3(0, 1, 0));
      m_modl = glm::scale(m_modl, glm::vec3(1.0f, 1.0f, 1.0f));

      m_shader.Bind();
      m_shader.SetUniform1i("tex", 0);
      m_shader.SetUniformMatrix4("proj", pers);
      m_shader.SetUniformMatrix4("modl", m_modl);

      m_directional.UseLight(m_shader.GetUniformLocation("directionalLight.ambientIntensity"),
                             m_shader.GetUniformLocation("directionalLight.color"),
                             m_shader.GetUniformLocation("directionalLight.diffuseIntensity"),
                             m_shader.GetUniformLocation("directionalLight.direction"));

      m_shader.SetUniform3f("directionalLight.color", 1, 1, 1);
      m_shader.SetUniform1f("directionalLight.ambientIntensity", 0.25f);
      m_shader.SetUniform1f("directionalLight.diffuseIntensity", 1);
      m_shader.SetUniform3f("directionalLight.direction", -1, 1, 0);
   }

   void OnUpdate(float delta) override {

   }

   void OnRender(float delta) override {
      m_shader.SetUniformMatrix4("modl", m_modl);
      m_model.Render();
   }
};

int main(int argc, char const *argv[]) {
   App app;
   app.Run();

   return 0;
}
