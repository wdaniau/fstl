#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QTabWidget>
#include "canvas.h"


class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    SettingsDialog(QWidget* parent, Canvas* _canvas);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void moveEvent(QMoveEvent *event) override;

public slots:
    void onDrawModeChange(DrawMode m);

private slots:
    void okButtonClicked();

private:
    QTabWidget* tabs;
    Canvas* canvas;
    QWidget* shaderPrefPage;

    const static QString PREFS_GEOM;
};

#endif // SETTINGSDIALOG_H
