#include "material.h"
#include <GL/gl.h>

// Implementation of the simple Material class.
Material::Material(GLfloat R, GLfloat G, GLfloat B, GLfloat specIntensity, GLfloat shin)
{
    // small self-emission to keep the color visible under lighting
    emission[0] = 0.06f * R; emission[1] = 0.06f * G; emission[2] = 0.06f * B; emission[3] = 1.0f;

    // ambient should be a fraction of base color
    ambient[0] = 0.35f * R; ambient[1] = 0.35f * G; ambient[2] = 0.35f * B; ambient[3] = 1.0f;

    // diffuse is the dominant component
    diffuse[0] = 0.9f * R; diffuse[1] = 0.9f * G; diffuse[2] = 0.9f * B; diffuse[3] = 1.0f;

    // white specular scaled by specIntensity
    specular[0] = specIntensity; specular[1] = specIntensity; specular[2] = specIntensity; specular[3] = 1.0f;

    shininess[0] = shin; // higher => tighter highlight
}

void Material::SetColor(GLfloat R, GLfloat G, GLfloat B)
{
    emission[0] = 0.02f * R; emission[1] = 0.02f * G; emission[2] = 0.02f * B; emission[3] = 1.0f;
    ambient[0]  = 0.2f * R;  ambient[1]  = 0.2f * G;  ambient[2]  = 0.2f * B;  ambient[3]  = 1.0f;
    diffuse[0]  = 0.8f * R;  diffuse[1]  = 0.8f * G;  diffuse[2]  = 0.8f * B;  diffuse[3]  = 1.0f;
}

void Material::Apply(GLenum face) const
{
    glMaterialfv(face, GL_EMISSION, emission);
    glMaterialfv(face, GL_AMBIENT,  ambient);
    glMaterialfv(face, GL_DIFFUSE,  diffuse);
    glMaterialfv(face, GL_SPECULAR, specular);
    glMaterialfv(face, GL_SHININESS, shininess);
}
