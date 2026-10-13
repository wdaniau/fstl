#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QTabWidget>
#include "canvas.h"
#include "window.h"


class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    SettingsDialog(Window* parent, Canvas* _canvas);
    Window* getWindow() {
        return window;
    }

protected:
    void resizeEvent(QResizeEvent *event) override;
    void moveEvent(QMoveEvent *event) override;

public slots:
    void onDrawModeChange(DrawMode m);

private slots:
    void okButtonClicked();

private:
    Window* window;
    QTabWidget* tabs;
    Canvas* canvas;
    QWidget* shaderPrefPage;

    const static QString PREFS_GEOM;
};

#endif // SETTINGSDIALOG_H
