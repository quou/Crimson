#include "SceneManager.h"


#include <glm/glm.hpp>

#include <fstream>
#include <algorithm>

#include "ComponentSystems/Transform.h"
#include "ComponentSystems/GraphicsSystems.h"
#include "Graphics/Lighting/PointLight.h"
#include "Graphics/Renderer.h"

using namespace tinyxml2;


namespace Crimson {

   void SceneManager::Init() {
      std::vector<std::string> skyboxFaces;
      skyboxFaces.push_back("Resources/skybox/right.jpg");
      skyboxFaces.push_back("Resources/skybox/left.jpg");
      skyboxFaces.push_back("Resources/skybox/top.jpg");
      skyboxFaces.push_back("Resources/skybox/bottom.jpg");
      skyboxFaces.push_back("Resources/skybox/front.jpg");
      skyboxFaces.push_back("Resources/skybox/back.jpg");

      m_skybox.Load(skyboxFaces, "Resources/Skybox.vert", "Resources/Skybox.frag");

      m_directionalShadowmap.Init();
   }

   void SceneManager::CreateEntity(const std::string& name, ECS& ecs) {
      EntityHandle newEntity = ecs.CreateEntity();
      ecs.AddComponent<Transform>(newEntity, name);
      ecs.GetComponent<Transform>(newEntity)->position = glm::vec3(0,0,0);
      ecs.GetComponent<Transform>(newEntity)->rotation = glm::vec3(0,0,0);
      ecs.GetComponent<Transform>(newEntity)->scale = glm::vec3(1,1,1);
      m_entities.push_back(newEntity);
   }

