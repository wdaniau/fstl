#ifndef ADDSHADERHEADER_H
#define ADDSHADERHEADER_H

#include <QByteArray>
#include <QOpenGLShader>
#include <QFile>
#include <QOpenGLContext>
#include <QOpenGLShaderProgram>

/*
This routine will read a file f containing a glsl shader without header,
then depending on the current context (OpenGL or OpenGL ES) and the shader type t
(Vertex or Fragment as Geometry does not exist in ES), it will prepend it
with the correct header.

OpenGL --> "#version 330 core", so OpenGL 3.3
OpenGL ES --> "#version 300 es\n", so OpenGL ES 3.0

There's a trick for noperspective as it does not exist in OpenGL ES.
If it is used in a shader one might use NOPERSPECTIVE in capital
instead of noperspective, so it will be safely ignored in ES.
Useless for this project specifically atm btw.
*/
inline QByteArray addShaderHeader(QOpenGLShader::ShaderTypeBit t, const QString& f) {
    bool es = QOpenGLContext::currentContext()->isOpenGLES();
    QByteArray header;
    if (es) {
        header += "#version 300 es\n";
        if (t == QOpenGLShader::Fragment) {
            header += "precision highp float;\nprecision highp int;\n";
        }
        header += "#define NOPERSPECTIVE\n";
    } else {
        header += "#version 330 core\n";
        header += "#define NOPERSPECTIVE noperspective\n";
    }
    QFile ff(f);
    ff.open(QIODevice::ReadOnly);
    QByteArray code = header + ff.readAll();
    ff.close();
    return code;
}

inline bool addHeaderlessShaderFromFile(QOpenGLShaderProgram& sh,QOpenGLShader::ShaderTypeBit t,const QString& f) {
    return sh.addShaderFromSourceCode(t,addShaderHeader(t,f));
}
#endif // ADDSHADERHEADER_H
