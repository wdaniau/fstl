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

private slots:
    void onPresetChanged(int index) const;
    void onTLColorButtonClicked();
    void onTRColorButtonClicked();
    void onBLColorButtonClicked();
    void onBRColorButtonClicked();

private:
    Canvas* canvas;
    QComboBox* comboBackdropPresets;
    QPushButton* buttonColorTL;
    QPushButton* buttonColorTR;
    QPushButton* buttonColorBL;
    QPushButton* buttonColorBR;
};

#endif // BACKDROPSETTINGSDIALOG_H
