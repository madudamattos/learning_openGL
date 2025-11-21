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
        GLfloat d = theta * M_PI / 180;

        Mat2 rt = Identity();

        rt.m[0][0] = cos(d);
        rt.m[0][1] = -sin(d);
        rt.m[1][0] = sin(d);
        rt.m[1][1] = cos(d);
        
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

    Tuple2 Mat2::Transform(const Tuple2& t) const
    {
        Tuple2 r(0.0f);

        r.x = m[0][0] * t.x + m[0][1] * t.y + m[0][2] * t.w;
        r.y = m[1][0] * t.x + m[1][1] * t.y + m[1][2] * t.w;
        
        return r;
    }

    void Tuple2::Print() const
    {
        std::cout << "[" << this->x << " " << this->y << " " << this->w << "]" << std::endl;
    }




}

