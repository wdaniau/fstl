#ifndef ADDSHADERHEADER_H
#define ADDSHADERHEADER_H

#include <QByteArray>
#include <QOpenGLShader>
#include <QFile>
#include <QOpenGLContext>

inline QByteArray addShaderHeader(QOpenGLShader::ShaderTypeBit t, QString f) {
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

#endif // ADDSHADERHEADER_H
