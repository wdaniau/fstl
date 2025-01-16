#include "shaderlightprefs.h"
#include "canvas.h"
#include <QColorDialog>

const QString ShaderLightPrefs::PREFS_GEOM = "shaderPrefsGeometry";

ShaderLightPrefs::ShaderLightPrefs(QWidget *parent, Canvas *_canvas) : QDialog(parent)
{
    canvas = _canvas;

    QVBoxLayout* prefsLayout = new QVBoxLayout;
    this->setLayout(prefsLayout);

    QLabel* title = new QLabel("Shader preferences");
    QFont boldFont = QApplication::font();
    boldFont.setWeight(QFont::Bold);
    title->setFont(boldFont);
    title->setAlignment(Qt::AlignCenter);
    prefsLayout->addWidget(title);

    QWidget* middleWidget = new QWidget;
    QGridLayout* middleLayout = new QGridLayout;
    middleWidget->setLayout(middleLayout);
    this->layout()->addWidget(middleWidget);

    // labels
    middleLayout->addWidget(new QLabel("Ambient Color"),0,0);
    middleLayout->addWidget(new QLabel("Directive Color"),1,0);
    //middleLayout->addWidget(new QLabel("Direction"),2,0);

    QPixmap dummy(20, 20);

    dummy.fill(canvas->getAmbientColor());
    buttonAmbientColor = new QPushButton;
    buttonAmbientColor->setIcon(QIcon(dummy));
    middleLayout->addWidget(buttonAmbientColor,0,1);
    buttonAmbientColor->setFocusPolicy(Qt::NoFocus);
    connect(buttonAmbientColor,SIGNAL(clicked(bool)),this,SLOT(buttonAmbientColorClicked()));

    editAmbientFactor = new QLineEdit;
    editAmbientFactor->setValidator(new QDoubleValidator);
    editAmbientFactor->setText(QString("%1").arg(canvas->getAmbientFactor()));
    middleLayout->addWidget(editAmbientFactor,0,2,1,2);
    connect(editAmbientFactor,SIGNAL(editingFinished()),this,SLOT(editAmbientFactorFinished()));

    QPushButton* buttonResetAmbientColor = new QPushButton("Reset");
    middleLayout->addWidget(buttonResetAmbientColor,0,4);
    buttonResetAmbientColor->setFocusPolicy(Qt::NoFocus);
    connect(buttonResetAmbientColor,SIGNAL(clicked(bool)),this,SLOT(resetAmbientColorClicked()));


    dummy.fill(canvas->getDirectiveColor());
    buttonDirectiveColor = new QPushButton;
    buttonDirectiveColor->setIcon(QIcon(dummy));
    middleLayout->addWidget(buttonDirectiveColor,1,1);
    buttonDirectiveColor->setFocusPolicy(Qt::NoFocus);
    connect(buttonDirectiveColor,SIGNAL(clicked(bool)),this,SLOT(buttonDirectiveColorClicked()));

    editDirectiveFactor = new QLineEdit;
    editDirectiveFactor->setValidator(new QDoubleValidator);
    editDirectiveFactor->setText(QString("%1").arg(canvas->getDirectiveFactor()));
    middleLayout->addWidget(editDirectiveFactor,1,2,1,2);
    connect(editDirectiveFactor,SIGNAL(editingFinished()),this,SLOT(editDirectiveFactorFinished()w));

    QPushButton* buttonResetDirectiveColor = new QPushButton("Reset");
    middleLayout->addWidget(buttonResetDirectiveColor,1,4);
    buttonResetDirectiveColor->setFocusPolicy(Qt::NoFocus);
    connect(buttonResetDirectiveColor,SIGNAL(clicked(bool)),this,SLOT(resetDirectiveColorClicked()));

    // Fill in directions

    comboDirections = new QComboBox;
    comboDirections->setFocusPolicy(Qt::NoFocus);
    middleLayout->addWidget(comboDirections,2,1,1,3);
    comboDirections->addItems(canvas->getNameDir());
    comboDirections->setCurrentIndex(canvas->getCurrentLightDirection());
    connect(comboDirections,SIGNAL(currentIndexChanged(int)),this,SLOT(comboDirectionsChanged(int)));

    QPushButton* buttonResetDirection = new QPushButton("Reset");
    //middleLayout->addWidget(buttonResetDirection,2,4);
    middleLayout->addWidget(buttonResetDirection,3,4);
    buttonResetDirection->setFocusPolicy(Qt::NoFocus);
    connect(buttonResetDirection,SIGNAL(clicked(bool)),this,SLOT(resetDirection()));


    QFrame* radioWidget = new QFrame;
    radioWidget->setFrameStyle(QFrame::Raised | QFrame::Box);
    QGridLayout* radioWidgetLayout = new QGridLayout;
    radioWidget->setLayout(radioWidgetLayout);

    leftRight = new QButtonGroup;
    topBottom = new QButtonGroup;
    rearFront = new QButtonGroup;

    QRadioButton* radioLeftButton = new QRadioButton;
    radioLeftButton->setText("Left");
    QRadioButton* radioRightButton = new QRadioButton;
    radioRightButton->setText("Right");
    QRadioButton* radioLRNoneButton = new QRadioButton;
    radioLRNoneButton->setText("off");
    leftRight->addButton(radioLeftButton,2);
    leftRight->addButton(radioRightButton,0);
    leftRight->addButton(radioLRNoneButton,1);
    leftRight->button(0)->setChecked(true);


    radioWidgetLayout->addWidget(radioLeftButton,0,0);
    radioWidgetLayout->addWidget(radioRightButton,0,1);
    radioWidgetLayout->addWidget(radioLRNoneButton,0,2);

    QRadioButton* radioTopButton = new QRadioButton;
    radioTopButton->setText("Top");
    QRadioButton* radioBottomButton = new QRadioButton;
    radioBottomButton->setText("Bottom");
    QRadioButton* radioTBNoneButton = new QRadioButton;
    radioTBNoneButton->setText("off");
    topBottom->addButton(radioTopButton,0);
    topBottom->addButton(radioBottomButton,2);
    topBottom->addButton(radioTBNoneButton,1);
    topBottom->button(0)->setChecked(true);

    radioWidgetLayout->addWidget(radioTopButton,1,0);
    radioWidgetLayout->addWidget(radioBottomButton,1,1);
    radioWidgetLayout->addWidget(radioTBNoneButton,1,2);

    QRadioButton* radioRearButton = new QRadioButton;
    radioRearButton->setText("Rear");
    QRadioButton* radioFrontButton = new QRadioButton;
    radioFrontButton->setText("Front");
    QRadioButton* radioRFNoneButton = new QRadioButton;
    radioRFNoneButton->setText("off");
    rearFront->addButton(radioRearButton,0);
    rearFront->addButton(radioFrontButton,2);
    rearFront->addButton(radioRFNoneButton,1);
    rearFront->button(0)->setChecked(true);


    radioWidgetLayout->addWidget(radioRearButton,2,0);
    radioWidgetLayout->addWidget(radioFrontButton,2,1);
    radioWidgetLayout->addWidget(radioRFNoneButton,2,2);

    middleLayout->addWidget(radioWidget,3,1,3,3);

    connect(leftRight,SIGNAL(buttonClicked(int)),this,SLOT(radioSourceClicked(int)));
    connect(topBottom,SIGNAL(buttonClicked(int)),this,SLOT(radioSourceClicked(int)));
    connect(rearFront,SIGNAL(buttonClicked(int)),this,SLOT(radioSourceClicked(int)));
    setRadio(canvas->getCurrentLightDirection());

    QLabel* sourcePositionLabel = new QLabel("Light\nSource\nPosition");
    sourcePositionLabel->setAlignment(Qt::AlignCenter);
    middleLayout->addWidget(sourcePositionLabel,3,0);

    groupWireFrame = new QFrame;
    QGridLayout* groupWireFrameLayout = new QGridLayout;
    groupWireFrame->setLayout(groupWireFrameLayout);

    checkboxUseWireFrame = new QCheckBox("Add wireframe");
    checkboxUseWireFrame->setChecked(canvas->getUseWire());
    groupWireFrameLayout->addWidget(checkboxUseWireFrame,0,0);
    checkboxUseWireFrame->setFocusPolicy(Qt::NoFocus);
    connect(checkboxUseWireFrame,SIGNAL(stateChanged(int)),this,SLOT(checkboxUseWireFrameChanged()));

    QLabel* labelWireColor = new QLabel("Wire Color");
    groupWireFrameLayout->addWidget(labelWireColor,1,0);
    dummy.fill(canvas->getWireColor());
    buttonWireColor = new QPushButton;
    buttonWireColor->setIcon(QIcon(dummy));
    groupWireFrameLayout->addWidget(buttonWireColor,1,1);
    buttonWireColor->setFocusPolicy(Qt::NoFocus);
    QPushButton* buttonResetWireColor = new QPushButton("Reset");
    buttonResetWireColor->setFocusPolicy(Qt::NoFocus);
    groupWireFrameLayout->addWidget(buttonResetWireColor,1,3);
    connect(buttonWireColor,SIGNAL(clicked(bool)),this,SLOT(buttonWireColorClicked()));
    connect(buttonResetWireColor,SIGNAL(clicked(bool)),this,SLOT(resetWireColorClicked()));

    labelWireWidth = new QLabel(QString("Wire Width : %1").arg((int)canvas->getWireWidth()));
    groupWireFrameLayout->addWidget(labelWireWidth,2,0);
    sliderWireWidth = new QSlider(Qt::Horizontal);
    sliderWireWidth->setFocusPolicy(Qt::NoFocus);
    sliderWireWidth->setRange(1,10);
    sliderWireWidth->setTickPosition(QSlider::TicksBelow);
    sliderWireWidth->setSingleStep(1);
    sliderWireWidth->setPageStep(1);
    sliderWireWidth->setValue((int)canvas->getWireWidth());
    groupWireFrameLayout->addWidget(sliderWireWidth,2,1,1,2);
    connect(sliderWireWidth,SIGNAL(valueChanged(int)),this,SLOT(sliderWireWidthChanged()));
    QPushButton* buttonResetLineWidth = new QPushButton("Reset");
    buttonResetLineWidth->setFocusPolicy(Qt::NoFocus);
    groupWireFrameLayout->addWidget(buttonResetLineWidth,2,3);
    connect(buttonResetLineWidth,SIGNAL(clicked(bool)),this,SLOT(resetWireWidthClicked()));

    middleLayout->addWidget(groupWireFrame,6,0,3,5);

    // Ok button
    QWidget* boxButton = new QWidget;
    QHBoxLayout* boxButtonLayout = new QHBoxLayout;
    boxButton->setLayout(boxButtonLayout);
    QFrame *spacerL = new QFrame;
    spacerL->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding));
    QPushButton* okButton = new QPushButton("Ok");
    boxButtonLayout->addWidget(spacerL);
    boxButtonLayout->addWidget(okButton);
    this->layout()->addWidget(boxButton);
    okButton->setFocusPolicy(Qt::NoFocus);
    connect(okButton,SIGNAL(clicked(bool)),this,SLOT(okButtonClicked()));

    QSettings settings;
    if (!settings.value(PREFS_GEOM).isNull()) {
        restoreGeometry(settings.value(PREFS_GEOM).toByteArray());
    }

    connect(canvas,SIGNAL(fallbackGlslUpdated(bool)),this,SLOT(onFallbackGlslUpdated(bool)));
}

