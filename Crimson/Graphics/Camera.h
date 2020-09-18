#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace Crimson {
   class Camera {
   private:
      glm::mat4 m_perspective;

      glm::vec3 m_position;
      glm::vec3 m_forward; /* forward and up are for rotation */
      glm::vec3 m_up;

      glm::vec3 m_rotation;

      float m_yaw;
      float m_pitch;

      float m_fov;
      float m_aspect;
      float m_near;
      float m_far;

   public:
      Camera() {}
      Camera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar) {
         m_fov = fov;
         m_aspect = aspect;
         m_near = zNear;
         m_far = zFar;

         m_perspective = glm::perspective(fov, aspect, zNear, zFar);
         m_position = pos;

         m_forward = glm::vec3(0.0f,0.0f,1.0f);
         m_up = glm::vec3(0,1,0);

         SetRotation(90.0f, 0.0f);
      }

      inline glm::mat4 GetViewProjection() const {
         return m_perspective * glm::lookAt(m_position, m_position + m_forward, m_up);
      }

      inline glm::mat4 GetView() const {
         return glm::lookAt(m_position, m_position + m_forward, m_up);
      }

      inline glm::mat4 GetProjection() const {
         return m_perspective;
      }

      void SetPosition(const glm::vec3& newPos) {
         m_position = newPos;
      }

      void SetFOV(float fov) {m_fov = fov;}
      void SetNear(float near) {m_near = near;}
      void SetFar(float far) {m_far = far;}

      void UpdatePerspective(float fov, float aspect, float zNear, float zFar) {
         m_perspective = glm::perspective(fov, aspect, zNear, zFar);
      }

      void SetRotation(float yaw, float pitch) {
         m_yaw = yaw;
         m_pitch = pitch;

         glm::vec3 front;
         front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
         front.y = sin(glm::radians(pitch));
         front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
         m_forward = glm::normalize(front);
      }

      inline const glm::vec3& GetPosition() {return m_position;}
      inline float GetYaw() {return m_yaw;}
      inline float GetPitch() {return m_pitch;}

      inline float GetFOV() const {return m_fov;}
      inline float GetNear() const {return m_near;}
      inline float GetFar() const {return m_far;}

      virtual ~Camera() {}
   };
}

#endif /* end of include guard: CAMERA_H */
