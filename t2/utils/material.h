// Simplified Material class (declaration only)
#ifndef MATERIAL_H
#define MATERIAL_H

#include <GL/gl.h>

// Minimal material: construct with RGB only. Implementations live in material.cpp
class Material {
public:
    // Construct with base color (R,G,B). Optional specular intensity and shininess.
    Material(GLfloat R = 0.8f, GLfloat G = 0.8f, GLfloat B = 0.8f,
             GLfloat specIntensity = 1.0f, GLfloat shin = 80.0f);

    // set a new base color after construction
    void SetColor(GLfloat R, GLfloat G, GLfloat B);

    // Apply the material to OpenGL (glMaterialfv)
    void Apply(GLenum face = GL_FRONT_AND_BACK) const;

    ~Material() = default;

private:
    GLfloat emission[4];
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat shininess[1];
};

#endif // MATERIAL_H