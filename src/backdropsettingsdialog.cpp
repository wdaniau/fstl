#include "backdropsettingsdialog.h"
#include "canvas.h"

namespace
{
    auto createColorPatch = [](const QColor& col)
    {
        QPixmap px(20, 20);
        px.fill(col);
        return QIcon(px);
    };
}

BackdropSettingsDialog::BackdropSettingsDialog(QWidget* parent, Canvas* _canvas) : QDialog(parent)
{
    canvas = _canvas;

    this->setMinimumWidth(400);

    auto* mainLayout = new QVBoxLayout(this);

    auto* title = new QLabel("Background Color Settings");
    QFont boldFont = QApplication::font();
    boldFont.setWeight(QFont::Bold);
    title->setFont(boldFont);
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    auto* mainGrid = new QGridLayout;
    mainGrid->setAlignment(Qt::AlignTop);

    auto* gridContainer = new QWidget;
    gridContainer->setLayout(mainGrid);
    mainLayout->addWidget(gridContainer, 0, Qt::AlignTop);

    mainLayout->addStretch();

    QLabel* presetLabel = new QLabel("Preset:");
    presetLabel->setFont(boldFont);
    presetLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    mainGrid->addWidget(presetLabel, 0, 0);

    comboBackdropPresets = new QComboBox;
    comboBackdropPresets->setFocusPolicy(Qt::NoFocus);
    comboBackdropPresets->addItem("Custom Colors", 0);
    comboBackdropPresets->addItem("Neutral", 1);
    comboBackdropPresets->addItem("Light Grey", 2);
    comboBackdropPresets->addItem("Blueprint", 3);
    comboBackdropPresets->addItem("Dark Studio", 4);
    comboBackdropPresets->addItem("Warm", 5);
    comboBackdropPresets->addItem("Neon Studio", 6);
    comboBackdropPresets->addItem("Sunset", 7);
    comboBackdropPresets->addItem("Cyber Tech", 8);
    comboBackdropPresets->addItem("Chocolate", 9);
    comboBackdropPresets->addItem("Extreme RGB", 10);
    comboBackdropPresets->setCurrentIndex(canvas->getBackdropPresetIndex());

    mainGrid->addWidget(comboBackdropPresets, 0, 1);
    connect(comboBackdropPresets,SIGNAL(currentIndexChanged(int)), this, SLOT(onPresetChanged(int)));

    canvas->loadBackdropFromSettings();

    const QString colorButtonStyleLeft = "QPushButton { text-align: left; padding: 8px;}";
    const QString colorButtonStyleRight = "QPushButton { text-align: left; padding: 8px;}";

    auto* colorLayoutTop = new QHBoxLayout;
    mainGrid->addLayout(colorLayoutTop, 1, 0, 1, 2); // spans 2 columns

    buttonColorTL = new QPushButton("Top Left");
    buttonColorTL->setStyleSheet(colorButtonStyleLeft);
    buttonColorTL->setIcon(createColorPatch(canvas->backdropTL));
    buttonColorTL->setFocusPolicy(Qt::NoFocus);
    connect(buttonColorTL,SIGNAL(clicked(bool)), this,SLOT(onTLColorButtonClicked()));

    buttonColorTR = new QPushButton("Top Right");
    buttonColorTR->setStyleSheet(colorButtonStyleRight);
    buttonColorTR->setIcon(createColorPatch(canvas->backdropTR));
    buttonColorTR->setFocusPolicy(Qt::NoFocus);
    connect(buttonColorTR,SIGNAL(clicked(bool)), this,SLOT(onTRColorButtonClicked()));

    colorLayoutTop->addWidget(buttonColorTL);
    colorLayoutTop->addWidget(buttonColorTR);

    auto* colorLayoutBottom = new QHBoxLayout;
    mainGrid->addLayout(colorLayoutBottom, 2, 0, 1, 2);

    buttonColorBL = new QPushButton("Bottom Left");
    buttonColorBL->setStyleSheet(colorButtonStyleLeft);
    buttonColorBL->setIcon(createColorPatch(canvas->backdropBL));
    buttonColorBL->setFocusPolicy(Qt::NoFocus);
    connect(buttonColorBL,SIGNAL(clicked(bool)), this,SLOT(onBLColorButtonClicked()));

    buttonColorBR = new QPushButton("Bottom Right");
    buttonColorBR->setStyleSheet(colorButtonStyleRight);
    buttonColorBR->setIcon(createColorPatch(canvas->backdropBR));
    buttonColorBR->setFocusPolicy(Qt::NoFocus);
    connect(buttonColorBR,SIGNAL(clicked(bool)), this,SLOT(onBRColorButtonClicked()));

    colorLayoutBottom->addWidget(buttonColorBL);
    colorLayoutBottom->addWidget(buttonColorBR);
}

