#ifndef SHADERLIGHTPREFS_H
#define SHADERLIGHTPREFS_H

#include <QDialog>

class Canvas;
class QLabel;
class QLineEdit;
class QComboBox;
class QCheckBox;
class QSlider;
class QFrame;
class QButtonGroup;

class ShaderLightPrefs : public QDialog
{
    Q_OBJECT
public:
    ShaderLightPrefs(QWidget* parent, Canvas* _canvas);
    void toggleUseWire();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void moveEvent(QMoveEvent *event) override;

private slots:
    void buttonAmbientColorClicked();
    void editAmbientFactorFinished();
    void resetAmbientColorClicked();

    void buttonDirectiveColorClicked();
    void editDirectiveFactorFinished();
    void resetDirectiveColorClicked();

    void comboDirectionsChanged(int ind);
    void resetDirection();
    void radioSourceClicked(int ind);

    void checkboxUseWireFrameChanged();
    void buttonWireColorClicked();
    void resetWireColorClicked();
    void sliderWireWidthChanged();
    void resetWireWidthClicked();

    void okButtonClicked();
    void onFallbackGlslUpdated(bool b);

    void setRadio(int ind);

private:
    Canvas* canvas;
    QPushButton* buttonAmbientColor;
    QLineEdit* editAmbientFactor;
    QPushButton* buttonDirectiveColor;
    QLineEdit* editDirectiveFactor;
    QComboBox* comboDirections;

    QFrame* groupWireFrame;
    QCheckBox* checkboxUseWireFrame;
    QPushButton* buttonWireColor;
    QLabel* labelWireWidth;
    QSlider* sliderWireWidth;

    QButtonGroup* leftRight;
    QButtonGroup* topBottom;
    QButtonGroup* rearFront;

    const static QString PREFS_GEOM;
};

#endif // SHADERLIGHTPREFS_H
