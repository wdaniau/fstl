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
    float getOpacity() {
        return opacity;
    }
    const QColor& getTextColor() {
        return textColor;
    }
    void setUseText(bool b);
    void setFilePath(const QString& f);
    void setText(const QString& t);
    void setOpacity(float f);
    void setTextColor(const QColor& c);
    bool loadFromFile();
    void update();
    void setNeedRender(bool b);

    const static QString WATERMARK_USETEXT;
    const static QString WATERMARK_FILEPATH;
    const static QString WATERMARK_TEXT;
    const static QString WATERMARK_OPACITY;
    const static QString WATERMARK_TEXT_COLOR;

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
    float opacity = 0.3f;
    QColor textColor = QColor(Qt::white);

};

#endif // WATERMARK_H
