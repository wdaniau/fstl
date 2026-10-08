#include "watermark.h"
#include "canvas.h"

#include <QFileInfo>
#include <QImageReader>
#include <QSettings>


const QString Watermark::WATERMARK_USETEXT = "Watermark/useText";
const QString Watermark::WATERMARK_FILEPATH = "Watermark/filePath";
const QString Watermark::WATERMARK_TEXT = "Watermark/text";

Watermark::Watermark(Canvas* _canvas) : QObject(_canvas) {
    canvas = _canvas;
    // persistent settings
    QSettings settings;
    useText = settings.value(WATERMARK_USETEXT, true).value<bool>();
    filePath = settings.value(WATERMARK_FILEPATH, QString("")).value<QString>();
    text = settings.value(WATERMARK_TEXT, QString("Watermark")).value<QString>();
    update();
}

void Watermark::update() {
    if (useText) {
        if (needRender) {
            renderWatermarkText();
            emit(watermarkChanged());
        }
        return;
    }
    // use image
    if (needReload) {
        bool state = loadFromFile();
        if (!state) {
            setUseText(true);
            return;
        } else {
            emit(watermarkChanged());
        }
    }
}

void Watermark::setUseText(bool b) {
    QSettings settings;
    settings.setValue(WATERMARK_USETEXT,b);
    useText = b;
    needRender = b ? true : false;
    needReload = b ? false : true;
    update();
}

void Watermark::setText(const QString& t) {
    QSettings settings;
    settings.setValue(WATERMARK_TEXT, t);
    text = t;
    needRender = true;
    update();
}

bool Watermark::loadFromFile() {
    QFileInfo info(filePath);
    if (!info.exists() || !info.isFile()) {
        qWarning("Cannot read file");
        return false;
    }
    QImageReader reader(filePath);
    reader.setAutoTransform(true);
    QByteArray format = reader.format().toLower();
    if (format != "png" && format != "jpg" && format != "jpeg") {
        qWarning("Bad image format. Must be png or jpg");
        return false;
    }
    QImage img = reader.read();
    if (img.isNull()) {
        qWarning("Problem reading image.");
        return  false;
    }
    image = img;
    needReload =false;
    return true;
}

void Watermark::setFilePath(const QString& f) {
    QSettings settings;
    settings.setValue(WATERMARK_FILEPATH,f);
    filePath = f;
    needReload = true;
    update();
}

void Watermark::renderWatermarkText() {
    const qreal dpr = canvas->devicePixelRatioF();
    QImage img(canvas->size() * dpr, QImage::Format_ARGB32_Premultiplied);
    img.setDevicePixelRatio(dpr);
    img.fill(Qt::transparent);

    const qreal w = canvas->size().width();
    const qreal h = canvas->size().height();
    const qreal angle = qRadiansToDegrees(qAtan2(h, w));
    const qreal diag  = std::hypot(w, h);

    // Font will use 70% of diag
    QFont font = QFontDatabase::systemFont(QFontDatabase::GeneralFont);
    font.setBold(true);
    const qreal textW = QFontMetricsF(font).horizontalAdvance(text);
    font.setPointSizeF(font.pointSizeF() * (diag * 0.7) / textW);

    QPainter p(&img);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::TextAntialiasing);
    p.translate(w / 2, h / 2);
    p.rotate(-angle);
    p.setFont(font);
    p.setPen(QColor(255, 255, 255, 80));
    p.drawText(QRectF(-diag / 2, -diag / 2, diag, diag),
               Qt::AlignCenter, text);
    p.end();
    image = img;
    needRender = false;
}

void Watermark::setNeedRender(bool b) {
    needRender = b;
    if (needRender)
        update();
}
