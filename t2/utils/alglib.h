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

    // -------------------- 3D types --------------------

    class Mat3 {
    public:
        // 4x4 homogeneous matrix for 3D transforms
        GLfloat m[4][4];

        Mat3();
        static Mat3 Identity();
        Mat3 Copy() const;
        Mat3& Replace(const Mat3& mat);
        Mat3 MatrixMultiply(const Mat3& a) const;

        Mat3& Scale(GLfloat sx, GLfloat sy, GLfloat sz);
        // Rotate accepts degrees for rotations around X, Y and Z (applied X then Y then Z)
        Mat3& Rotate(GLfloat rx, GLfloat ry, GLfloat rz);
        Mat3& RotateX(GLfloat theta); // degrees
        Mat3& RotateY(GLfloat theta); // degrees
        Mat3& RotateZ(GLfloat theta); // degrees

        Mat3& Translate(GLfloat dX, GLfloat dY, GLfloat dZ);

        void Print() const;
    };

    class Tuple3 {
    public:
        GLfloat x;
        GLfloat y;
        GLfloat z;
        GLfloat w;

        Tuple3() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
        Tuple3(GLfloat vx, GLfloat vy, GLfloat vz, GLfloat vw = 1.0f) : x(vx), y(vy), z(vz), w(vw) {}
        Tuple3(GLfloat vw) : x(0.0f), y(0.0f), z(0.0f), w(vw) {}

        void Print() const;

        Tuple3& Transform(const Mat3& m);

        GLfloat GetX() const { return x; }
        GLfloat GetY() const { return y; }
        GLfloat GetZ() const { return z; }
        GLfloat GetW() const { return w; }
    };

    Tuple3 Subtract(const Tuple3& t1, const Tuple3& t2);

    Tuple3 Add(const Tuple3& t1, const Tuple3& t2);

}
#endif