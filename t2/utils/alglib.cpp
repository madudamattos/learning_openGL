#include "alglib.h"
#include <iostream>

namespace Alglib
{

    Mat2::Mat2()
    {
        for(int i=0; i<3; i++)
        {
            for(int j=0; j<3 ; j++)
            {
                if(i==j)
                    m[i][j] = 1.0f;
                else
                    m[i][j] = 0.0f;
            }

        }
    }

    Mat2 Mat2::Identity()
    {
        Mat2 I;

        for(int i=0; i< 3; i++)
        {
            for(int j=0; j<3 ; j++)
            {
                if(i==j)
                    I.m[i][j] = 1.0f;
                else
                    I.m[i][j] = 0.0f;
            }

        }

        return I;
    }

    Mat2 Mat2::Copy() const
    {
        return *this;
    }

    Mat2& Mat2::Replace(const Mat2& mat)
    {
        (*this).m[0][0]=mat.m[0][0]; (*this).m[0][1]=mat.m[0][1]; (*this).m[0][2]=mat.m[0][2];
        (*this).m[1][0]=mat.m[1][0]; (*this).m[1][1]=mat.m[1][1]; (*this).m[1][2]=mat.m[1][2];
        (*this).m[2][0]=mat.m[2][0]; (*this).m[2][1]=mat.m[2][1]; (*this).m[2][2]=mat.m[2][2];
        return *this;
    }


    Mat2 Mat2::MatrixMultiply(const Mat2& a) const
    {
        Mat2 mat = Identity();

        for(int i=0; i<3; ++i)
        {
            for(int j=0; j<3; ++j)
            {
                mat.m[i][j] = 0.0f;

                for(int k=0; k<3; ++k)
                {
                    mat.m[i][j] += m[i][k] * a.m[k][j];
                }
            }
        }

        return mat;
    }

    Mat2& Mat2::Scale(GLfloat sx, GLfloat sy)
    {
        Mat2 sc = Identity();

        sc.m[0][0] = sx;
        sc.m[1][1] = sy;

        *this = (*this).MatrixMultiply(sc);

        return *this;
    }

    // espera o angulo em graus
    Mat2& Mat2::Rotate(GLfloat theta)
    {
        // converte o angulo para radiano
        GLfloat d = theta * M_PI / 180.0f;

        Mat2 rt = Identity();

        rt.m[0][0] = cosf(d);
        rt.m[0][1] = -sinf(d);
        rt.m[1][0] = sinf(d);
        rt.m[1][1] = cosf(d);

        *this = (*this).MatrixMultiply(rt);

        return *this;
    }

    Mat2& Mat2::Translate(GLfloat dX, GLfloat dY)
    {
        Mat2 tl = Identity();

        tl.m[0][2] = dX;
        tl.m[1][2] = dY;

        *this = (*this).MatrixMultiply(tl);

        return *this;
    }

    void Mat2::Print() const
    {
        for(int i=0; i<3; i++)
        {
            std::cout << "[";
            for(int j=0; j<3; j++)
            {
                std::cout << m[i][j];
                if(j!=2) std::cout << " ";
            }

            std::cout << "]" << std::endl;
        }

        std::cout << "]" << std::endl;
    }


    Tuple2& Tuple2::Transform(const Mat2& mat)
    {
        Tuple2 r(0.0f);

        r.x = mat.m[0][0] * this->x + mat.m[0][1] * this->y + mat.m[0][2] * this->w;
        r.y = mat.m[1][0] * this->x + mat.m[1][1] * this->y + mat.m[1][2] * this->w;
        r.w = mat.m[2][0] * this->x + mat.m[2][1] * this->y + mat.m[2][2] * this->w;

        (*this) = r;

        return (*this);
    }

    Tuple2 Subtract(const Tuple2& t1, const Tuple2& t2)
    {
        Tuple2 r;
        r.x = t1.x - t2.x;
        r.y = t1.y - t2.y;
        r.w = 0.0f; 
        return r;
    }

    Tuple2 Add(const Tuple2& t1, const Tuple2& t2)
    {
        Tuple2 r;
        r.x = t1.x + t2.x;
        r.y = t1.y + t2.y;
        r.w = 0.0f; 
        return r;
    }

    void Tuple2::Print() const
    {
        std::cout << "[" << this->x << " " << this->y << " " << this->w << "]" << std::endl;
    }

    // -------------------- 3D implementations --------------------

    Mat3::Mat3()
    {
        for(int i=0;i<4;++i)
            for(int j=0;j<4;++j)
                m[i][j] = (i==j) ? 1.0f : 0.0f;
    }

    Mat3 Mat3::Identity()
    {
        Mat3 I;
        for(int i=0;i<4;++i)
            for(int j=0;j<4;++j)
                I.m[i][j] = (i==j) ? 1.0f : 0.0f;
        return I;
    }

