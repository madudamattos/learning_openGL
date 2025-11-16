#ifndef ALGLIB_H
#define	ALGLIB_H

#include <array>
#include <cmath>
#include <GL/gl.h>

namespace Alglib {
    class Tuple2{
        public:
            GLfloat x,y,w;

            Tuple2(GLfloat w){
                x = 0.0f;
                y = 0.0f;
                w = w;
            };

            Tuple2(GLfloat x,GLfloat y, GLfloat w){
                x = x;
                y = y;
                w = w;
            };

            void imprime(const Tuple2& t) const;

    };

    class Mat2{
        private:
            std::array<std::array<GLfloat, 3>, 3> m;

        public:
            Mat2();
            static Mat2 identidade();
            
            Mat2 operator*(const Mat2& other) const;
            Mat2& escala(GLfloat sX, GLfloat sY);
            Mat2& rotacao(GLfloat theta);
            Mat2& translacao(GLfloat dX, GLfloat dY);

            Tuple2 transformaTupla(const Tuple2& t) const;

            void imprime() const;

            const std::array<std::array<GLfloat, 3>, 3>& data() const { return m; }
    };
}

#endif