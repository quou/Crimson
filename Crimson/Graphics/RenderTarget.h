#ifndef RENDERTARGET_H
#define RENDERTARGET_H


namespace Crimson {
   class RenderTarget {
   private:
      unsigned int ID;

      unsigned int outputID;
      unsigned int depthID;

      int m_width = 800, m_height = 600;
   public:
      RenderTarget();
      virtual ~RenderTarget();

      void Bind();
      void Clear();

      void Resize(int newWidth, int newHeight);

      inline unsigned int GetOutput() {return outputID;}

      inline unsigned int GetWidth() {return m_width;}
      inline unsigned int GetHeight() {return m_height;}
   };
}

#endif /* end of include guard: RENDERTARGET_H */
