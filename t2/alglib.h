#ifndef ALGLIB_H
#define	ALGLIB_H

#include <array>
#include <cmath>
#include <GL/gl.h>
#include <iostream>

namespace Alglib {

    class Mat2 {
    public:
        GLfloat m[3][3];

        Mat2();
        static Mat2 Identity();
        Mat2 Copy() const;
        Mat2& Replace(const Mat2& mat);
        Mat2 MatrixMultiply(const Mat2& a) const;

        Mat2& Scale(GLfloat sx, GLfloat sy);
        Mat2& Rotate(GLfloat theta);      // theta em graus
        Mat2& Translate(GLfloat dX, GLfloat dY);

        void Print() const;
    };

    class Tuple2{
    public:
        GLfloat x;
        GLfloat y;
        GLfloat w;


        Tuple2() : x(0.0f), y(0.0f), w(1.0f) {}
        Tuple2(GLfloat vx, GLfloat vy, GLfloat vw = 1.0f) : x(vx), y(vy), w(vw) {}
        Tuple2(GLfloat vw) : x(0.0f), y(0.0f), w(vw) {}

        void Print() const;

        Tuple2& Transform(const Mat2& m);

        GLfloat GetX() const { return x; }
        GLfloat GetY() const { return y; }
        GLfloat GetW() const { return w; }
    };

    Tuple2 Subtract(const Tuple2& t1, const Tuple2& t2);

    Tuple2 Add(const Tuple2& t1, const Tuple2& t2);
}

#endif