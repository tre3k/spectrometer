#include "plotwindow.h"

PlotWindow::PlotWindow(QWidget *parent) : QMdiSubWindow(parent)
{
    generateElements();
}

void PlotWindow::slot_CloseThis(){
    this->close();
}

void PlotWindow::generateElements(){
    /* set icon and geometry */
    //this->setWindowIcon(QIcon(QPixmap(":/icon.png")));

    centralWidget = new QWidget(this);
    centralWidget->setLayout(new QVBoxLayout());

    /* Init menubar and add items on menu */
    menuBar = new QMenuBar();

    menuFile = new QMenu("&File");
    menuBar->addMenu(menuFile);

    actionExport = new QAction("&Export");
    menuFile->addAction(actionExport);

    menuFile->addSeparator();

    actionClose = new QAction("&Close");
    actionClose->setIcon(QIcon(":/cancel.svg"));
    menuFile->addAction(actionClose);

    menuAxes = new QMenu("&Axes");
    menuBar->addMenu(menuAxes);
    menuAxis_x = new QMenu("&x");
    menuAxis_y = new QMenu("&y");
    menuAxes->addMenu(menuAxis_x);
    menuAxes->addMenu(menuAxis_y);
    actionAxis_x_log_scale = new QAction("log scale");
    actionAxis_x_log_scale->setCheckable(true);
    actionAxis_y_log_scale = new QAction("log scale");
    actionAxis_y_log_scale->setCheckable(true);
    menuAxis_x->addAction(actionAxis_x_log_scale);
    menuAxis_y->addAction(actionAxis_y_log_scale);
    actionChannels_time = new QAction("channels/time");
    actionChannels_time->setCheckable(true);
    actionChannels_time->setChecked(true);
    menuAxis_x->addAction(actionChannels_time);
    actionAutoScale = new QAction("Auto scale");
    menuAxes->addAction(actionAutoScale);


    /* Set menu bar and widget on layout */
    plot = new Plot(this);
    centralWidget->layout()->setMenuBar(menuBar);
    centralWidget->layout()->addWidget(plot);


    this->setWidget(centralWidget);

    connect(actionClose,SIGNAL(triggered()),
            this,SLOT(slot_CloseThis()));
    connect(actionAxis_x_log_scale,SIGNAL(triggered(bool)),
            this,SLOT(slot_AxisXLogScale(bool)));
    connect(actionAxis_y_log_scale,SIGNAL(triggered(bool)),
            this,SLOT(slot_AxisYLogScale(bool)));
    connect(actionAutoScale,SIGNAL(triggered()),
            this,SLOT(slot_AutoScale()));
}

void PlotWindow::slot_AxisXLogScale(bool value){
    if(value){
        plot->xAxis->setScaleType(QCPAxis::stLogarithmic);
        plot->xAxis2->setScaleType(QCPAxis::stLogarithmic);
    }else{
        plot->xAxis->setScaleType(QCPAxis::stLinear);
        plot->xAxis2->setScaleType(QCPAxis::stLinear);
    }
    plot->replot();
}

void PlotWindow::slot_AxisYLogScale(bool value){
    if(value){
        plot->yAxis->setScaleType(QCPAxis::stLogarithmic);
        plot->yAxis2->setScaleType(QCPAxis::stLogarithmic);
    }else{
        plot->yAxis->setScaleType(QCPAxis::stLinear);
        plot->yAxis2->setScaleType(QCPAxis::stLinear);
    }
    plot->replot();
}

void PlotWindow::slot_AutoScale(){
    plot->rescaleAxes(true);
    plot->replot();
}
