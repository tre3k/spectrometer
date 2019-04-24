#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    this->setLayout(ui->mainLayout);
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_pushButtonClose_clicked()
{
    this->close();
}

void Settings::on_pushButtonOk_clicked()
{

}
