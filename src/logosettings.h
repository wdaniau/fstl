#ifndef LOGOSETTINGS_H
#define LOGOSETTINGS_H

#include "logo.h"
#include <QWidget>
#include "canvas.h"
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>

class LogoSettings : public QWidget
{
    Q_OBJECT
public:
    LogoSettings(QWidget* parent, Canvas* _canvas);

signals:
private slots:
    void browseFile();
    void updatePreview();
    void updateEnabledState();

private:
    void getPix(const QString& path);
    Canvas* canvas;
    QCheckBox* drawLogoCheckbox;
    QCheckBox* useDefaultCheck;
    QComboBox* positionCombo;
    QGroupBox* customGroup;
    QLineEdit* filePathEdit;
    QPushButton* browseButton;
    QLabel* previewLabel;
    QLabel* infoLabel;
    QCheckBox* needResizeCheck;
    QLineEdit* resizeHeightEdit;

    QPixmap pixmap;
    QString positionToString(LogoPosition position);

};

#endif // LOGOSETTINGS_H
