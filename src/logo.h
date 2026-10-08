#ifndef LOGO_H
#define LOGO_H
#include <QObject>
#include <QImage>

enum LogoPosition {topLeft, topRight, bottomLeft, bottomRight, LOGOPOSITIONCOUNT};

class Logo : public QObject
{
    Q_OBJECT
public:
    Logo(QObject* parent);
    const QImage& getImage() {
        return current;
    }
    bool getDefault() {
        return useDefault;
    }
    void setDefault(bool b);
    const QString& getFilePath() {
        return filePath;
    }
    void setFilePath(const QString& f);
    bool loadFromFile();
    void resetToDefault();
    // bool resize(,Qt::AspectRatioMode mode = Qt::KeepAspectRatio,
    //             Qt::TransformationMode transformation = Qt::SmoothTransformation);
    void resetSize();
    void update();
    bool getNeedResize() {
        return needResize;
    }
    void setNeedResize(bool b);
    int getresizeHeight() {
        return resizeHeight;
    }
    void setResizeHeight(int i);
    LogoPosition getPosition() {
        return position;
    }
    void setPosition(LogoPosition p);

    const static QString LOGO_USEDEFAULT;
    const static QString LOGO_FILEPATH;
    const static QString LOGO_RESIZEHEIGHT;
    const static QString LOGO_NEEDRESIZE;
    const static QString LOGO_POSITION;

signals:
    void logoChanged();

private:
    QString defaultResource = QStringLiteral(":/qt/icons/fstl-e_64x64.png");
    QImage original;
    QImage current;
    bool useDefault = true;
    QString filePath = QStringLiteral("");
    int resizeHeight = 64;
    bool needResize = false;
    bool needReload = true;
    LogoPosition position = bottomLeft;

};

#endif // LOGO_H
