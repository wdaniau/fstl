#include "speedmousedialog.h"
#include "canvas.h"
#include <QSlider>
#include <QStatusBar>

SpeedMouseDialog::SpeedMouseDialog(QWidget* parent, Canvas* _canvas, QStatusBar* _sbar) : QDialog(parent)

{
    canvas = _canvas;
    sbar = _sbar;
    // current value
    int currentFactor = (int) canvas->getAbFactor();
    int minValue = 1;
    int maxValue = 10;
    bool hasChanged = false;
    if (currentFactor < minValue) {
        currentFactor = minValue;
        hasChanged = true;
    }
    if (currentFactor > maxValue) {
        currentFactor = maxValue;
        hasChanged = true;
    }

    QVBoxLayout* speedMouseDialogLayout = new QVBoxLayout;
    this->setLayout(speedMouseDialogLayout);

    QLabel* pix = new QLabel;
    pix->setPixmap(QPixmap(":/qt/icons/speed_mouse_64x64.png").scaledToWidth(32));
    speedMouseDialogLayout->addWidget(pix);

    speedSlider = new QSlider(Qt::Vertical);
    speedSlider->setTickInterval(1);
    speedSlider->setTickPosition(QSlider::TicksRight);
    speedSlider->setFocusPolicy(Qt::NoFocus);
    speedSlider->setRange(minValue,maxValue);
    speedSlider->setValue(currentFactor);
    speedMouseDialogLayout->addWidget(speedSlider);
    QLabel* labelValue = new QLabel;
    labelValue->setAlignment(Qt::AlignCenter);
    labelValue->setText(QString("%1").arg(currentFactor));
    speedMouseDialogLayout->addWidget(labelValue);

    connect(speedSlider,&QSlider::valueChanged,[=](){
        labelValue->setText(QString("%1").arg(speedSlider->value()));
        canvas->setAbFactor((double) speedSlider->value());
        sbar->showMessage(QString("Speed Mouse changed to %1").arg(speedSlider->value()),2500);
    });
    connect(speedSlider,&QSlider::sliderPressed,[=] {
        sbar->showMessage(QString("Speed Mouse value : %1").arg(speedSlider->value()),2500);
    });

    if (hasChanged) {
        emit(speedSlider->valueChanged(currentFactor));
    }

    //this->setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);
    this->setWindowFlags(Qt::Popup);
    this->hide();
}
