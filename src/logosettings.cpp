#include "logosettings.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QApplication>
#include <QCheckBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QFileDialog>
#include <QMessageBox>

LogoSettings::LogoSettings(QWidget *parent, Canvas *_canvas) : QWidget(parent) {
    canvas = _canvas;

    QVBoxLayout* mainLayout = new QVBoxLayout;
    this->setLayout(mainLayout);

    QLabel* title = new QLabel("Logo Settings");
    QFont boldFont = QApplication::font();
    boldFont.setWeight(QFont::Bold);
    title->setFont(boldFont);
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    drawLogoCheckbox = new QCheckBox("Draw Logo");
    mainLayout->addWidget(drawLogoCheckbox);

    useDefaultCheck = new QCheckBox("Use default");

    positionCombo = new QComboBox;
    for (int i=0; i < LOGOPOSITIONCOUNT; i++) {
        positionCombo->addItem(positionToString(static_cast<LogoPosition>(i)), i);
    }

    QFormLayout* positionForm = new QFormLayout;
    positionForm->addRow(tr("Position :"), positionCombo);

    customGroup = new QGroupBox("Custom Logo");

    filePathEdit = new QLineEdit(customGroup);
    filePathEdit->setReadOnly(true);
    filePathEdit->setPlaceholderText("No file selected");

    browseButton = new QPushButton("Browse...", customGroup);

    QHBoxLayout* fileLayout = new QHBoxLayout;
    fileLayout->addWidget(filePathEdit, 1);
    fileLayout->addWidget(browseButton);

    previewLabel = new QLabel(customGroup);
    previewLabel->setFixedSize(240, 140);
    previewLabel->setAlignment(Qt::AlignCenter);
    previewLabel->setFrameShape(QFrame::StyledPanel);
    previewLabel->setText("Preview");

    infoLabel = new QLabel(customGroup);
    infoLabel->setAlignment(Qt::AlignCenter);

    needResizeCheck = new QCheckBox("Resize height", customGroup);

    resizeHeightEdit = new QLineEdit(customGroup);
    resizeHeightEdit->setValidator(new QIntValidator(1, 10000, resizeHeightEdit));
    resizeHeightEdit->setMaximumWidth(80);

    QHBoxLayout* resizeLayout = new QHBoxLayout;
    resizeLayout->addWidget(needResizeCheck);
    resizeLayout->addWidget(resizeHeightEdit);
    resizeLayout->addWidget(new QLabel("px", customGroup));
    resizeLayout->addStretch(1);

    QVBoxLayout* groupLayout = new QVBoxLayout(customGroup);
    groupLayout->addLayout(fileLayout);
    groupLayout->addWidget(previewLabel, 0, Qt::AlignHCenter);
    groupLayout->addWidget(infoLabel);
    groupLayout->addLayout(resizeLayout);

    mainLayout->addWidget(useDefaultCheck);
    mainLayout->addLayout(positionForm);
    mainLayout->addWidget(customGroup);
    mainLayout->addStretch();

    // fill form with settings
    // Note : need a unified place with all the settings keys
    QSettings settings;
    drawLogoCheckbox->setChecked(settings.value("drawLogo",false).value<bool>());
    useDefaultCheck->setChecked(settings.value(Logo::LOGO_USEDEFAULT,true).value<bool>());
    filePathEdit->setText(settings.value(Logo::LOGO_FILEPATH,"").value<QString>());
    needResizeCheck->setChecked(settings.value(Logo::LOGO_NEEDRESIZE,false).value<bool>());
    resizeHeightEdit->setText(settings.value(Logo::LOGO_RESIZEHEIGHT,"64").value<QString>());
    positionCombo->setCurrentIndex(settings.value(Logo::LOGO_POSITION, static_cast<int>(LogoPosition::bottomLeft)).toInt());

    // initialize preview
    getPix(filePathEdit->text());

    // initialize state
    updateEnabledState();

    // connections for drawLogoCheckbox
    connect(canvas, &Canvas::drawLogoChanged, this, [this](bool b) {drawLogoCheckbox->setChecked(b);});
    connect(drawLogoCheckbox, &QCheckBox::toggled, this, [this](bool b){canvas->draw_logo(b);});

    // general dialog connections
    connect(useDefaultCheck, &QCheckBox::toggled, this, &LogoSettings::updateEnabledState);
    connect(browseButton, &QPushButton::clicked, this, &LogoSettings::browseFile);
    connect(needResizeCheck, &QCheckBox::toggled, this, &LogoSettings::updateEnabledState);
    connect(needResizeCheck, &QCheckBox::toggled, this, &LogoSettings::updatePreview);
    connect(resizeHeightEdit, &QLineEdit::textChanged, this, &LogoSettings::updatePreview);

    // functional connections
    // use default
    connect(useDefaultCheck, &QCheckBox::toggled, this,[this](bool b){
        canvas->getLogo()->setDefault(b);
    });
    // position
    // connect(positionCombo, &QComboBox::currentIndexChanged, this, [this](int i){
    //     LogoPosition p = static_cast<LogoPosition>(i);
    //     canvas->getLogo()->setPosition(p);
    // });
    connect(positionCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [this](int i){
        LogoPosition p = static_cast<LogoPosition>(i);
        canvas->getLogo()->setPosition(p);
    });


    // pixmap change
    connect(filePathEdit, &QLineEdit::textChanged, this, [this](const QString& t){
        canvas->getLogo()->setFilePath(t);
    });
    // resize change
    connect(needResizeCheck, &QCheckBox::toggled, this,[this](bool b){
        canvas->getLogo()->setNeedResize(b);
    });
    connect(resizeHeightEdit, &QLineEdit::textChanged, this,[this](const QString& t){
        canvas->getLogo()->setResizeHeight(t.toInt());
    });

}

QString LogoSettings::positionToString(LogoPosition position)
{
    switch (position) {
    case topLeft:     return QString("Top Left");
    case topRight:    return QString("Top Right");
    case bottomLeft:  return QString("Bottom Left");
    case bottomRight: return QString("Bottom Right");
    default:          return QString();
    }
}

void LogoSettings::browseFile() {
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

void LogoSettings::getPix(const QString& path) {

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

void LogoSettings::updatePreview()
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

    QString info = QString("Original : %1 x %2")
                       .arg(pixmap.width())
                       .arg(pixmap.height());

    bool ok = false;
    const int h = resizeHeightEdit->text().toInt(&ok);
    if (needResizeCheck->isChecked() && ok && h > 0 && pixmap.height() > 0) {
        const int w = qRound(double(pixmap.width()) * h / pixmap.height());
        info += QString("\nResized : %1 x %2").arg(w).arg(h);
    }
    infoLabel->setText(info);
}

void LogoSettings::updateEnabledState()
{
    const bool custom = !useDefaultCheck->isChecked();
    customGroup->setEnabled(custom);
    resizeHeightEdit->setEnabled(custom && needResizeCheck->isChecked());
}


