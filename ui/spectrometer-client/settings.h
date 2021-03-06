#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QtMath>
#include <QColorDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QPalette>
#include <QFileDialog>

#include "options.h"
#include "plot.h"

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT
private:
    Options *options;
    Options localOptions;

    struct {
        QVBoxLayout mainLayout;
        QHBoxLayout inLayoyt;
        QHBoxLayout inLayout2;
        QComboBox comboBox;
        QLabel labelStyle;
        QLabel labelColor;
        QPalette paletteColorButton;
        QPushButton pushButtonColor;
        Plot plot;

        QVector<double> plot_data_x;
        QVector<double> plot_data_y;
    } plotWidgets;

    struct {
        QFormLayout layout;
        QLineEdit lineEditURL;
        QLineEdit lineuserName;

        QHBoxLayout logfileLayout;
        QLineEdit lineLogFile;
        QPushButton buttonLogFile;

        QDoubleSpinBox spinBoxDistance;
    } urlWidget;

    QColorDialog *colorDialog;

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

private slots:
    void slot_recvOptions(Options *);
    void slot_changePlotStyle(int style_code);
    void slot_changePlotColor(QColor color);
    void slot_pushButtonColor_clicked();
    void slot_pushButtonLogFilesDir();

private slots:
    void on_pushButtonClose_clicked();

    void on_pushButtonOk_clicked();

signals:
    void signal_setSettings();


private:
    Ui::Settings *ui;
};

#endif // SETTINGS_H