   void SceneManager::DeleteEntity(EntityHandle entity, ECS& ecs) {
      m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), entity), m_entities.end());
      ecs.DestroyEntity(entity);
   }

   static auto FindEntity(XMLElement* node, const char* name)  {
      std::vector<XMLElement*> found;
         for (
            auto el = node->FirstChildElement(name);
            el;
            el = el->NextSiblingElement(name)
         ) {
            found.push_back(el);
         }
      return found;
   }

   EntityHandle SceneManager::ParseEntity(XMLElement* node, ECS& ecs) {
      EntityHandle newEntity = ecs.CreateEntity();

      /* TRANSFORM LOADING */
      XMLElement* eComponent = node->FirstChildElement("transform");
      if (eComponent) {
         std::string name = eComponent->Attribute("name");
         glm::vec3 position;
         glm::vec3 rotation;
         glm::vec3 scale;

         XMLElement* ePosition = eComponent->FirstChildElement("position");
         position = glm::vec3(ePosition->FloatAttribute("x"), ePosition->FloatAttribute("y"), ePosition->FloatAttribute("z"));

         XMLElement* eRotation = eComponent->FirstChildElement("rotation");
         rotation = glm::vec3(eRotation->FloatAttribute("x"), eRotation->FloatAttribute("y"), eRotation->FloatAttribute("z"));

         XMLElement* eScale = eComponent->FirstChildElement("scale");
         scale = glm::vec3(eScale->FloatAttribute("x"), eScale->FloatAttribute("y"), eScale->FloatAttribute("z"));

         ecs.AddComponent<Transform>(newEntity, name, position, rotation, scale);
      }

      /* MODEL LOADING */
      eComponent = node->FirstChildElement("model");
      if (eComponent) {
         ecs.AddComponent<ModelComponent>(newEntity);

         XMLElement* eTexture = eComponent->FirstChildElement("texture");
         std::string texRes = eTexture->Attribute("res");

         XMLElement* eMesh = eComponent->FirstChildElement("mesh");
         std::string meshRes = eMesh->Attribute("res");

         XMLElement* eMaterial = eComponent->FirstChildElement("material");
         XMLElement* eAmbient = eMaterial->FirstChildElement("ambient");
         XMLElement* eDiffuse = eMaterial->FirstChildElement("diffuse");
         XMLElement* eSpecular = eMaterial->FirstChildElement("specular");
         XMLElement* eShininess = eMaterial->FirstChildElement("shininess");
         glm::vec3 matAmbient(eAmbient->FloatAttribute("x"), eAmbient->FloatAttribute("y"), eAmbient->FloatAttribute("z"));
         glm::vec3 matDiffuse(eDiffuse->FloatAttribute("x"), eDiffuse->FloatAttribute("y"), eDiffuse->FloatAttribute("z"));
         glm::vec3 matSpecular(eSpecular->FloatAttribute("x"), eSpecular->FloatAttribute("y"), eSpecular->FloatAttribute("z"));
         float matShininess = eShininess->FloatAttribute("shininess");

         XMLElement* eShader = eMaterial->FirstChildElement("shader");
         std::string vertPath = eShader->Attribute("vertex");
         std::string fragPath = eShader->Attribute("fragment");

         ecs.GetComponent<ModelComponent>(newEntity)->shader.Init(vertPath, fragPath);
         ecs.GetComponent<ModelComponent>(newEntity)->texture.Load(texRes);
         ecs.GetComponent<ModelComponent>(newEntity)->model.Load(meshRes);
         ecs.GetComponent<ModelComponent>(newEntity)->material = {matAmbient, matDiffuse, matSpecular, matShininess};
      }

      /* POINT LIGHT LOADING */
      eComponent = node->FirstChildElement("pointlight");
      if (eComponent) {
         XMLElement* eAmbient = eComponent->FirstChildElement("ambient");
         XMLElement* eDiffuse = eComponent->FirstChildElement("diffuse");
         XMLElement* eSpecular = eComponent->FirstChildElement("specular");
         glm::vec3 ambient(eAmbient->FloatAttribute("x"), eAmbient->FloatAttribute("y"), eAmbient->FloatAttribute("z"));
         glm::vec3 diffuse(eDiffuse->FloatAttribute("x"), eDiffuse->FloatAttribute("y"), eDiffuse->FloatAttribute("z"));
         glm::vec3 specular(eSpecular->FloatAttribute("x"), eSpecular->FloatAttribute("y"), eSpecular->FloatAttribute("z"));

         XMLElement* ePoint = eComponent->FirstChildElement("point");
         float constant = ePoint->FloatAttribute("constant");
         float linear = ePoint->FloatAttribute("linear");
         float quadratic = ePoint->FloatAttribute("quadratic");

         ecs.AddComponent<PointLight>(newEntity, constant, linear, quadratic, ambient, diffuse, specular);
      }

      m_entities.push_back(newEntity);

      return newEntity;
   }

   void SceneManager::SerializeEntity(EntityHandle ent, XMLPrinter& printer, ECS& ecs) {
      printer.OpenElement("entity");

      if (ecs.HasComponent<Transform>(ent)) {
         std::string name = ecs.GetComponent<Transform>(ent)->name;
         glm::vec3 position = ecs.GetComponent<Transform>(ent)->position;
         glm::vec3 rotation = ecs.GetComponent<Transform>(ent)->rotation;
         glm::vec3 scale = ecs.GetComponent<Transform>(ent)->scale;

         printer.OpenElement("transform");
            printer.PushAttribute("name", name.c_str());

            printer.OpenElement("position");
               printer.PushAttribute("x", position.x);
               printer.PushAttribute("y", position.y);
               printer.PushAttribute("z", position.z);
            printer.CloseElement();

            printer.OpenElement("rotation");
               printer.PushAttribute("x", rotation.x);
               printer.PushAttribute("y", rotation.y);
               printer.PushAttribute("z", rotation.z);
            printer.CloseElement();

            printer.OpenElement("scale");
               printer.PushAttribute("x", scale.x);
               printer.PushAttribute("y", scale.y);
               printer.PushAttribute("z", scale.z);
            printer.CloseElement();
         printer.CloseElement();
      }

      if (ecs.HasComponent<ModelComponent>(ent)) {
         std::string vertPath = ecs.GetComponent<ModelComponent>(ent)->shader.GetVertPath();
         std::string fragPath = ecs.GetComponent<ModelComponent>(ent)->shader.GetFragPath();
         std::string texRes = ecs.GetComponent<ModelComponent>(ent)->texture.GetRes();
         std::string meshRes = ecs.GetComponent<ModelComponent>(ent)->model.GetRes();
         glm::vec3 matAmbient = ecs.GetComponent<ModelComponent>(ent)->material.ambient;
         glm::vec3 matDiffuse = ecs.GetComponent<ModelComponent>(ent)->material.diffuse;
         glm::vec3 matSpecular = ecs.GetComponent<ModelComponent>(ent)->material.specular;
         float matShininess = ecs.GetComponent<ModelComponent>(ent)->material.shininess;

         printer.OpenElement("model");
            printer.OpenElement("texture");
               printer.PushAttribute("res", texRes.c_str());
            printer.CloseElement();

            printer.OpenElement("mesh");
               printer.PushAttribute("res", meshRes.c_str());
            printer.CloseElement();

            printer.OpenElement("material");
               printer.OpenElement("ambient");
                  printer.PushAttribute("x", matAmbient.x);
                  printer.PushAttribute("y", matAmbient.y);
                  printer.PushAttribute("z", matAmbient.z);
               printer.CloseElement();

               printer.OpenElement("diffuse");
                  printer.PushAttribute("x", matDiffuse.x);
                  printer.PushAttribute("y", matDiffuse.y);
                  printer.PushAttribute("z", matDiffuse.z);
               printer.CloseElement();

               printer.OpenElement("specular");
                  printer.PushAttribute("x", matSpecular.x);
                  printer.PushAttribute("y", matSpecular.y);
                  printer.PushAttribute("z", matSpecular.z);
               printer.CloseElement();

               printer.OpenElement("shininess");
                  printer.PushAttribute("shininess", matShininess);
               printer.CloseElement();

               printer.OpenElement("shader");
                  printer.PushAttribute("vertex", vertPath.c_str());
                  printer.PushAttribute("fragment", fragPath.c_str());
               printer.CloseElement();
            printer.CloseElement();
         printer.CloseElement();
      }

      if (ecs.HasComponent<PointLight>(ent)) {
         glm::vec3 ambient = ecs.GetComponent<PointLight>(ent)->ambient;
         glm::vec3 diffuse = ecs.GetComponent<PointLight>(ent)->diffuse;
         glm::vec3 specular = ecs.GetComponent<PointLight>(ent)->specular;
         float constant = ecs.GetComponent<PointLight>(ent)->constant;
         float linear = ecs.GetComponent<PointLight>(ent)->linear;
         float quadratic = ecs.GetComponent<PointLight>(ent)->quadratic;

         printer.OpenElement("pointlight");
            printer.OpenElement("point");
               printer.PushAttribute("constant", constant);
               printer.PushAttribute("linear", linear);
               printer.PushAttribute("quadratic", quadratic);
            printer.CloseElement();

            printer.OpenElement("ambient");
               printer.PushAttribute("x", ambient.x);
               printer.PushAttribute("y", ambient.y);
               printer.PushAttribute("z", ambient.z);
            printer.CloseElement();

            printer.OpenElement("diffuse");
               printer.PushAttribute("x", diffuse.x);
               printer.PushAttribute("y", diffuse.y);
               printer.PushAttribute("z", diffuse.z);
            printer.CloseElement();

            printer.OpenElement("specular");
               printer.PushAttribute("x", specular.x);
               printer.PushAttribute("y", specular.y);
               printer.PushAttribute("z", specular.z);
            printer.CloseElement();
         printer.CloseElement();
      }

      if (ecs.HasComponent<Transform>(ent)) {
         for (auto& e : ecs.GetComponent<Transform>(ent)->children) {
            SerializeEntity(e, printer, ecs);
         }
      }

      printer.CloseElement();
   }


   void SceneManager::ParseEntities(tinyxml2::XMLElement* node, ECS& ecs, EntityHandle parent) {
      for (auto el : FindEntity(node, "entity")) {
         auto entity = ParseEntity(el, ecs);

         if (parent && entity) {
            AddChild(ecs.GetComponent<Transform>(parent), entity);
            AddParent(ecs.GetComponent<Transform>(entity), parent);
         }

         ParseEntities(el, ecs, entity);
      }
   }

   void SceneManager::Serialize(const std::string& fileName, ECS& ecs) {
      XMLPrinter printer;

      printer.OpenElement("scene");

      /* SAVE SCENE CONFIG */
      printer.OpenElement("scenesettings");
         printer.OpenElement("directionallight");
            printer.OpenElement("direction");
               printer.PushAttribute("x", GetConfig()->directionalLight.direction.x);
               printer.PushAttribute("y", GetConfig()->directionalLight.direction.y);
               printer.PushAttribute("z", GetConfig()->directionalLight.direction.z);
            printer.CloseElement();

            printer.OpenElement("ambient");
               printer.PushAttribute("x", GetConfig()->directionalLight.ambient.x);
               printer.PushAttribute("y", GetConfig()->directionalLight.ambient.y);
               printer.PushAttribute("z", GetConfig()->directionalLight.ambient.z);
            printer.CloseElement();

            printer.OpenElement("diffuse");
               printer.PushAttribute("x", GetConfig()->directionalLight.diffuse.x);
               printer.PushAttribute("y", GetConfig()->directionalLight.diffuse.y);
               printer.PushAttribute("z", GetConfig()->directionalLight.diffuse.z);
            printer.CloseElement();

            printer.OpenElement("specular");
               printer.PushAttribute("x", GetConfig()->directionalLight.specular.x);
               printer.PushAttribute("y", GetConfig()->directionalLight.specular.y);
               printer.PushAttribute("z", GetConfig()->directionalLight.specular.z);
            printer.CloseElement();
         printer.CloseElement();
      printer.CloseElement();

      for (EntityHandle ent : m_entities) {
         if (!ecs.GetComponent<Transform>(ent)->parent) {
            SerializeEntity(ent, printer, ecs);
         }
      }
      printer.CloseElement();

      std::ofstream file(fileName);
      if (file.is_open()) {
         file << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
         file << printer.CStr();
         file.close();
      }

      std::cout << "Saved Scene: " << fileName <<'\n';
   }


   int SceneManager::Deserialize(const std::string& fileName, ECS& ecs) {
      XMLDocument doc;
      XMLError result = doc.LoadFile(fileName.c_str());
      if (result != XML_SUCCESS) {
         std::cout << "Failed to load file: " << fileName << ". Check that the XML isn't corrupt, make sure the file exists.\n";
         return 0;
      }

      for (EntityHandle ent : m_entities) {
         ecs.DestroyEntity(ent);
      }
      m_entities.clear();

      try {
         /* LOAD SCENE CONFIG */
         XMLElement* eSceneSettings = doc.RootElement()->FirstChildElement("scenesettings");
         XMLElement* eDirectionalLight = eSceneSettings->FirstChildElement("directionallight");
         XMLElement* eDirection = eDirectionalLight->FirstChildElement("direction");
         XMLElement* eAmbient = eDirectionalLight->FirstChildElement("ambient");
         XMLElement* eDiffuse = eDirectionalLight->FirstChildElement("diffuse");
         XMLElement* eSpecular = eDirectionalLight->FirstChildElement("specular");
         GetConfig()->directionalLight.direction = glm::vec3(eDirection->FloatAttribute("x"), eDirection->FloatAttribute("y"), eDirection->FloatAttribute("z"));
         GetConfig()->directionalLight.ambient = glm::vec3(eAmbient->FloatAttribute("x"), eAmbient->FloatAttribute("y"), eAmbient->FloatAttribute("z"));
         GetConfig()->directionalLight.diffuse = glm::vec3(eDiffuse->FloatAttribute("x"), eDiffuse->FloatAttribute("y"), eDiffuse->FloatAttribute("z"));
         GetConfig()->directionalLight.specular = glm::vec3(eSpecular->FloatAttribute("x"), eSpecular->FloatAttribute("y"), eSpecular->FloatAttribute("z"));

         ParseEntities(doc.RootElement(), ecs);
      } catch (const std::exception& e) {
         std::cout << e.what() << "\n";
      }

      std::cout << "Loaded Scene: " << fileName <<'\n';

      return 1;
   }
}
