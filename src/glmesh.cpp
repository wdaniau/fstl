#include "glmesh.h"
#include "mesh.h"

GLMesh::GLMesh(const Mesh* const mesh)
    : vertices(QOpenGLBuffer::VertexBuffer), indices(QOpenGLBuffer::IndexBuffer), baryBuffer(QOpenGLBuffer::VertexBuffer)
{
    initializeOpenGLFunctions();

    vertexCount = 0;

    vertices.create();
    indices.create();
    baryBuffer.create();

    vertices.setUsagePattern(QOpenGLBuffer::StaticDraw);
    indices.setUsagePattern(QOpenGLBuffer::StaticDraw);
    baryBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);

    std::vector<GLfloat> expandedVertices;
    std::vector<QVector3D> baryData;
    expandedVertices.reserve(mesh->indices.size());
    baryData.reserve(mesh->indices.size());

    static const QVector3D baryTable[3] = {
        {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}
    };

    for (size_t i = 0; i < mesh->indices.size(); i += 3) {
        for (int k = 0; k < 3; ++k) {
            GLuint idx = mesh->indices[i + k];
            expandedVertices.push_back(mesh->vertices[idx * 3 + 0]); // x
            expandedVertices.push_back(mesh->vertices[idx * 3 + 1]); // y
            expandedVertices.push_back(mesh->vertices[idx * 3 + 2]); // z
            baryData.push_back(baryTable[k]);
        }
    }

    vertexCount = int(expandedVertices.size() / 3);

    vertices.bind();
    vertices.allocate(expandedVertices.data(),
                      expandedVertices.size() * sizeof(GLfloat));
    vertices.release();

    baryBuffer.bind();
    baryBuffer.allocate(baryData.data(), int(baryData.size() * sizeof(QVector3D)));
    baryBuffer.release();

    indices.bind();
    indices.allocate(mesh->indices.data(),
                     mesh->indices.size() * sizeof(uint32_t));
    indices.release();
}

void GLMesh::draw(GLuint vp, GLuint bp)
{
    vertices.bind();

    glVertexAttribPointer(vp, 3, GL_FLOAT, false, 3*sizeof(float), NULL);
    vertices.release();

    baryBuffer.bind();
    glVertexAttribPointer(bp, 3, GL_FLOAT, false, 3*sizeof(float), NULL);
    baryBuffer.release();

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);

}
