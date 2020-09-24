#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <cstdint>
#include <cassert>
#include <queue>
#include <array>
#include <unordered_map>
#include <memory>
#include <set>

namespace Crimson {
   using EntityHandle = std::uint32_t;
   using ComponentType = std::uint8_t;

   const std::uint32_t MAX_ENTITIES = 5000;
   const std::uint8_t MAX_COMPONENTS = 32;

   using Signature = std::bitset<MAX_COMPONENTS>;

   class EntityManager {
   private:
      std::queue<EntityHandle> m_availableEntities;

      std::array<Signature, MAX_ENTITIES> m_signatures;

      uint32_t m_livingEntityCount{0};
   public:
      EntityManager() {
         for (EntityHandle ent = 0; ent < MAX_ENTITIES; ent++) {
            m_availableEntities.push(ent);
         }
      }

      EntityHandle CreateEntity() {
         assert(m_livingEntityCount < MAX_ENTITIES && "Too many entities in existance");

         EntityHandle newEnt = m_availableEntities.front();
         m_availableEntities.pop();
         ++m_livingEntityCount;

         return newEnt;
      }

      void DestroyEntity(EntityHandle ent) {
         assert(ent < MAX_ENTITIES && "Entity out of range.");

         m_signatures[ent].reset();

         m_availableEntities.push(ent);
         --m_livingEntityCount;
      }

      void SetSignature(EntityHandle ent, Signature signature) {
         assert(ent < MAX_ENTITIES && "Entity out of range.");

         m_signatures[ent] = signature;
      }

      Signature GetSignature(EntityHandle ent) {
         assert(ent < MAX_ENTITIES && "Entity our of range.");

         return m_signatures[ent];
      }
   };

   class IComponentArray {
   public:
      virtual ~IComponentArray() = default;
      virtual void EntityDestroyed(EntityHandle ent) {}
   };

   template <typename T>
   class ComponentArray : public IComponentArray {
   private:
      std::array<T, MAX_COMPONENTS> m_componentArray;

      std::unordered_map<EntityHandle, size_t> m_entityToIndexMap;
      std::unordered_map<size_t, EntityHandle> m_indexToEntityMap;

      size_t m_size;
   public:
      void InsertData(EntityHandle ent, T component) {
         assert(m_entityToIndexMap.find(ent) == m_entityToIndexMap.end() && "Component added to same entity more than once.");

         size_t newIndex = m_size;
         m_entityToIndexMap[ent] = newIndex;
         m_indexToEntityMap[newIndex] = ent;
         m_componentArray[newIndex] = component;
         ++m_size;
      }

      void RemoveData(EntityHandle ent) {
         assert(m_entityToIndexMap.find(ent) != m_entityToIndexMap.end() && "Removing non-existent component.");

         size_t indexOfRemovedEntity = m_entityToIndexMap[ent];
         size_t indexOfLastElement = m_size - 1;
         m_componentArray[indexOfRemovedEntity] = m_componentArray[indexOfLastElement];

         EntityHandle entityOfLastElement = m_indexToEntityMap[indexOfLastElement];
         m_entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
         m_indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

         m_entityToIndexMap.erase(ent);
         m_indexToEntityMap.erase(indexOfLastElement);

         --m_size;
      }


      T& GetData(EntityHandle ent) {
         assert(m_entityToIndexMap.find(ent) != m_entityToIndexMap.end() && "Retrieving non-existent component.");

         return m_componentArray[m_entityToIndexMap[ent]];
      }

      void EntityDestroyed(EntityHandle ent) override {
         if (m_entityToIndexMap.find(ent) != m_entityToIndexMap.end()) {
            RemoveData(ent);
         }
      }
   };

   class ComponentManager {
   private:
      std::unordered_map<const char*, ComponentType> m_componentTypes;
      std::unordered_map<const char*, std::shared_ptr<IComponentArray>> m_componentArrays;

      ComponentType m_nextComponentType;

      template <typename T>
      std::shared_ptr<ComponentArray<T>> GetComponentArray() {
         const char* typeName = typeid(T).name();

         assert(m_componentTypes.find(typeName) != m_componentTypes.end() && "Component used before registration.");

         return std::static_pointer_cast<ComponentArray<T>>(m_componentArrays[typeName]);
      }
   public:
      template <typename T>
      void RegisterComponent() {
         const char* typeName = typeid(T).name();

         assert(m_componentTypes.find(typeName) == m_componentTypes.end() && "Registering component type more than once.");

         m_componentTypes.insert({typeName, m_nextComponentType});
         m_componentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

         ++m_nextComponentType;
      }

      template <typename T>
      ComponentType GetComponentType() {
         const char* typeName = typeid(T).name();

         assert(m_componentTypes.find(typeName) != m_componentTypes.end() && "Component used before registration.");

         return m_componentTypes[typeName];
      }