void BackdropSettingsDialog::onPresetChanged(const int index) const
{
    const int presetId = comboBackdropPresets->itemData(index).toInt();
    canvas->setBackdropPresetIndex(presetId);

    switch (presetId)
    {
    case 1:
        canvas->setBackdropCorners(
            QColor::fromRgbF(0.80f, 0.83f, 0.86f),
            QColor::fromRgbF(0.72f, 0.75f, 0.78f),
            QColor::fromRgbF(0.18f, 0.19f, 0.22f),
            QColor::fromRgbF(0.26f, 0.28f, 0.31f)
        );
        break;
    case 2:
        canvas->setBackdropCorners(
            QColor::fromRgbF(0.90f, 0.90f, 0.92f),
            QColor::fromRgbF(0.88f, 0.88f, 0.90f),
            QColor::fromRgbF(0.80f, 0.80f, 0.82f),
            QColor::fromRgbF(0.78f, 0.78f, 0.80f)
        );
        break;
    case 3:
        canvas->setBackdropCorners(
            QColor::fromRgbF(0.12f, 0.22f, 0.45f),
            QColor::fromRgbF(0.10f, 0.18f, 0.40f),
            QColor::fromRgbF(0.05f, 0.10f, 0.25f),
            QColor::fromRgbF(0.04f, 0.08f, 0.20f)
        );
        break;
    case 4:
        canvas->setBackdropCorners(
            QColor::fromRgbF(0.15f, 0.15f, 0.18f),
            QColor::fromRgbF(0.10f, 0.10f, 0.12f),
            QColor::fromRgbF(0.02f, 0.02f, 0.03f),
            QColor::fromRgbF(0.04f, 0.04f, 0.05f)
        );
        break;
    case 5:
        canvas->setBackdropCorners(
            QColor::fromRgbF(0.90f, 0.85f, 0.78f),
            QColor::fromRgbF(0.95f, 0.90f, 0.82f),
            QColor::fromRgbF(0.60f, 0.53f, 0.45f),
            QColor::fromRgbF(0.68f, 0.60f, 0.50f)
        );
        break;
    case 6:
        canvas->setBackdropCorners(
            QColor::fromRgbF(0.22f, 0.05f, 0.32f),
            QColor::fromRgbF(0.05f, 0.32f, 0.38f),
            QColor::fromRgbF(0.10f, 0.02f, 0.18f),
            QColor::fromRgbF(0.02f, 0.20f, 0.28f)
        );
        break;
    case 7:
        canvas->setBackdropCorners(
            QColor::fromRgbF(0.95f, 0.78f, 0.82f),
            QColor::fromRgbF(0.88f, 0.85f, 0.95f),
            QColor::fromRgbF(0.98f, 0.88f, 0.70f),
            QColor::fromRgbF(0.90f, 0.78f, 0.92f)
        );
        break;
    case 8:
        canvas->setBackdropCorners(
            QColor::fromRgbF(0.05f, 0.18f, 0.32f),
            QColor::fromRgbF(0.06f, 0.30f, 0.42f),
            QColor::fromRgbF(0.02f, 0.10f, 0.20f),
            QColor::fromRgbF(0.00f, 0.22f, 0.32f)
        );
        break;
    case 9:
        canvas->setBackdropCorners(
            QColor::fromRgbF(0.28f, 0.18f, 0.12f),
            QColor::fromRgbF(0.38f, 0.25f, 0.12f),
            QColor::fromRgbF(0.12f, 0.08f, 0.05f),
            QColor::fromRgbF(0.22f, 0.15f, 0.08f)
        );
        break;
    case 10:
        canvas->setBackdropCorners(
            QColor::fromRgbF(1.0f, 0.0f, 0.0f),
            QColor::fromRgbF(0.0f, 1.0f, 0.0f),
            QColor::fromRgbF(0.0f, 0.0f, 1.0f),
            QColor::fromRgbF(1.0f, 1.0f, 0.0f)
        );
        break;
    default: ;
    }

    buttonColorTL->setIcon(createColorPatch(canvas->backdropTL));
    buttonColorTR->setIcon(createColorPatch(canvas->backdropTR));
    buttonColorBL->setIcon(createColorPatch(canvas->backdropBL));
    buttonColorBR->setIcon(createColorPatch(canvas->backdropBR));
}

static QColor pickColor(const QColor& initial, QWidget* parent)
{
    return QColorDialog::getColor(initial, parent, "Choose color", QColorDialog::DontUseNativeDialog);
}

void BackdropSettingsDialog::onTLColorButtonClicked()
{
    const QColor newColor = pickColor(canvas->backdropTL, this);
    if (newColor.isValid() != true) return;
    canvas->setBackdropTLCorner(newColor);
    buttonColorTL->setIcon(createColorPatch(newColor));
    canvas->update();
    comboBackdropPresets->setCurrentIndex(0);
    canvas->setBackdropPresetIndex(0);
}

void BackdropSettingsDialog::onTRColorButtonClicked()
{
    const QColor newColor = pickColor(canvas->backdropTR, this);
    if (newColor.isValid() != true) return;
    canvas->setBackdropTRCorner(newColor);
    buttonColorTR->setIcon(createColorPatch(newColor));
    canvas->update();
    comboBackdropPresets->setCurrentIndex(0);
    canvas->setBackdropPresetIndex(0);
}

void BackdropSettingsDialog::onBLColorButtonClicked()
{
    const QColor newColor = pickColor(canvas->backdropBL, this);
    if (newColor.isValid() != true) return;
    canvas->setBackdropBLCorner(newColor);
    buttonColorBL->setIcon(createColorPatch(newColor));
    canvas->update();
    comboBackdropPresets->setCurrentIndex(0);
    canvas->setBackdropPresetIndex(0);
}

void BackdropSettingsDialog::onBRColorButtonClicked()
{
    const QColor newColor = pickColor(canvas->backdropBR, this);
    if (newColor.isValid() != true) return;
    canvas->setBackdropBRCorner(newColor);
    buttonColorBR->setIcon(createColorPatch(newColor));
    canvas->update();
    comboBackdropPresets->setCurrentIndex(0);
    canvas->setBackdropPresetIndex(0);
}
