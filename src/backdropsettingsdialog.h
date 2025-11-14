#ifndef BACKDROPSETTINGSDIALOG_H
#define BACKDROPSETTINGSDIALOG_H

#include <qcombobox.h>
#include <QDialog>

class Canvas;

class BackdropSettingsDialog final : public QDialog
{
    Q_OBJECT

public:
    BackdropSettingsDialog(QWidget* parent, Canvas* _canvas);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void moveEvent(QMoveEvent *event) override;

private slots:
    void onPresetChanged(int index);
    void onTLColorButtonClicked();
    void onTRColorButtonClicked();
    void onBLColorButtonClicked();
    void onBRColorButtonClicked();
    static void setCustomBackdropCorners(const QColor& tl, const QColor& tr, const QColor& bl, const QColor& br);
    void restoreCustomBackdropCorners() const;
    void applyCustomPreset() const;
    bool confirmCustomColorChange();

private:
    Canvas* canvas;
    QComboBox* comboBackdropPresets;
    QPushButton* buttonColorTL;
    QPushButton* buttonColorTR;
    QPushButton* buttonColorBL;
    QPushButton* buttonColorBR;
    const static QString BACKDROP_TOP_LEFT_CUSTOM;
    const static QString BACKDROP_TOP_RIGHT_CUSTOM;
    const static QString BACKDROP_BOTTOM_LEFT_CUSTOM;
    const static QString BACKDROP_BOTTOM_RIGHT_CUSTOM;
    const static QString SETTINGS_DIALOG_GEOMETRY;
};

#endif // BACKDROPSETTINGSDIALOG_H
