#include "plotwindow.h"

PlotWindow::PlotWindow(QWidget *parent) : QMdiSubWindow(parent)
{
    generateElements();
}

void PlotWindow::slot_CloseThis(){
    this->close();
}

void PlotWindow::generateElements(){
    centralWidget = new QWidget(this);
    centralWidget->setLayout(new QVBoxLayout());

    /* Init menubar and add items on menu */
    menuBar = new QMenuBar();

    menuFile = new QMenu("&File");
    menuBar->addMenu(menuFile);

    menuExport = new QMenu("&Export");
    menuExport->setIcon(QIcon(":/icons/share.svg"));
    menuFile->addMenu(menuExport);
    actionExportTXT = new QAction("Export to text file");
    actionExportTXT->setIcon(QIcon(":/icons/txt.svg"));
    actionExportPDF = new QAction("Export to PDF");
    actionExportPDF->setIcon(QIcon(":/icons/pdf.svg"));
    actionExportJPG = new QAction("Export to image file (JPG)");
    actionExportJPG->setIcon(QIcon(":/icons/jpg.svg"));
    actionExportBMP = new QAction("Export to image file (BMP)");
    actionExportBMP->setIcon(QIcon(":/icons/bmp.svg"));
    actionExportPNG = new QAction("Export to image file (PNG)");
    actionExportPNG->setIcon(QIcon(":/icons/png.svg"));
    menuExport->addAction(actionExportTXT);
    menuExport->addAction(actionExportPDF);
    menuExport->addAction(actionExportJPG);
    menuExport->addAction(actionExportBMP);
    menuExport->addAction(actionExportPNG);

    menuFile->addSeparator();

    actionClose = new QAction("&Close");
    actionClose->setIcon(QIcon(":/icons/cancel.svg"));
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

    actionChannels = new QAction("channels");
    actionChannels->setCheckable(true);
    actionChannels->setChecked(true);

    actionChannels_time = new QAction("time");
    actionChannels_time->setCheckable(true);
    actionChannels_time->setChecked(false);

    actionChannels_lambda = new QAction("wavelength");
    actionChannels_lambda->setCheckable(true);
    actionChannels_lambda->setChecked(false);

    menuAxis_x->addSeparator();
    menuAxis_x->addAction(actionChannels);
    menuAxis_x->addAction(actionChannels_time);
    menuAxis_x->addAction(actionChannels_lambda);

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
    connect(actionExportPDF,SIGNAL(triggered()),
            this,SLOT(slot_ExportPDF()));
    connect(actionExportTXT,SIGNAL(triggered()),
            this,SLOT(slot_ExportTXT()));
    connect(actionExportPNG,SIGNAL(triggered()),
            this,SLOT(slot_ExportPNG()));
    connect(actionExportJPG,SIGNAL(triggered()),
            this,SLOT(slot_ExportJPG()));
    connect(actionExportBMP,SIGNAL(triggered()),
            this,SLOT(slot_ExportBMP()));

    connect(actionChannels,SIGNAL(triggered()),
            this,SLOT(slot_xAxie_channels()));
    connect(actionChannels_time,SIGNAL(triggered()),
            this,SLOT(slot_xAxie_time()));
    connect(actionChannels_lambda,SIGNAL(triggered()),
            this,SLOT(slot_xAxie_wavelength()));
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

void PlotWindow::slot_ExportTXT(){
    QString filename = QFileDialog::getSaveFileName(NULL,"Export to text file","","text file (*.txt)");
    filename = filename.split(QString(".")).at(0);
    filename += ".txt";
    QFile f(filename);
    f.open(QIODevice::WriteOnly);
    QTextStream txtStream(&f);
    for(int num=0;num<plot->graphCount();num++){
        for(int i=0;i<plot->graph(num)->data()->size();i++){
            txtStream << QString::number(plot->graph(num)->data()->at(i)->key) << "\t"
                      << QString::number(plot->graph(num)->data()->at(i)->value) << "\n";
        }
    }
    f.close();
}

void PlotWindow::slot_ExportPDF(){
    QString filename = QFileDialog::getSaveFileName(NULL,"Export to PDF","","PDF (*.pdf)");
    filename = filename.split(QString(".")).at(0);
    filename += ".pdf";
    plot->savePdf(filename);
}

void PlotWindow::slot_ExportPNG(){
    QString filename = QFileDialog::getSaveFileName(NULL,"Export to image file (PNG)","","Image file (*.png)");
    filename = filename.split(QString(".")).at(0);
    filename += ".png";
    plot->savePng(filename);
}

void PlotWindow::slot_ExportBMP(){
    QString filename = QFileDialog::getSaveFileName(NULL,"Export to image file (BMP)","","Image file (*.bmp)");
    filename = filename.split(QString(".")).at(0);
    filename += ".bmp";
    plot->saveBmp(filename);
}

void PlotWindow::slot_ExportJPG(){
    QString filename = QFileDialog::getSaveFileName(NULL,"Export to image file (JPG)","","Image file (*.jpg)");
    filename = filename.split(QString(".")).at(0);
    filename += ".jpg";
    plot->saveJpg(filename);
}

void PlotWindow::changesXAxieTyepe(int type){
    xAxieType = type;

    switch (xAxieType) {
    case X_AXIE_TYPE_CHANNEL:
        actionChannels->setChecked(true);
        actionChannels_time->setChecked(false);
        actionChannels_lambda->setChecked(false);
        this->plot->xAxis->setLabel("channels");
        break;

    case X_AXIE_TYPE_TIME:
        actionChannels->setChecked(false);
        actionChannels_time->setChecked(true);
        actionChannels_lambda->setChecked(false);
        this->plot->xAxis->setLabel("time, ms");
        break;

    case X_AXIE_TYPE_WAVELENGTH:
        actionChannels->setChecked(false);
        actionChannels_time->setChecked(false);
        actionChannels_lambda->setChecked(true);
        this->plot->xAxis->setLabel("wavelength, Å");
        break;
    }
    this->plot->replot();
}

void PlotWindow::slot_xAxie_channels(){changesXAxieTyepe(X_AXIE_TYPE_CHANNEL);}
void PlotWindow::slot_xAxie_time(){changesXAxieTyepe(X_AXIE_TYPE_TIME);}
void PlotWindow::slot_xAxie_wavelength(){changesXAxieTyepe(X_AXIE_TYPE_WAVELENGTH);}

void PlotWindow::slot_replot(){
    this->plot->replot();
}
