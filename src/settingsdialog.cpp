#include "settingsdialog.h"
#include <QLabel>
#include <QVBoxLayout>
#include "canvas.h"
#include "backdropsettingsdialog.h"
#include "shaderlightprefs.h"
#include <QPushButton>
#include "logosettings.h"
#include "watermarksettings.h"

const QString SettingsDialog::PREFS_GEOM = "prefsGeometry";

SettingsDialog::SettingsDialog(QWidget *parent, Canvas* _canvas) : QDialog(parent) {
    setWindowTitle(tr("Preferences"));

    canvas = _canvas;
    tabs = new QTabWidget(this);

    tabs->addTab(new BackdropSettingsDialog(this,canvas),QIcon(":/qt/icons/backdrop-settings.png"),"Backdrop");
    tabs->addTab(new LogoSettings(this,canvas),QIcon(":/qt/icons/draw_logo_64x64.png"),"Logo");
    tabs->addTab(new WatermarkSettings(this,canvas),QIcon(":/qt/icons/draw_watermark_64x64.png"),"Watermark");

    // Shader pref tab
    shaderPrefPage = nullptr;
    onDrawModeChange(canvas->get_drawMode());

    QLayout* layout = new QVBoxLayout(this);
    layout->addWidget(tabs);

    connect(canvas,SIGNAL(drawModeChanged(DrawMode)),this,SLOT(onDrawModeChange(DrawMode)));

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
}

void SettingsDialog::onDrawModeChange(DrawMode m) {
    //qDebug() << "Received" << m;
    // delete tab
    if (shaderPrefPage) {
        tabs->removeTab((tabs->indexOf(shaderPrefPage)));
        delete shaderPrefPage;
        shaderPrefPage = nullptr;
    }
    // add tab
    if (m == meshlight) {
        QWidget* page = new ShaderLightPrefs(this,canvas);
        shaderPrefPage = page;
        tabs->addTab(shaderPrefPage,QIcon(":/qt/icons/sphere_shader4.png"),"Shader");
    }

}

void SettingsDialog::resizeEvent(QResizeEvent *event)
{
    QSettings().setValue(PREFS_GEOM, saveGeometry());
    QWidget::resizeEvent(event);
}

void SettingsDialog::moveEvent(QMoveEvent *event)
{
    QSettings().setValue(PREFS_GEOM, saveGeometry());
    QWidget::moveEvent(event);
}

void SettingsDialog::okButtonClicked() {
    this->close();
}
