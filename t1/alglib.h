#ifndef ALGLIB_H
#define	ALGLIB_H

#include <array>
#include <cmath>
#include <GL/gl.h>

namespace Alglib {
    class Tuple2{
        public:
            GLfloat x,y,w;

            GLfloat GetX() const {return this->x;};
            GLfloat GetY() const {return this->y;};
            GLfloat GetW() const {return this->w;};

            void SetX(GLfloat value) {x = value;};
            void SetY(GLfloat value) {y = value;};
            void SetW(GLfloat value) {w = value;};
            
            Tuple2(GLfloat w){
                this->x = 0.0f;
                this->y = 0.0f;
                this->w = w;
            };

            Tuple2(GLfloat x,GLfloat y, GLfloat w){
                this->x = x;
                this->y = y;
                this->w = w;
            };

            void Print() const;

    };

    class Mat2{
        private:
            std::array<std::array<GLfloat, 3>, 3> m;

        public:
            Mat2();
            static Mat2 Identity();
            
            Mat2 Copy() const;
            Mat2 MatrixMultiply(const Mat2& other) const;
            Mat2& Scale(GLfloat sX, GLfloat sY);
            Mat2& Rotate(GLfloat theta);
            Mat2& Translate(GLfloat dX, GLfloat dY);

            Tuple2 Transform(const Tuple2& t) const;

            void Print() const;

            const std::array<std::array<GLfloat, 3>, 3>& Data() const { return m; }
    };
}

#endif