void ShaderLightPrefs::buttonAmbientColorClicked() {
    QColor newColor = QColorDialog::getColor(canvas->getAmbientColor(), this, QString("Choose color"),QColorDialog::DontUseNativeDialog);
    if (newColor.isValid() == true)
    {
        canvas->setAmbientColor(newColor);
        QPixmap dummy(20, 20);
        dummy.fill(canvas->getAmbientColor());
        buttonAmbientColor->setIcon(QIcon(dummy));
        canvas->update();
    }
}

void ShaderLightPrefs::editAmbientFactorFinished() {
    canvas->setAmbientFactor(editAmbientFactor->text().toDouble());
    canvas->update();
}

void ShaderLightPrefs::resetAmbientColorClicked() {
    canvas->resetAmbientColor();
    QPixmap dummy(20, 20);
    dummy.fill(canvas->getAmbientColor());
    buttonAmbientColor->setIcon(QIcon(dummy));
    editAmbientFactor->setText(QString("%1").arg(canvas->getAmbientFactor()));
    canvas->update();
}

void ShaderLightPrefs::buttonDirectiveColorClicked() {
    QColor newColor = QColorDialog::getColor(canvas->getDirectiveColor(), this, QString("Choose color"),QColorDialog::DontUseNativeDialog);
    if (newColor.isValid() == true)
    {
        canvas->setDirectiveColor(newColor);
        QPixmap dummy(20, 20);
        dummy.fill(canvas->getDirectiveColor());
        buttonDirectiveColor->setIcon(QIcon(dummy));
        canvas->update();
    }
}

