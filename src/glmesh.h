#ifndef GLMESH_H
#define GLMESH_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>

// forward declaration
class Mesh;

class GLMesh : protected QOpenGLFunctions
{
public:
    GLMesh(const Mesh* const mesh);
    void draw(GLuint vp, GLuint bp);
private:
	QOpenGLBuffer vertices;
	QOpenGLBuffer indices;
    QOpenGLBuffer baryBuffer;
    int vertexCount;
    QOpenGLVertexArrayObject vao;
};

#endif // GLMESH_H
