#include "axis.h"
#include <QFont>
#include <QFontMetrics>
#include <QPainter>
#include <QFontDatabase>
#include <QtMath>

const float xLet[] = {
    -0.1, -0.2, 0,
    0.1, 0.2, 0,
    0.1, -0.2, 0,
    -0.1, 0.2, 0
};
const float yLet[] = {
    0, -0.2, 0,
    0, 0, 0,
    0, 0, 0,
    0.1, 0.2, 0,
    0, 0, 0,
    -0.1, 0.2, 0
};
const float zLet[] = {
    -0.1, -0.2, 0,
    0.1, -0.2, 0,
    0.1, -0.2, 0,
    -0.1, 0.2, 0,
    -0.1, 0.2, 0,
    0.1, 0.2, 0
};
const int axisSegCount[] = {2, 3, 3};
const float* axisLabels[] = {xLet, yLet, zLet};

using couleur = QVector3D;
const couleur blanc = {1.0, 1.0, 1.0};
const couleur noir = {0.0, 0.0, 0.0};
const couleur rouge = {1.0, 0.0, 0.0};
const couleur vert = {0.0, 1.0, 0.0};
const couleur bleu = {0.0, 0.0, 1.0};

struct point {
    QVector3D pos;
    couleur color;
};

const point orig = {QVector3D(0.0f,0.0f,0.0f),blanc};

struct ligne {
    point a, b;
};


Axis::Axis()
{
    initializeOpenGLFunctions();
    vao.create();
    QOpenGLVertexArrayObject::Binder bind(&vao);

    shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/gl/colored_lines.vert");
    shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/gl/colored_lines.frag");
    shader.link();
    const int ptSize = 6*sizeof(float);
    for(int lIdx = 0; lIdx < 3; lIdx++)
    {
        const float* l = axisLabels[lIdx];
        const int ptCount = axisSegCount[lIdx]*2;
        float c[3] = {0.3f,0.3f,0.3f};
        c[lIdx] = 1.0;//set color
        QOpenGLBuffer b = flowerLabelVertices[lIdx];
        b.create();
        b.bind();
        b.allocate(ptCount*ptSize);
        for(int pIdx = 0; pIdx < ptCount; pIdx++)
        {
            b.write(pIdx*ptSize, &(l[pIdx*3]), ptSize/2);//write coords
            b.write(pIdx*ptSize + ptSize/2, c, ptSize/2);//write color
        }
        b.release();
    }
    //Axis buffer: 6 floats per vertex, 2 vert per line, 3 lines
    //float aBuf[6*2*3];
    float baseCol = 0.3f;
    float aBuf[6*2*3] = {0.0,0.0,0.0,1.0,baseCol,baseCol,
                         1.0,0.0,0.0,1.0,baseCol,baseCol,
                         0.0,0.0,0.0,baseCol,1.0,baseCol,
                         0.0,1.0,0.0,baseCol,1.0,baseCol,
                         0.0,0.0,0.0,baseCol,baseCol,1.0,
                         0.0,0.0,1.0,baseCol,baseCol,1.0};

    //The lines which form the 'axis-flower' in the corner
    flowerAxisVertices.create();
    flowerAxisVertices.bind();
    flowerAxisVertices.allocate(aBuf, sizeof(aBuf));
    flowerAxisVertices.release();
    // Rules
    // in constructor only two points  at origin
    rulesVertices.create();
    rulesVertices.bind();
    QVector<ligne> l = {
        {orig,{QVector3D(1.0,0.0,0.0),noir}}
    };
    //qDebug() << sizeof(point) << sizeof(ligne);
    rulesVertices.allocate(l.data(),l.size()*sizeof(ligne));

    // Init labels
    labelShader.addShaderFromSourceFile(QOpenGLShader::Vertex,":/gl/labels.vert");
    labelShader.addShaderFromSourceFile(QOpenGLShader::Fragment,":/gl/labels.frag");
    labelShader.link();

    // unit quad centered on origin
    // actual size will be defined in the shader
    static const float corners[] = { -.5f,-.5f,  .5f,-.5f,  -.5f,.5f,  .5f,.5f };

    quadVao.create();
    QOpenGLVertexArrayObject::Binder b(&quadVao); // (auto release vao)
    quadBuffer.create();
    quadBuffer.bind();
    quadBuffer.allocate(corners, sizeof(corners));
    const GLuint ac = labelShader.attributeLocation("corner_position");
    glEnableVertexAttribArray(ac);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    quadBuffer.release();

    // get max line width available
    GLfloat range[2];
    glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, range);
    maxLineWidth = range[1];
}