void ShaderLightPrefs::editDirectiveFactorFinished() {
    canvas->setDirectiveFactor(editDirectiveFactor->text().toDouble());
    canvas->update();
}

void ShaderLightPrefs::resetDirectiveColorClicked() {
    canvas->resetDirectiveColor();
    QPixmap dummy(20, 20);
    dummy.fill(canvas->getDirectiveColor());
    buttonDirectiveColor->setIcon(QIcon(dummy));
    editDirectiveFactor->setText(QString("%1").arg(canvas->getDirectiveFactor()));
    canvas->update();
}

void ShaderLightPrefs::okButtonClicked() {
    this->close();
}

void ShaderLightPrefs::comboDirectionsChanged(int ind) {
    setRadio(ind);
    canvas->setCurrentLightDirection(ind);
    canvas->update();
}

void ShaderLightPrefs::resetDirection() {
    canvas->resetCurrentLightDirection();
    comboDirections->setCurrentIndex(canvas->getCurrentLightDirection());
    canvas->update();
}

void ShaderLightPrefs::resizeEvent(QResizeEvent *event)
{
    QSettings().setValue(PREFS_GEOM, saveGeometry());
    QWidget::resizeEvent(event);
}

void ShaderLightPrefs::moveEvent(QMoveEvent *event)
{
    QSettings().setValue(PREFS_GEOM, saveGeometry());
    QWidget::moveEvent(event);
}

