#include "RenderTarget.h"

#include <glad/glad.h>

#include <iostream>

namespace Crimson {
   RenderTarget::RenderTarget() {
      glGenFramebuffers(1, &ID);
      glBindFramebuffer(GL_FRAMEBUFFER, ID);

      glGenTextures(1, &outputID);
      glBindTexture(GL_TEXTURE_2D, outputID);

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      glGenTextures(1, &depthID);
      glBindTexture(GL_TEXTURE_2D, depthID);

      glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, 800, 600, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputID, 0);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthID, 0);

      int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
      if (status != GL_FRAMEBUFFER_COMPLETE) {
         std::cout << "Framebuffer error: " << status << '\n';
      }
   }

   void RenderTarget::Bind() {
      glBindFramebuffer(GL_FRAMEBUFFER, ID);
      glViewport(0,0,m_width,m_height);
   }

   void RenderTarget::Clear() {
      Bind();
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   }

   void RenderTarget::Resize(int newWidth, int newHeight) {
      m_width = newWidth;
      m_height = newHeight;

      glBindTexture(GL_TEXTURE_2D, outputID);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newWidth, newHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

      glBindTexture(GL_TEXTURE_2D, depthID);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, newWidth, newHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
   }

   RenderTarget::~RenderTarget() {
      glDeleteTextures(1, &depthID);
      glDeleteTextures(1, &outputID);
      glDeleteFramebuffers(1, &ID);
   }
}
