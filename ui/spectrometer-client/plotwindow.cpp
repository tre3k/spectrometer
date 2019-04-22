#include "plotwindow.h"

PlotWindow::PlotWindow(QWidget *parent) : QMdiSubWindow(parent)
{
    this->setWindowIcon(QIcon(QPixmap(":/icon.png")));
    this->setGeometry(0,0,500,200);


}