void ShaderLightPrefs::checkboxUseWireFrameChanged() {
    bool state = checkboxUseWireFrame->isChecked();
    canvas->setUseWire(state);
    canvas->update();
}

void ShaderLightPrefs::buttonWireColorClicked() {
    QColor newColor = QColorDialog::getColor(canvas->getWireColor(), this, QString("Choose color"),QColorDialog::DontUseNativeDialog);
    if (newColor.isValid() == true)
    {
        canvas->setWireColor(newColor);
        QPixmap dummy(20, 20);
        dummy.fill(canvas->getWireColor());
        buttonWireColor->setIcon(QIcon(dummy));
        canvas->update();
    }
}

void ShaderLightPrefs::resetWireColorClicked() {
    canvas->resetWireColor();
    QPixmap dummy(20, 20);
    dummy.fill(canvas->getWireColor());
    buttonWireColor->setIcon(QIcon(dummy));
    canvas->update();
}

void ShaderLightPrefs::sliderWireWidthChanged() {
    int lw = sliderWireWidth->value();
    canvas->setWireWidth((double) lw);
    labelWireWidth->setText(QString("Wire Width : %1").arg(lw));
    canvas->update();
}

void ShaderLightPrefs::resetWireWidthClicked() {
    canvas->resetWireWidth();
    sliderWireWidth->setValue((int)canvas->getWireWidth());
}

void ShaderLightPrefs::onFallbackGlslUpdated(bool b) {
        groupWireFrame->setDisabled(b);
}

void ShaderLightPrefs::toggleUseWire() {
    // toggle if enable, no sense to do so otherwise
    if (checkboxUseWireFrame->isEnabled())
        checkboxUseWireFrame->toggle();
}

void ShaderLightPrefs::radioSourceClicked(int ind) {
    int pos = leftRight->checkedId() * 9 + topBottom->checkedId() * 3 + rearFront->checkedId();
    // Forbidden : 13 off,off,off
    if (pos == 13) {
        resetDirection();
        return;
    }
    pos = (pos >= 14) ? pos - 1 : pos;
    comboDirections->setCurrentIndex(pos);
    canvas->setCurrentLightDirection(pos);
    canvas->update();
}

void ShaderLightPrefs::setRadio(int ind) {
    leftRight->button(1 + QVariant(canvas->getListDir().at(ind).x()).toInt())->setChecked(true);
    topBottom->button(1 + QVariant(canvas->getListDir().at(ind).y()).toInt())->setChecked(true);
    rearFront->button(1 + QVariant(canvas->getListDir().at(ind).z()).toInt())->setChecked(true);
}
