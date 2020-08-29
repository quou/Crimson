#include "SceneManager.h"

#include "Utils/tinyxml2.h"

#include <glm/glm.hpp>

#include "ComponentSystems/Transform.h"
#include "ComponentSystems/GraphicsSystems.h"
#include "Graphics/Renderer.h"

using namespace tinyxml2;

namespace Crimson {
   void SceneManager::Deserialize(const std::string& fileName, ECS& ecs, Renderer& renderer) {
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
            matDoc.LoadFile(eMaterial->Attribute("res"));
            XMLElement* matRoot = matDoc.FirstChildElement("material");
            float matSpecularIntensity = matRoot->FloatAttribute("specular_intensity");
            float matShininess = matRoot->FloatAttribute("shininess");

            ecs.GetComponent<ModelComponent>(newEntity)->shader.Init(vertPath, fragPath, renderer);
            ecs.GetComponent<ModelComponent>(newEntity)->texture.Load(texRes);
            ecs.GetComponent<ModelComponent>(newEntity)->model.Load(meshRes);
            ecs.GetComponent<ModelComponent>(newEntity)->material = Material(matSpecularIntensity, matShininess);
         }

         m_entities.push_back(newEntity);
         eEntity = eEntity->NextSiblingElement("entity");
      }
   }
}
