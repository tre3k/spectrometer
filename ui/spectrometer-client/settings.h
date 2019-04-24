#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

private slots:
    void on_pushButtonClose_clicked();

    void on_pushButtonOk_clicked();

private:
    Ui::Settings *ui;
};

#endif // SETTINGS_H
