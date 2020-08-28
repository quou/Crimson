/*
MIT License

Copyright (c) 2020 George Lamb

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


 --- NOTES ---
 - After including this file, you must define `const int MAX_ENTITIES = someValue;` in **one** source file.
 - Currently tested with CMake 3.16.3, GNU Make 4.2.1 and GCC 9.3 on Ubuntu 20 LTS
 - YOU MUST USE C++ 14 OR ABOVE. Otherwise it will fail to compile
*/


#ifndef SLECS_H
#define SLECS_H

#define MAX_COMPONENTS 32
#define MAX_ENTITIES 1000

#include <iostream>
#include <vector>
#include <bitset>
#include <utility>
#include <tuple>

static unsigned int impl_componentCounter = 0;
template <typename T>
unsigned int GetComponentID() {
   static unsigned int s_componentID = impl_componentCounter++;
   return s_componentID;
}

typedef unsigned int EntityIndex;
typedef unsigned int EntityVersion;
typedef unsigned long long EntityHandle;

typedef std::bitset<MAX_COMPONENTS> ComponentMask;


inline EntityHandle CreateHandleToEntity(EntityIndex index, EntityVersion version) {
  return ((EntityHandle)index << 32) | ((EntityHandle)version);
}

inline EntityIndex GetEntityIndex(EntityHandle entity) {
  return entity >> 32;
}

inline EntityVersion GetEntityVersion(EntityHandle entity) {
  return (EntityVersion)entity;
}

inline bool IsEntityValid(EntityHandle entity) {
  return GetEntityIndex(entity) != EntityIndex(-1);
}

#define INVALID_ENTITY CreateHandleToEntity(EntityIndex(-1), 0)


/* This class is a pool of memory for components
   It is stored in a char array, since we cannot know the
   size of the container at compile time*/
class ComponentContainer {
private:
   char* m_data {nullptr};
   size_t m_elementSize {0};
public:
   ComponentContainer(size_t elementSize) {
      m_elementSize = elementSize;
      m_data = new char[m_elementSize * MAX_ENTITIES];
   }

   ~ComponentContainer() {
      delete[] m_data;
   }

   inline void* Get(size_t index) {
      // Look up the component
      return m_data + index * m_elementSize;
   }
};


struct ECS {
private:
   std::vector<std::pair<EntityHandle, ComponentMask>> m_entities;
   std::vector<ComponentContainer*> m_components;
   std::vector<EntityIndex> m_freeEntities; /* A vector of entities that can be overwritton */

public:
   ~ECS() {
      for (size_t i = 0; i < m_components.size(); i++) {
         delete m_components[i];
      }
   }

   inline const std::vector<std::pair<EntityHandle, ComponentMask>>& Internal_GetEntities() const {
      return m_entities;
   }

   template <typename T>
   bool HasComponent(EntityHandle entity) {
      return m_entities[GetEntityIndex(entity)].second.test(GetComponentID<T>());
   }

   template <typename T, typename... Args>
   T* AddComponent(EntityHandle entity, Args... pArgs) {
      unsigned int componentID = GetComponentID<T>();

      if (m_entities[GetEntityIndex(entity)].first != entity) {
         std::cerr << __FUNCTION__ << ": Attempt to access a deleted entity." << '\n';
         return nullptr;
      }

      if (m_components.size() <= componentID) {
         m_components.resize(componentID + 1, nullptr);
      }

      if (m_components[componentID] == nullptr) {
         m_components[componentID] = new ComponentContainer(sizeof(T));
      }

      T* newComponent = new(m_components[componentID]->Get(GetEntityIndex(entity))) T{pArgs...};

      m_entities[GetEntityIndex(entity)].second.set(componentID);
      return newComponent;
   }