void Axis::addLabel(const QString &text, const QVector3D &pos, const QColor &color,float angleDeg)
{
    QFont font = QFontDatabase::systemFont(QFontDatabase::GeneralFont);
    font.setPixelSize(48);
    font.setBold(true);
    //qDebug() << QFontInfo(font).family();
    QFontMetrics fm(font);
    QImage img(fm.horizontalAdvance(text) + 8, fm.height() + 8, QImage::Format_ARGB32_Premultiplied);
    img.fill(Qt::transparent);
    QPainter p(&img);
    p.setRenderHint(QPainter::TextAntialiasing);
    p.setFont(font);
    p.setPen(color);
    p.drawText(4, 4 + fm.ascent(), text);
    p.end();

    // text is written into a texture
    // this texture is saved in the Label struct
    // with needed informations, position, aspect ratio and angle
    Label l;
    l.tex = QSharedPointer<QOpenGLTexture>::create(img.convertToFormat(QImage::Format_RGBA8888));
    l.tex->setMinMagFilters(QOpenGLTexture::LinearMipMapLinear, QOpenGLTexture::Linear);
    l.tex->setWrapMode(QOpenGLTexture::ClampToEdge);
    l.pos = pos;
    l.aspect = float(img.width()) / img.height();
    l.angle = qDegreesToRadians(angleDeg);
    labels.append(l);
}


void Axis::setScale(const QVector3D &min, const QVector3D &max)
{
    labels.clear(); // reset rule labels
    //Max function. not worth importing <algorithm> just for max
    auto Max = [](float a, float b)
    {
        return (a > b) ? a : b;
    };
    //This is how much the axes extend beyond the model
    //We want it to be dependent on the model's size, but uniform on all axes.
    const float axismargin = 0.25*Max(Max(max[0]-min[0], max[1]-min[1]), max[2]-min[2]);
    const float dob = axismargin / 2.0f; // distance to object
    const float ext = axismargin / 8.0f; // Extremas
    const float mid = axismargin / 10.0f;  // middle bar
    const float qua = axismargin / 12.0f; // quarter

    rulesVertices.bind();
    float mean[3];
    for (int i=0; i<3;i++) {
        mean[i] = (min[i] + max[i]) / 2.0f;
    }
    QVector<ligne> r;
    for (int i=0; i < 3; i++) {
        couleur axColor = {0.3f,0.3f,0.3f};
        axColor[i] = 1.0f;
        int idxMean = (i+2)%3;
        int idxDep = (i+1)%3;
        // main rule min --- max
        point rulePa = orig;
        rulePa.color = axColor;
        rulePa.pos[i] = min[i];
        rulePa.pos[idxMean] = mean[idxMean];
        rulePa.pos[idxDep] = max[idxDep] + dob;
        point rulePb = rulePa;
        rulePb.pos[i] = max[i];
        ligne ruleI = {rulePa,rulePb};
        //
        r.append(ruleI);
        // label = rule length at point lp
        QVector3D lp = rulePa.pos;
        lp[i] = mean[i];
        lp[idxDep] = max[idxDep] + dob + ext;
        // rotate text on Z
        addLabel(QString::number(max[i] - min[i], 'f', 2), lp,
                 QColor::fromRgbF(axColor.x(), axColor.y(), axColor.z()),
                 i == 2 ? 90.0f : 0.0f);
        // ext 1
        point barP1a = rulePa;
        barP1a.pos[idxDep] = max[idxDep] + dob - ext;
        point barP1b = rulePa;
        barP1b.pos[idxDep] = max[idxDep] + dob + ext;
        ligne bar1 = {barP1a,barP1b};
        r.append(bar1);
        // ext 2
        point barP2a = rulePb;
        barP2a.pos[idxDep] = max[idxDep] + dob - ext;
        point barP2b = rulePb;
        barP2b.pos[idxDep] = max[idxDep] + dob + ext;
        ligne bar2 = {barP2a,barP2b};
        r.append(bar2);
        // middle
        point midPa = rulePa;
        midPa.pos[i] = mean[i];
        point midPb = midPa;
        midPb.pos[idxDep] = max[idxDep] + dob - mid;
        ligne midLig ={midPa,midPb};
        r.append(midLig);
        // quarter
        for (int q = -1; q < 2; q+=2) {
            point qPa =  midPa;
            qPa.pos[i] = mean[i] + q * (max[i] - mean[i]) / 2.0f;
            point qPb = qPa;
            qPb.pos[idxDep] = max[idxDep] + dob - qua;
            ligne ligQua = {qPa,qPb};
            r.append(ligQua);
        }

    }
    rulesVertices.allocate(r.data(),r.size()*sizeof(ligne));
    rulesVertices.release();
}


