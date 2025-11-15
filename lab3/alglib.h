#ifndef ALGLIB_H
#define	ALGLIB_H

#include <array>
#include <cmath>
#include <GL/gl.h>

namespace Alglib {
    struct Vet2 {
        GLfloat x, y;
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

            Vet2 transformaVetor(const Vet2& p) const;

            void imprime() const;

            const std::array<std::array<GLfloat, 3>, 3>& data() const { return m; }
    };
}

#endif