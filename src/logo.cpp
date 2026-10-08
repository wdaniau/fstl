#include "logo.h"
#include <QFileInfo>
#include <QImageReader>
#include <QSettings>


const QString Logo::LOGO_USEDEFAULT = "Logo/useDefault";
const QString Logo::LOGO_FILEPATH = "Logo/filePath";
const QString Logo::LOGO_RESIZEHEIGHT = "Logo/resizeHeight";
const QString Logo::LOGO_NEEDRESIZE = "Logo/needResize";
const QString Logo::LOGO_POSITION = "Logo/position";

Logo::Logo(QObject *parent) :QObject(parent) {

    // persistent settings
    QSettings settings;
    useDefault = settings.value(LOGO_USEDEFAULT, true).value<bool>();
    filePath = settings.value(LOGO_FILEPATH, QString("")).value<QString>();
    resizeHeight = settings.value(LOGO_RESIZEHEIGHT, 64).value<int>();
    needResize = settings.value(LOGO_NEEDRESIZE, false).value<bool>();
    position = static_cast<LogoPosition>(settings.value(LOGO_POSITION, static_cast<int>(LogoPosition::bottomLeft)).toInt());
    update();
}

void Logo::update() {
    if (useDefault) {
        resetToDefault();
        return;
    }
    if (needReload) {
        bool st = loadFromFile();
        if (!st) {
            resetToDefault();
            return;
        } else {
            emit(logoChanged());
        }
    }
    if (needResize) {
        if (resizeHeight <=0) {
            qWarning("resize height invalid");
        } else {
            current = original.scaledToHeight(resizeHeight,Qt::SmoothTransformation);
            emit(logoChanged());
        }
    } else {
        current = original;
        emit(logoChanged());
    }
}

void Logo::resetToDefault() {
    QSettings settings;
    useDefault = true;
    settings.setValue(LOGO_USEDEFAULT,true);
    QImage img = QImage(defaultResource);
    original = img;
    current = img;
    emit(logoChanged());
}

bool Logo::loadFromFile() {
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
    original = img;
    current = img;
    needReload =false;
    return true;
}

void Logo::setFilePath(const QString& f) {
    QSettings settings;
    settings.setValue(LOGO_FILEPATH,f);
    filePath = f;
    needReload = true;
    update();
}

void Logo::setDefault(bool b) {
    QSettings settings;
    settings.setValue(LOGO_USEDEFAULT,b);
    useDefault = b;
    needReload = true;
    update();
}

void Logo::setNeedResize(bool b) {
    QSettings settings;
    settings.setValue(LOGO_NEEDRESIZE, b);
    needResize = b;
    update();
}

void Logo::setResizeHeight(int i) {
    QSettings settings;
    settings.setValue(LOGO_RESIZEHEIGHT,i);
    resizeHeight = i;
    update();
}
void Logo::setPosition(LogoPosition p) {
    QSettings settings;
    settings.setValue(LOGO_POSITION,static_cast<int>(p));
    position = p;
    emit(logoChanged());
}

