#ifndef __UNIFORM_GLSL__
#define __UNIFORM_GLSL__

#include <OpenGL.h>

BABLIB_NAMESPACE_BEGIN

class UniformGLSL {
    public:
        UniformGLSL(QString name, GLuint prog) {
            id = glGetUniformLocation(prog, name.toAscii().constData());
            valid = (id != -1);
            }
        
        virtual void setUniform1i(int v)                                    const { glUniform1i(id, v); }
        virtual void setUniform2i(int v0, int v1)                           const { glUniform2i(id, v0, v1); }
        virtual void setUniform3i(int v0, int v1, int v2)                   const { glUniform3i(id, v0, v1, v2); }
        virtual void setUniform4i(int v0, int v1, int v2, int v3)           const { glUniform4i(id, v0, v1, v2, v3); }
        virtual void setUniform1iv(const int *v)                            const { glUniform1iv(id, 1, v); }
        virtual void setUniform2iv(const int *v)                            const { glUniform2iv(id, 1, v); }
        virtual void setUniform3iv(const int *v)                            const { glUniform3iv(id, 1, v); }
        virtual void setUniform4iv(const int *v)                            const { glUniform4iv(id, 1, v); }
        
        virtual void setUniform1f(float v)                                  const { glUniform1f(id, v); }
        virtual void setUniform2f(float v0, float v1)                       const { glUniform2f(id, v0, v1); }
        virtual void setUniform3f(float v0, float v1, float v2)             const { glUniform3f(id, v0, v1, v2); }
        virtual void setUniform4f(float v0, float v1, float v2, float v3)   const { glUniform4f(id, v0, v1, v2, v3); }
        virtual void setUniform1fv(const float *v)                          const { glUniform1fv(id, 1, v); }
        virtual void setUniform2fv(const float *v)                          const { glUniform2fv(id, 1, v); }
        virtual void setUniform3fv(const float *v)                          const { glUniform3fv(id, 1, v); }
        virtual void setUniform4fv(const float *v)                          const { glUniform4fv(id, 1, v); }
        
        virtual void setUniform1dv(const double*v)                          const { glUniform1f(id, float(*v)); }

        virtual void setUniformMatrix2fv(const float *v)                    const { glUniformMatrix2fv(id, 1, GL_FALSE, v); }
        virtual void setUniformMatrix3fv(const float *v)                    const { glUniformMatrix3fv(id, 1, GL_FALSE, v); }
        virtual void setUniformMatrix4fv(const float *v)                    const { glUniformMatrix4fv(id, 1, GL_FALSE, v); }

        virtual void setSampler(GLuint, int texNum)                         const { glUniform1i(id, texNum); }

        void setUniform1b(bool v)                               const { setUniform1i(b2i(v)); }
        void setUniform2b(bool v0, bool v1)                     const { setUniform2i(b2i(v0), b2i(v1)); }
        void setUniform3b(bool v0, bool v1, bool v2)            const { setUniform3i(b2i(v0), b2i(v1), b2i(v2)); }
        void setUniform4b(bool v0, bool v1, bool v2, bool v3)   const { setUniform4i(b2i(v0), b2i(v1), b2i(v2), b2i(v3)); }
        void setUniform1bv(const bool *v)                       const { setUniform1i(b2i(v[0])); }                             
        void setUniform2bv(const bool *v)                       const { setUniform2i(b2i(v[0]), b2i(v[1])); }                   
        void setUniform3bv(const bool *v)                       const { setUniform3i(b2i(v[0]), b2i(v[1]), b2i(v[2])); }          
        void setUniform4bv(const bool *v)                       const { setUniform4i(b2i(v[0]), b2i(v[1]), b2i(v[2]), b2i(v[3])); } 
        
        inline bool exists() const { return valid; }
        
    private:
        static int b2i(bool b) { return b ? 1 : 0; }    // conversion pour envoyer un bool au GPU
        
    private:
        GLint id;
        bool valid;    // true ssi la variable est active dans le programme
    };

BABLIB_NAMESPACE_END

#endif

