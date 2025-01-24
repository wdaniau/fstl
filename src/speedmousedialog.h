#ifndef SPEEDMOUSEDIALOG_H
#define SPEEDMOUSEDIALOG_H

#include <QDialog>

class Canvas;
class QSlider;
class QStatusBar;

class SpeedMouseDialog : public QDialog
{
    Q_OBJECT
public:
    SpeedMouseDialog(QWidget* parent, Canvas* _canvas, QStatusBar* _sbar);

private:

    Canvas* canvas;
    QSlider* speedSlider;
    QStatusBar* sbar;
};

#endif // SPEEDMOUSEDIALOG_H
