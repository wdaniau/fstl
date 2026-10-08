#ifndef WATERMARK_H
#define WATERMARK_H
#include <QObject>
#include <QImage>

class Canvas;

class Watermark : public QObject
{
    Q_OBJECT
public:
    Watermark(Canvas* _canvas);
    const QImage& getImage() {
        return image;
    }
    bool getUseText() {
        return useText;
    }
    const QString& getText() {
        return text;
    }
    const QString& getFilePath() {
        return filePath;
    }
    void setUseText(bool b);
    void setFilePath(const QString& f);
    void setText(const QString& t);
    bool loadFromFile();
    void update();
    void setNeedRender(bool b);

    const static QString WATERMARK_USETEXT;
    const static QString WATERMARK_FILEPATH;
    const static QString WATERMARK_TEXT;

signals:
    void watermarkChanged();

private:
    Canvas* canvas;
    void renderWatermarkText();
    bool useText = true;
    QString text = QStringLiteral("Watermark");
    QImage image;
    QString filePath = QStringLiteral("");
    bool needReload = true;
    bool needRender = true;

};

#endif // WATERMARK_H
