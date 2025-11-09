#include "backdrop.h"

Backdrop::Backdrop()
{
    tl = QColor();
    tr = QColor();
    bl = QColor();
    br = QColor();

    initializeOpenGLFunctions();

    shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/gl/quad.vert");
    shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/gl/quad.frag");
    shader.link();

    constexpr float vbuf[] = {
        -1.f, -1.f, 0.f, 0.f, // BL
        -1.f, 1.f, 0.f, 1.f, // TL
        1.f, -1.f, 1.f, 0.f, // BR
        1.f, 1.f, 1.f, 1.f // TR
    };

    vertices.create();
    vertices.bind();
    vertices.allocate(vbuf, sizeof(vbuf));
    vertices.release();
}

void Backdrop::setColors(const QColor& topLeft, const QColor& topRight,
                         const QColor& bottomLeft, const QColor& bottomRight)
{
    setTopLeft(topLeft);
    setTopRight(topRight);
    setBottomLeft(bottomLeft);
    setBottomRight(bottomRight);
}

void Backdrop::setTopLeft(const QColor& color)
{
    tl = color;
}

void Backdrop::setTopRight(const QColor& color)
{
    tr = color;
}

void Backdrop::setBottomLeft(const QColor& color)
{
    bl = color;
}

void Backdrop::setBottomRight(const QColor& color)
{
    br = color;
}

void Backdrop::draw()
{
    shader.bind();
    vertices.bind();

    const GLint vp = shader.attributeLocation("vertex_position");
    const GLint vt = shader.attributeLocation("vertex_uv");

    glEnableVertexAttribArray(vp);
    glEnableVertexAttribArray(vt);
    glVertexAttribPointer(vp, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
    glVertexAttribPointer(vt, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(2 * sizeof(GLfloat)));

    auto toVec3 = [](const QColor& c) {
        return QVector3D{
            static_cast<float>(c.redF()),
            static_cast<float>(c.greenF()),
            static_cast<float>(c.blueF())
        };
    };

    shader.setUniformValue("colorTL", toVec3(tl));
    shader.setUniformValue("colorTR", toVec3(tr));
    shader.setUniformValue("colorBL", toVec3(bl));
    shader.setUniformValue("colorBR", toVec3(br));

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    vertices.release();
    shader.release();
}
