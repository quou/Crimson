#include "DirectionalShadowmap.h"

#include <glad/glad.h>

namespace Crimson {
   DirectionalShadowmap::DirectionalShadowmap() {

   }

   void DirectionalShadowmap::Init() {
      shader.Init("Resources/DirectionalShadowmap.vert", "Resources/DirectionalShadowmap.frag");

      glGenFramebuffers(1, &m_depthFBO);

      glGenTextures(1, &m_depthMap);
      glBindTexture(GL_TEXTURE_2D, m_depthMap);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_res, m_res, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
      float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
      glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

      glBindFramebuffer(GL_FRAMEBUFFER, m_depthFBO);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap, 0);
      glDrawBuffer(GL_NONE);
      glDrawBuffer(GL_NONE);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
   }

   void DirectionalShadowmap::BeginRender() {
      glCullFace(GL_FRONT);
      glViewport(0, 0, m_res, m_res);
      glBindFramebuffer(GL_FRAMEBUFFER, m_depthFBO);
      glClear(GL_DEPTH_BUFFER_BIT);
   }

   void DirectionalShadowmap::EndRender() {
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glCullFace(GL_BACK);
   }
}