    Mat3 Mat3::Copy() const
    {
        return *this;
    }

    Mat3& Mat3::Replace(const Mat3& mat)
    {
        for(int i=0;i<4;++i)
            for(int j=0;j<4;++j)
                m[i][j] = mat.m[i][j];
        return *this;
    }

    Mat3 Mat3::MatrixMultiply(const Mat3& a) const
    {
        Mat3 mat = Identity();

        for(int i=0;i<4;++i)
        {
            for(int j=0;j<4;++j)
            {
                mat.m[i][j] = 0.0f;
                for(int k=0;k<4;++k)
                {
                    mat.m[i][j] += m[i][k] * a.m[k][j];
                }
            }
        }

        return mat;
    }

    Mat3& Mat3::Scale(GLfloat sx, GLfloat sy, GLfloat sz)
    {
        Mat3 sc = Identity();
        sc.m[0][0] = sx;
        sc.m[1][1] = sy;
        sc.m[2][2] = sz;

        *this = (*this).MatrixMultiply(sc);
        return *this;
    }

    Mat3& Mat3::RotateX(GLfloat theta)
    {
        GLfloat d = theta * M_PI / 180.0f;
        Mat3 rx = Identity();
        rx.m[1][1] = cosf(d);
        rx.m[1][2] = -sinf(d);
        rx.m[2][1] = sinf(d);
        rx.m[2][2] = cosf(d);

        *this = (*this).MatrixMultiply(rx);
        return *this;
    }

    Mat3& Mat3::RotateY(GLfloat theta)
    {
        GLfloat d = theta * M_PI / 180.0f;
        Mat3 ry = Identity();
        ry.m[0][0] = cosf(d);
        ry.m[0][2] = sinf(d);
        ry.m[2][0] = -sinf(d);
        ry.m[2][2] = cosf(d);

        *this = (*this).MatrixMultiply(ry);
        return *this;
    }

    Mat3& Mat3::RotateZ(GLfloat theta)
    {
        GLfloat d = theta * M_PI / 180.0f;
        Mat3 rz = Identity();
        rz.m[0][0] = cosf(d);
        rz.m[0][1] = -sinf(d);
        rz.m[1][0] = sinf(d);
        rz.m[1][1] = cosf(d);

        *this = (*this).MatrixMultiply(rz);
        return *this;
    }

    // Apply rotations in X then Y then Z order
    Mat3& Mat3::Rotate(GLfloat rx, GLfloat ry, GLfloat rz)
    {
        this->RotateX(rx);
        this->RotateY(ry);
        this->RotateZ(rz);
        return *this;
    }

    Mat3& Mat3::Translate(GLfloat dX, GLfloat dY, GLfloat dZ)
    {
        Mat3 tl = Identity();
        tl.m[0][3] = dX;
        tl.m[1][3] = dY;
        tl.m[2][3] = dZ;

        *this = (*this).MatrixMultiply(tl);
        return *this;
    }

    void Mat3::Print() const
    {
        for(int i=0;i<4;++i)
        {
            std::cout << "[";
            for(int j=0;j<4;++j)
            {
                std::cout << m[i][j];
                if(j!=3) std::cout << " ";
            }
            std::cout << "]" << std::endl;
        }
    }

    Tuple3& Tuple3::Transform(const Mat3& mat)
    {
        Tuple3 r(0.0f);

        r.x = mat.m[0][0] * this->x + mat.m[0][1] * this->y + mat.m[0][2] * this->z + mat.m[0][3] * this->w;
        r.y = mat.m[1][0] * this->x + mat.m[1][1] * this->y + mat.m[1][2] * this->z + mat.m[1][3] * this->w;
        r.z = mat.m[2][0] * this->x + mat.m[2][1] * this->y + mat.m[2][2] * this->z + mat.m[2][3] * this->w;
        r.w = mat.m[3][0] * this->x + mat.m[3][1] * this->y + mat.m[3][2] * this->z + mat.m[3][3] * this->w;

        (*this) = r;
        return (*this);
    }

    Tuple3 Subtract(const Tuple3& t1, const Tuple3& t2)
    {
        Tuple3 r;
        r.x = t1.x - t2.x;
        r.y = t1.y - t2.y;
        r.z = t1.z - t2.z;
        r.w = 0.0f;
        return r;
    }

    Tuple3 Add(const Tuple3& t1, const Tuple3& t2)
    {
        Tuple3 r;
        r.x = t1.x + t2.x;
        r.y = t1.y + t2.y;
        r.z = t1.z + t2.z;
        r.w = 0.0f;
        return r;
    }

    void Tuple3::Print() const
    {
        std::cout << "[" << this->x << " " << this->y << " " << this->z << " " << this->w << "]" << std::endl;
    }

}

