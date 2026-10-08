#ifndef WATERMARKSETTINGS_H
#define WATERMARKSETTINGS_H

#include <QWidget>
#include "canvas.h"
#include <QCheckBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "watermark.h"

class WatermarkSettings : public QWidget
{
    Q_OBJECT
public:
    WatermarkSettings(QWidget *parent, Canvas* _canvas);

private slots:
    void browseFile();
    void updatePreview();
    void updateEnabledState();

private:
    void getPix(const QString& path);
    Canvas* canvas;

    QCheckBox* drawWatermarkCheckbox;
    QCheckBox* useTextCheck;

    QGroupBox* textGroup;
    QLineEdit* textEdit;

    QGroupBox* imageGroup;
    QLineEdit* filePathEdit;
    QPushButton* browseButton;
    QLabel* previewLabel;
    QLabel* infoLabel;

    QPixmap pixmap;
};

#endif // WATERMARKSETTINGS_H
