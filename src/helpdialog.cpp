#include "helpdialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QApplication>

HelpDialog::HelpDialog(QWidget *parent) : QDialog(parent)
{
    QVBoxLayout* helpLayout = new QVBoxLayout;
    this->setLayout(helpLayout);

    QLabel* title = new QLabel("Help");

    QFont boldFont = QApplication::font();
    boldFont.setWeight(QFont::Bold);
    title->setFont(boldFont);
    title->setAlignment(Qt::AlignCenter);

    helpLayout->addWidget(title);

    QWidget* centralWidget = new QWidget;
    QHBoxLayout* centralWidgetLayout = new QHBoxLayout;
    centralWidget->setLayout(centralWidgetLayout);
    QLabel* image = new QLabel;
    image->setPixmap(QPixmap(":/qt/images/benchy_doc.png"));
    centralWidgetLayout->addWidget(image);
    QLabel* shortcuts = new QLabel;
    shortcuts->setText("<h3>Mouse</h3>"
                       "<ul>"
                       "<li>Left clic : rotate the object</li>"
                       "<li>Right clic : translate the object</li>"
                       "<li>Wheel : zoom in/out</li>"
                       "</ul>"
                       "<h3>Shortcuts</h3>"
                       "<ul>"
                       "<li><b>H</b> : Display this help message"
                       "<li><b>Q</b> : Quit"
                       "<li><b>O</b> : Open"
                       "<li><b>R</b> : Reload the file"
                       "<li><b>P</b> : Draw Mode Settings for current shader (if available)"
                       "<li><b>A</b> : Draw Axes (and some informations)"
                       "<li><b>M</b> : Show/Hide Menu (and Toolbar as well)"
                       "<li><b>S</b> : Save Screenshot"
                       "<li><b>F</b> : Toggle Fullscreen"
                       "<li><b>W</b> : Toggle Wireframe on top of shader (if available)"
                       "<li><b>C</b> : Center View"
                       "<li><b>0-6</b> : Apply Default,Top,Bottom,Front,Rear,Left,Right view"
                       "<li><b>Left Arrow</b> : load previous stl file"
                       "<li><b>Right Arrow</b> : load next stl file"
                       "<li><b>Up Arrow</b> : use next shader"
                       "<li><b>Down Arrow</b> : use previous shader"
                       "</ul>");
    centralWidgetLayout->addWidget(shortcuts);

    helpLayout->addWidget(centralWidget);

    QWidget* boxButton = new QWidget;
    QHBoxLayout* boxButtonLayout = new QHBoxLayout;
    boxButton->setLayout(boxButtonLayout);
    QFrame *spacerL = new QFrame;
    spacerL->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding));
    QPushButton* okButton = new QPushButton("Ok");
    boxButtonLayout->addWidget(spacerL);
    boxButtonLayout->addWidget(okButton);
    this->layout()->addWidget(boxButton);
    //okButton->setFocusPolicy(Qt::NoFocus);

    connect(okButton,&QPushButton::clicked,[=]{
        this->hide();
    });



}