void Axis::drawLabels(const QMatrix4x4 &transMat, const QMatrix4x4 &viewMat, float aspectRatio)
{
    if (labels.empty()) return;

    // For a fixed height label whatever the port
    // GLint vp[4];
    // glGetIntegerv(GL_VIEWPORT, vp);
    // const float heightPx = 30.0f;
    // const float hNdc = 2.0f * heightPx / vp[3]; // Fixed height whatever viewport size.

    // For a label normalize by the port height
    const float hNdc = 0.1f;   // text height in normalized coordinates (screen = 2)

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    labelShader.bind();
    labelShader.setUniformValue("transform_matrix", transMat);
    labelShader.setUniformValue("view_matrix", viewMat);
    labelShader.setUniformValue("label_texture", 0); // we only use first opengl texture slot (0) in our case.
    labelShader.setUniformValue("aspect_ratio", aspectRatio);

    QOpenGLVertexArrayObject::Binder b(&quadVao);
    for (const Label &l : labels) {
        labelShader.setUniformValue("label_position", l.pos);
        labelShader.setUniformValue("label_size", QVector2D(hNdc * l.aspect, hNdc)); // (w,h)
        labelShader.setUniformValue("offset_y", hNdc * 0.75f);
        labelShader.setUniformValue("label_angle", l.angle);
        l.tex->bind(0); // bind texture slot 0
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}

void Axis::draw(const QMatrix4x4 &transMat, const QMatrix4x4 &viewMat,
    const QMatrix4x4 &orientMat, const QMatrix4x4 &aspectMat, float aspectRatio)
{
    QOpenGLVertexArrayObject::Binder bind(&vao); // (automatic release)

    shader.bind();

    glLineWidth(qMin(2.0f,maxLineWidth));

    // Load the transform and view matrices into the shader
    auto loadMatrixUniforms = [&](QMatrix4x4 transform, QMatrix4x4 view)
    {
        glUniformMatrix4fv(
                    shader.uniformLocation("transform_matrix"),
                    1, GL_FALSE, transform.data());
        glUniformMatrix4fv(
                    shader.uniformLocation("view_matrix"),
                    1, GL_FALSE, view.data());
    };
    const GLuint vp = shader.attributeLocation("vertex_position");
    const GLuint vc = shader.attributeLocation("vertex_color");
    glEnableVertexAttribArray(vp);
    glEnableVertexAttribArray(vc);
    auto loadAttribPtr = [&]()
    {
        glVertexAttribPointer(vp, 3, GL_FLOAT, false,
                        6 * sizeof(GLfloat), 0);
        glVertexAttribPointer(vc, 3, GL_FLOAT, false,
                        6 * sizeof(GLfloat),
                        (GLvoid*)(3 * sizeof(GLfloat)));
    };
    loadMatrixUniforms(transMat, viewMat);
    loadAttribPtr();
    // en fait ce devrait être 3 (lignes) * 2 (sommets par ligne) = 6
    //qDebug() << "vertices" << vertices.size()/sizeof(point) << "sommets";
    //glDrawArrays(GL_LINES, 0, 6);
    //vertices.release();

    rulesVertices.bind();
    loadMatrixUniforms(transMat, viewMat);
    loadAttribPtr();
    //qDebug() << rulesVertices.size();
    int np = rulesVertices.size()/sizeof(point);
    glDrawArrays(GL_LINES, 0, np);
    rulesVertices.release();


    drawLabels(transMat, viewMat, aspectRatio);
    vao.bind();
    shader.bind();   // drawLabels a changé de programme

    //Next, we draw the hud axis-flower
    flowerAxisVertices.bind();
    glClear(GL_DEPTH_BUFFER_BIT);//Ensure hud draws over everything
    const float hudSize = 0.2;
    QMatrix4x4 hudMat;
    //Move the hud to the bottom left corner with margin
    if (aspectRatio > 1.0)
    {
        hudMat.translate(aspectRatio-2*hudSize, -1.0+2*hudSize, 0);
    }
    else
    {
        hudMat.translate(1.0-2*hudSize, -1.0/aspectRatio+2*hudSize, 0);
    }
    //Scale the hud to be small
    hudMat.scale(hudSize, hudSize, 1);
    loadMatrixUniforms(orientMat, aspectMat*hudMat);
    loadAttribPtr();
    glDrawArrays(GL_LINES, 0, 3*6);
    flowerAxisVertices.release();
    for(int aIdx = 0; aIdx < 3; aIdx++){
        QVector3D transVec = QVector3D();
        transVec[aIdx] = 1.25;//This is how far we want the letters to be extended out
        QOpenGLBuffer b = flowerLabelVertices[aIdx];
        //The only transform we want is to translate the letters to the ends of the axis lines
        QMatrix4x4 labelTransMat = QMatrix4x4();
        labelTransMat.translate(orientMat.map(transVec));
        b.bind();
        loadMatrixUniforms(labelTransMat, aspectMat * hudMat);
        loadAttribPtr();
        glDrawArrays(GL_LINES, 0, axisSegCount[aIdx]*2*6);
        b.release();
    }
    shader.release();
}