   template <typename T>
   T* GetComponent(EntityHandle entity) {
      int componentID = GetComponentID<T>();

      if (m_entities[GetEntityIndex(entity)].first != entity) {
         std::cerr << __FUNCTION__ << ": Attempt to access a deleted entity." << '\n';
         return nullptr;
      }

      if (!m_entities[GetEntityIndex(entity)].second.test(componentID)) {
         std::cerr << __FUNCTION__ << ": Component doesn't exist on the requested entity. Returning nullptr instead." << '\n';
         return nullptr;
      }

      T* gotComponent = static_cast<T*>(m_components[componentID]->Get(GetEntityIndex(entity)));
      return gotComponent;
   }

   template <typename T>
   void RemoveComponent(EntityHandle entity) {
      if (m_entities[GetEntityIndex(entity)].first != entity) {
         std::cerr << __FUNCTION__ << ": Attempt to access a deleted entity." << '\n';
         return;
      }

      m_entities[GetEntityIndex(entity)].second.reset(GetComponentID<T>());
   }


   void DestroyEntity(EntityHandle entity) {
      EntityHandle newEntity = CreateHandleToEntity(EntityIndex(-1), GetEntityVersion(entity) + 1);

      EntityIndex index = GetEntityIndex(entity);

      m_entities[index].first = newEntity;
      m_entities[index].second.reset();
      m_freeEntities.push_back(index);
   }

   EntityHandle CreateEntity() {
      if (!m_freeEntities.empty()) {
         EntityIndex newIndex = m_freeEntities.back();
         m_freeEntities.pop_back();

         EntityHandle newEntity = CreateHandleToEntity(newIndex, GetEntityVersion(m_entities[newIndex].first));

         m_entities[newIndex].first = newEntity;
         return m_entities[newIndex].first;
      }

      m_entities.push_back(std::make_pair(CreateHandleToEntity(m_entities.size(), 0), ComponentMask()));
      return m_entities.back().first;
   }
};

template <typename... ComponentTypes>
class System {
private:
   ECS* m_ecs;
   ComponentMask m_componentMask;
   bool m_all = false;

public:
   System(ECS& ecs) : m_ecs(&ecs) {
      if (sizeof...(ComponentTypes) == 0) {
         m_all = true;
      } else {
         unsigned int componentIDs[] = { 0, GetComponentID<ComponentTypes>()... };
         for (unsigned int i = 1; i < (sizeof...(ComponentTypes) + 1); i++) {
            m_componentMask.set(componentIDs[i]);
         }
      }
   }

   struct Iterator {
      Iterator(ECS* ecs, EntityIndex index, ComponentMask mask, bool all) :
         index(index), ecs(ecs), mask(mask), all(all) {}

      EntityHandle operator*() const {
         return ecs->Internal_GetEntities()[index].first;
      }

      bool operator==(const Iterator& other) const {
         return index == other.index;
      }
      bool operator!=(const Iterator& other) const {
         return index != other.index;
      }

      bool ValidIndex() {
         return
            IsEntityValid(ecs->Internal_GetEntities()[index].first) &&
            (all || mask == (mask & ecs->Internal_GetEntities()[index].second));
      }

      Iterator& operator++() {
         do {
            index++;
         } while (index < ecs->Internal_GetEntities().size() && !ValidIndex());
         return *this;
      }

      EntityIndex index;
      ECS* ecs;
      ComponentMask mask;
      bool all = false;
   };

   const Iterator begin() const {
     int firstIndex = 0;
     while (firstIndex < m_ecs->Internal_GetEntities().size() &&
       (m_componentMask != (m_componentMask & m_ecs->Internal_GetEntities()[firstIndex].second)
         || !IsEntityValid(m_ecs->Internal_GetEntities()[firstIndex].first))) {
          firstIndex++;
      }
      return Iterator(m_ecs, firstIndex, m_componentMask, m_all);
   }

   const Iterator end() const {
     return Iterator(m_ecs, EntityIndex(m_ecs->Internal_GetEntities().size()), m_componentMask, m_all);
   }


};


#endif /* end of include guard: SLECS_H */
