#include "SceneManager.h"

#include "Utils/tinyxml2.h"

#include <glm/glm.hpp>

#include <fstream>
#include <algorithm>

#include "ComponentSystems/Transform.h"
#include "ComponentSystems/GraphicsSystems.h"
#include "Graphics/Renderer.h"

using namespace tinyxml2;


namespace Crimson {
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


   void SceneManager::Serialize(const std::string& fileName, ECS& ecs) {
      XMLPrinter printer;

      for (EntityHandle ent : m_entities) {
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
            std::string matRes = ecs.GetComponent<ModelComponent>(ent)->material.resFile;

            printer.OpenElement("model");
               printer.OpenElement("shader");
                  printer.PushAttribute("vertex", vertPath.c_str());
                  printer.PushAttribute("fragment", fragPath.c_str());
               printer.CloseElement();

               printer.OpenElement("texture");
                  printer.PushAttribute("res", texRes.c_str());
               printer.CloseElement();

               printer.OpenElement("mesh");
                  printer.PushAttribute("res", meshRes.c_str());
               printer.CloseElement();

               printer.OpenElement("material");
                  printer.PushAttribute("res", matRes.c_str());
               printer.CloseElement();
            printer.CloseElement();
         }

         printer.CloseElement();
      }

      std::ofstream file(fileName);
      if (file.is_open()) {
         file << printer.CStr();
         file.close();
      }
   }

   void SceneManager::Deserialize(const std::string& fileName, ECS& ecs) {
      XMLDocument doc;
      doc.LoadFile(fileName.c_str());

      XMLElement* eEntity = doc.FirstChildElement("entity");
      while (eEntity) {
         EntityHandle newEntity = ecs.CreateEntity();

         /* TRANSFORM LOADING */
         XMLElement* eComponent = eEntity->FirstChildElement("transform");
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
         eComponent = eEntity->FirstChildElement("model");
         if (eComponent) {
            ecs.AddComponent<ModelComponent>(newEntity);

            XMLElement* eShader = eComponent->FirstChildElement("shader");
            std::string vertPath = eShader->Attribute("vertex");
            std::string fragPath = eShader->Attribute("fragment");

            XMLElement* eTexture = eComponent->FirstChildElement("texture");
            std::string texRes = eTexture->Attribute("res");

            XMLElement* eMesh = eComponent->FirstChildElement("mesh");
            std::string meshRes = eMesh->Attribute("res");

            XMLElement* eMaterial = eComponent->FirstChildElement("material");
            XMLDocument matDoc;
            std::string matRes = eMaterial->Attribute("res");
            matDoc.LoadFile(eMaterial->Attribute("res"));
            XMLElement* matRoot = matDoc.FirstChildElement("material");
            float matSpecularIntensity = matRoot->FloatAttribute("specular_intensity");
            float matShininess = matRoot->FloatAttribute("shininess");

            ecs.GetComponent<ModelComponent>(newEntity)->shader.Init(vertPath, fragPath);
            ecs.GetComponent<ModelComponent>(newEntity)->texture.Load(texRes);
            ecs.GetComponent<ModelComponent>(newEntity)->model.Load(meshRes);
            ecs.GetComponent<ModelComponent>(newEntity)->material = Material(matSpecularIntensity, matShininess, matRes);
         }

         m_entities.push_back(newEntity);
         eEntity = eEntity->NextSiblingElement("entity");
      }
   }
}
