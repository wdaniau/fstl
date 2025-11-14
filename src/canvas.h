#ifndef CANVAS_H
#define CANVAS_H

#include <QtOpenGL>
#include <QSurfaceFormat>
#include <QOpenGLShaderProgram>

class GLMesh;
class Mesh;
class Backdrop;
class Axis;

enum DrawMode {shaded, wireframe, surfaceangle, meshlight, DRAWMODECOUNT};

class Canvas : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit Canvas(QSurfaceFormat format, QWidget* parent=0);
    ~Canvas();

    const static float P_PERSPECTIVE;
    const static float P_ORTHOGRAPHIC;

    void view_perspective(float p, bool animate);
    void draw_axes(bool d);
    void invert_zoom(bool d);
    void set_drawMode(enum DrawMode mode);
    void setResetTransformOnLoad(bool d);

    QColor getAmbientColor();
    void setAmbientColor(QColor c);
    double getAmbientFactor();
    void setAmbientFactor(double f);
    void resetAmbientColor();

    QColor getDirectiveColor();
    void setDirectiveColor(QColor c);
    double getDirectiveFactor();
    void setDirectiveFactor(double f);
    void resetDirectiveColor();

    QList<QString> getNameDir();
    QList<QVector3D> getListDir();
    int getCurrentLightDirection();
    void setCurrentLightDirection(int ind);
    void resetCurrentLightDirection();

    bool getUseWire();
    void setUseWire(bool b);
    void resetUseWire();

    double getWireWidth();
    void setWireWidth(double w);
    void resetWireWidth();

    QColor getWireColor();
    void setWireColor(QColor c);
    void resetWireColor();

    QString getDefaultView();
    void setDefaultView(QString v);
    void recenterView();

    bool isFallbackGlsl();

    void resetView();
    void applyRotation(QString name);

    double getAbFactor();
    void setAbFactor(double f);
    void resetAbFactor();

    int getMsaa();
    void setMsaa(int m);
    void resetMsaa();
    void setBackdropCorners(const QColor& tl, const QColor& tr, const QColor& bl, const QColor& br);
    void setBackdropTLCorner(const QColor& color);
    void setBackdropTRCorner(const QColor& color);
    void setBackdropBLCorner(const QColor& color);
    void setBackdropBRCorner(const QColor& color);
    void setBackdropPresetIndex(int index);
    int getBackdropPresetIndex();

    QColor backdropTL;
    QColor backdropTR;
    QColor backdropBL;
    QColor backdropBR;

    QColor tlStandardBackdrop = QColor::fromRgbF(0.03137255f, 0.20784314f, 0.25882353f);
    QColor trStandardBackdrop = QColor::fromRgbF(0.05882353f, 0.25882353f, 0.29803922f);
    QColor blStandardBackdrop = QColor::fromRgbF(0.00000000f, 0.10196078f, 0.15294118f);
    QColor brStandardBackdrop = QColor::fromRgbF(0.00000000f, 0.12156863f, 0.18039216f);

    void loadBackdropFromSettings();

public slots:
    void set_status(const QString& s);
    void clear_status();
    void load_mesh(Mesh* m, bool is_reload);

protected:
    void paintGL() override;
    void initializeGL() override;
    void resizeGL(int width, int height) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

    void set_perspective(float p);
    void view_anim(float v);

signals:
    void fallbackGlslUpdated(bool b);

private:
    void draw_mesh();

    QMatrix4x4 orient_matrix() const;
    QMatrix4x4 transform_matrix() const;
    QMatrix4x4 aspect_matrix() const;
    QMatrix4x4 view_matrix() const;
    void resetTransform();
    QPointF changeMouseCoordinates(QPoint p);
    void calcArcballTransform(QPointF p1, QPointF p2);

    QOpenGLShader* mesh_vertshader;
    QOpenGLShaderProgram mesh_shader;
    QOpenGLShaderProgram mesh_wireframe_shader;
    QOpenGLShaderProgram mesh_surfaceangle_shader;
    QOpenGLShaderProgram mesh_meshlight_shader;

    QColor ambientColor;
    QColor directiveColor;
    float ambientFactor;
    float directiveFactor;
    QList<QString> nameDir;
    QList<QVector3D> listDir;
    int currentLightDirection;
    bool useWire;
    float wireWidth;
    QColor wireColor;
    bool fallbackGlsl;

    QHash<QString,QList<float>> predefinedRotations;
    QString defaultView;

    const static QColor defaultAmbientColor;
    const static QColor defaultDirectiveColor;
    const static double defaultAmbientFactor;
    const static double defaultDirectiveFactor;
    const static int defaultCurrentLightDirection;
    const static bool defaultUseWire;
    const static double defaultWireWidth;
    const static QColor defaultWireColor;
    const static QString defaultDefaultView;
    const static double defaultAbFactor;
    const static int defaultMsaa;

    const static QString AMBIENT_COLOR;
    const static QString AMBIENT_FACTOR;
    const static QString DIRECTIVE_COLOR;
    const static QString DIRECTIVE_FACTOR;
    const static QString CURRENT_LIGHT_DIRECTION;
    const static QString USE_WIRE;
    const static QString WIRE_WIDTH;
    const static QString WIRE_COLOR;
    const static QString DEFAULT_VIEW;
    const static QString AB_FACTOR;
    const static QString MSAA;
    const static QString BACKDROP_TOP_LEFT;
    const static QString BACKDROP_TOP_RIGHT;
    const static QString BACKDROP_BOTTOM_LEFT;
    const static QString BACKDROP_BOTTOM_RIGHT;
    const static QString BACKDROP_PRESET_INDEX;

    GLMesh* mesh;
    Backdrop* backdrop;
    Axis* axis;

    QVector3D center;
    QVector3D centerOrg;
    float scale;
    float scaleOrg;
    float zoom;
    QMatrix4x4 currentTransform;
    float abFactor;
    int msaa;

    float perspective;
    enum DrawMode drawMode;
    bool drawAxes;
    bool invertZoom;
    bool resetTransformOnLoad;
    Q_PROPERTY(float perspective MEMBER perspective WRITE set_perspective);
    QPropertyAnimation anim;

    QPoint mouse_pos;
    QString status;
    QString meshInfo;
};

#endif // CANVAS_H