      template <typename T>
      void AddComponent(EntityHandle ent, T component) {
         GetComponentArray<T>()->InsertData(ent, component);
      }

      template <typename T>
      void RemoveComponent(EntityHandle ent) {
         GetComponentArray<T>()->RemoveData(ent);
      }

      template <typename T>
      T& GetComponent(EntityHandle ent) {
         return GetComponentArray<T>()->GetData(ent);
      }

      void EntityDestroyed(EntityHandle ent) {
         for (const auto& pair : m_componentArrays) {
            const auto& component = pair.second;

            component->EntityDestroyed(ent);
         }
      }
   };

   class System {
   public:
      std::set<EntityHandle> m_entities;
   };

   class SystemManager {
   private:
      std::unordered_map<const char*, Signature> m_signatures;

      std::unordered_map<const char*, std::shared_ptr<System>> m_systems;
   public:
      template<typename T, typename... Args>
      std::shared_ptr<T> RegisterSystem(Args... constructorArgs) {
         const char* typeName = typeid(T).name();

         assert(m_systems.find(typeName) == m_systems.end() && "Registering a system more than once.");

         auto system = std::make_shared<T>(constructorArgs...);
         m_systems.insert({typeName, system});
         return system;
      }

      template<typename T>
      void SetSignature(Signature signature) {
         const char* typeName = typeid(T).name();

         assert(m_systems.find(typeName) != m_systems.end() && "System used before registration.");

         m_signatures.insert({typeName, signature});
      }

      void EntityDestroyed(EntityHandle ent) {
         for (const auto& pair : m_systems) {
            const auto& system = pair.second;

            system->m_entities.erase(ent);
         }
      }

      void EntitySignatureChanged(EntityHandle ent, Signature entitySignature) {
         for (const auto& pair : m_systems) {
            const auto& type = pair.first;
            const auto& system = pair.second;
            const auto& systemSignature = m_signatures[type];

            if ((entitySignature & systemSignature) == systemSignature) {
               system->m_entities.insert(ent);
            } else {
               system->m_entities.erase(ent);
            }
         }
      }
   };

   class ECS {
   private:
      std::unique_ptr<ComponentManager> m_componentManager;
      std::unique_ptr<EntityManager> m_entityManager;
      std::unique_ptr<SystemManager> m_systemManager;
   public:
      void Init() {
         m_componentManager = std::make_unique<ComponentManager>();
         m_entityManager = std::make_unique<EntityManager>();
         m_systemManager = std::make_unique<SystemManager>();
      }

      /* Entities */
      EntityHandle CreateEntity() {
         return m_entityManager->CreateEntity();
      }

      void DestroyEntity(EntityHandle ent) {
         m_entityManager->DestroyEntity(ent);

         m_componentManager->EntityDestroyed(ent);
         m_systemManager->EntityDestroyed(ent);
      }

      /* Components */
      template <typename T>
      void RegisterComponent() {
         m_componentManager->RegisterComponent<T>();
      }

      template <typename T>
      T& AddComponent(EntityHandle ent, T component) {
         m_componentManager->AddComponent<T>(ent, component);

         auto signature = m_entityManager->GetSignature(ent);
         signature.set(m_componentManager->GetComponentType<T>(), true);
         m_entityManager->SetSignature(ent, signature);

         m_systemManager->EntitySignatureChanged(ent, signature);

         return m_componentManager->GetComponent<T>(ent);
      }

      template <typename T>
      void RemoveComponent(EntityHandle ent) {
         m_componentManager->RemoveComponent<T>(ent);

         auto signature = m_entityManager->GetSignature(ent);
         signature.set(m_componentManager->GetComponentType<T>(), false);
         m_entityManager->SetSignature(ent, signature);

         m_systemManager->EntitySignatureChanged(ent, signature);
      }

      template <typename T>
      T& GetComponent(EntityHandle ent) {
         return m_componentManager->GetComponent<T>(ent);
      }

      template <typename T>
      bool HasComponent(EntityHandle ent) {
         return m_entityManager->GetSignature(ent).test(m_componentManager->GetComponentType<T>());
      }

      template <typename T>
      ComponentType GetComponentType() {
         return m_componentManager->GetComponentType<T>();
      }

      /* Systems */
      template <typename T, typename... Args>
      std::shared_ptr<T> RegisterSystem(Args... constructorArgs) {
         return m_systemManager->RegisterSystem<T>(constructorArgs...);
      }

      template <typename T>
      void SetSystemSignature(Signature signature) {
         m_systemManager->SetSignature<T>(signature);
      }
   };
}

#endif /* end of include guard: ECS_H */
