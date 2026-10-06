#ifndef AXIS_H
#define AXIS_H

#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>

class Axis : protected QOpenGLFunctions
{
public:
    Axis();
    void setScale(const QVector3D &min, const QVector3D &max);
    void draw(const QMatrix4x4 &transMat, const QMatrix4x4 &viewMat,
              const QMatrix4x4 &orientMat, const QMatrix4x4 &aspectMat, float aspectRatio);
private:
    QOpenGLShaderProgram shader;
    //QOpenGLBuffer vertices; //GL Buffer for model-space coords
    QOpenGLBuffer    flowerAxisVertices; //GL Buffer for hud-space axis lines
    QOpenGLBuffer flowerLabelVertices[3];//Buffer for hud-space label lines
    QOpenGLBuffer rulesVertices;
    QOpenGLVertexArrayObject vao;

    struct Label {
        QSharedPointer<QOpenGLTexture> tex; // (QSharedPointer -> automatic delete)
        QVector3D pos;      // position
        float aspect;       // aspect ratio
        float angle;        // radians
    };
    void addLabel(const QString &text, const QVector3D &pos, const QColor &color,float angleDeg = 0.0f);
    void drawLabels(const QMatrix4x4 &transMat, const QMatrix4x4 &viewMat, float aspectRatio);

    QVector<Label> labels;
    QOpenGLShaderProgram labelShader;
    QOpenGLBuffer quadBuffer;
    QOpenGLVertexArrayObject quadVao;

    GLfloat maxLineWidth;
};

#endif // AXIS_H
