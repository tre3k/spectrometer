#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Spectrometer UI Client");

    /* set central Widget as MDIArea */
    mdiArea = new MdiArea();
    this->setCentralWidget(mdiArea);

    /* init About dialog */
    about = new About();

    mainPlotWindow = new PlotWindow();
    mdiArea->addSubWindow(mainPlotWindow);
    mainPlotWindow->show();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    about->show();
}
