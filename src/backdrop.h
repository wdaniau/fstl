#ifndef BACKDROP_H
#define BACKDROP_H

#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QColor>

class Backdrop : protected QOpenGLFunctions
{
public:
    Backdrop();
    void setColors(const QColor& topLeft,
                   const QColor& topRight,
                   const QColor& bottomLeft,
                   const QColor& bottomRight);
    void setTopLeft(const QColor& color);
    void setTopRight(const QColor& color);
    void setBottomLeft(const QColor& color);
    void setBottomRight(const QColor& color);
    QColor getTopLeft() const { return tl; }
    QColor getTopRight() const { return tr; }
    QColor getBottomLeft() const { return bl; }
    QColor getBottomRight() const { return br; }
    void draw();
private:
    QOpenGLShaderProgram shader;
    QOpenGLBuffer vertices;
    QColor tl, tr, bl, br;
};

#endif // BACKDROP_H
