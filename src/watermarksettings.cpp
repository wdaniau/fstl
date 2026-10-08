#include "watermarksettings.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QApplication>
#include <QFileDialog>

WatermarkSettings::WatermarkSettings(QWidget *parent, Canvas* _canvas) : QWidget(parent) {
    canvas = _canvas;

    QVBoxLayout* mainLayout = new QVBoxLayout;
    this->setLayout(mainLayout);

    QLabel* title = new QLabel("Watermark Settings");
    QFont boldFont = QApplication::font();
    boldFont.setWeight(QFont::Bold);
    title->setFont(boldFont);
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    drawWatermarkCheckbox = new QCheckBox("Draw Watermark");
    mainLayout->addWidget(drawWatermarkCheckbox);

    useTextCheck = new QCheckBox("Use text");
    textGroup = new QGroupBox("Text");
    textEdit = new QLineEdit(textGroup);
    textEdit->setPlaceholderText("Watermark text");
    QVBoxLayout* textLayout = new QVBoxLayout(textGroup);
    textLayout->addWidget(textEdit);

    imageGroup = new QGroupBox("Image");
    filePathEdit = new QLineEdit(imageGroup);
    filePathEdit->setReadOnly(true);
    filePathEdit->setPlaceholderText("No file selected");
    browseButton = new QPushButton("Browse...",imageGroup);
    QHBoxLayout* fileLayout = new QHBoxLayout;
    fileLayout->addWidget(filePathEdit, 1);
    fileLayout->addWidget(browseButton);

    previewLabel = new QLabel(imageGroup);
    previewLabel->setFixedSize(240, 140);
    previewLabel->setAlignment(Qt::AlignCenter);
    previewLabel->setFrameShape(QFrame::StyledPanel);
    previewLabel->setText(tr("Preview"));

    infoLabel = new QLabel(imageGroup);
    infoLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout* imageLayout = new QVBoxLayout(imageGroup);
    imageLayout->addLayout(fileLayout);
    imageLayout->addWidget(previewLabel, 0, Qt::AlignHCenter);
    imageLayout->addWidget(infoLabel);

    mainLayout->addWidget(useTextCheck);
    mainLayout->addWidget(textGroup);
    mainLayout->addWidget(imageGroup);

    mainLayout->addStretch();

    // fill form with settings
    QSettings settings;
    drawWatermarkCheckbox->setChecked(settings.value("drawWatermark",false).value<bool>());
    useTextCheck->setChecked(settings.value(Watermark::WATERMARK_USETEXT,true).value<bool>());
    filePathEdit->setText(settings.value(Watermark::WATERMARK_FILEPATH,"").value<QString>());
    textEdit->setText(settings.value(Watermark::WATERMARK_TEXT,"Watermark").value<QString>());

    // initialize preview
    getPix(filePathEdit->text());

    // initialize state
    updateEnabledState();

    // connections for drawWatermarkCheckbox
    connect(canvas, &Canvas::drawWatermarkChanged, this, [this](bool b){drawWatermarkCheckbox->setChecked(b);});
    connect(drawWatermarkCheckbox,&QCheckBox::toggled,this,[this](bool b){canvas->draw_watermark(b);});

    // general dialog connections
    connect(useTextCheck, &QCheckBox::toggled, this, &WatermarkSettings::updateEnabledState);
    connect(browseButton, &QPushButton::clicked, this, &WatermarkSettings::browseFile);

    // functional connections
    connect(useTextCheck,&QCheckBox::toggled,this,[this](bool b){
        canvas->getWatermark()->setUseText(b);
    });
    // pixmap change
    connect(filePathEdit,&QLineEdit::textChanged,this,[this](const QString& t){
        canvas->getWatermark()->setFilePath(t);
    });
    // text change
    connect(textEdit,&QLineEdit::textChanged,this,[this](const QString& t){
        canvas->getWatermark()->setText(t);
    });
}

void WatermarkSettings::browseFile() {
    const QString startDir = filePathEdit->text().isEmpty()
                                 ? QString()
                                 : QFileInfo(filePathEdit->text()).absolutePath();

    const QString path = QFileDialog::getOpenFileName(
        this,
        "Choose image",
        startDir,
        "Images (*.png *.jpg *.jpeg);;All files (*)");

    getPix(path);
}

void WatermarkSettings::getPix(const QString& path) {

    if (path.isEmpty())
        return;

    QPixmap pix(path);
    if (pix.isNull()) {
        // QMessageBox::warning(this, "Invalid file",
        //                      "Cannot read image");
        qWarning() << "Cannot read file " << path;
        return;
    }

    pixmap = pix;
    filePathEdit->setText(path);
    updatePreview();
}

void WatermarkSettings::updateEnabledState()
{
    const bool useText = useTextCheck->isChecked();

    textGroup->setEnabled(useText);
    imageGroup->setEnabled(!useText);
}

void WatermarkSettings::updatePreview()
{
    if (pixmap.isNull()) {
        previewLabel->setPixmap(QPixmap());
        previewLabel->setText("Preview");
        infoLabel->clear();
        return;
    }

    previewLabel->setText(QString());
    previewLabel->setPixmap(pixmap.scaled(previewLabel->size() - QSize(4, 4),
                                          Qt::KeepAspectRatio,
                                          Qt::SmoothTransformation));

    QString info = QString("Size : %1 x %2")
                       .arg(pixmap.width())
                       .arg(pixmap.height());

    infoLabel->setText(info);
}
