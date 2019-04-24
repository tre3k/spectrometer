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

    /* init settings dialog */
    settings = new Settings();

    /* data arrays */
    data_time = new QVector<double>;
    data_channels = new QVector<double>;
    data_counts = new QVector<double>;

    /* set TOF channels comboBox */
    ui->comboBoxChannelsTOF->addItem("256");
    ui->comboBoxChannelsTOF->addItem("512");
    ui->comboBoxChannelsTOF->addItem("1024");
    ui->comboBoxChannelsTOF->addItem("2048");
    ui->comboBoxChannelsTOF->addItem("4096");
    ui->comboBoxChannelsTOF->addItem("8192");
    ui->comboBoxChannelsTOF->addItem("16384");

    /* set TOF width of channel */
    ui->comboBoxWidthChannelTOF->addItem("8.0 μs");
    ui->comboBoxWidthChannelTOF->addItem("4.0 μs");
    ui->comboBoxWidthChannelTOF->addItem("2.0 μs");
    ui->comboBoxWidthChannelTOF->addItem("1.0 μs");
    ui->comboBoxWidthChannelTOF->addItem("0.5 μs");
    ui->comboBoxWidthChannelTOF->addItem("0.2 μs");
    ui->comboBoxWidthChannelTOF->addItem("0.1 μs");


    /* init and load options */
    options = new Options();
    ui->comboBoxChannelsTOF->setCurrentIndex(options->channels);
    ui->comboBoxWidthChannelTOF->setCurrentIndex(options->width_channel);
    ui->spinBoxCyclesTOF->setValue(options->cycles);
    ui->doubleSpinBoxTimeOfCycleTOF->setValue(options->time_of_cycles);

    /* formerid spectra Windows Plot*/
    mainPlotWindow = new PlotWindow();
    mainPlotWindow->setWindowTitle("spectra");
    mainPlotWindow->setWindowIcon(QIcon(":/icon.png"));
    mainPlotWindow->setGeometry(20,20,800,600);
    mainPlotWindow->setMinimumSize(400,300);
    createSpectraWin();
    mdiArea->addSubWindow(mainPlotWindow);

    /* set progressBar */
    progressBar = new QProgressBar();
    progressBar->setMaximumSize(120,13);
    ui->statusBar->addPermanentWidget(progressBar);


    /* connect signals and slots */
    connect(ui->comboBoxChannelsTOF,SIGNAL(currentIndexChanged(int)),
            this,SLOT(slot_setChannels(int)));
    connect(ui->comboBoxWidthChannelTOF,SIGNAL(currentIndexChanged(int)),
            this,SLOT(slot_setWidthChannel(int)));
    connect(mainPlotWindow->actionChannels_time,SIGNAL(triggered(bool)),
            this,SLOT(slot_changesXaxis(bool)));
}

MainWindow::~MainWindow()
{
    options->cycles = ui->spinBoxCyclesTOF->value();
    options->width_channel = ui->comboBoxWidthChannelTOF->currentIndex();
    options->channels = ui->comboBoxChannelsTOF->currentIndex();
    options->time_of_cycles = ui->doubleSpinBoxTimeOfCycleTOF->value();

    options->saveOptions();
    delete ui;
}

void MainWindow::createSpectraWin(){
    mainPlotWindow->generateElements();
    slot_changesXaxis(mainPlotWindow->channels_time);
    mainPlotWindow->plot->yAxis->setLabel("counts");

    slot_setChannels(options->channels);
}

void MainWindow::on_actionAbout_triggered()
{
    about->show();
}

void MainWindow::on_action_Quit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionSpectra_triggered()
{
    createSpectraWin();
    buildDataOnPlot();
    mainPlotWindow->show();
}

void MainWindow::on_actionTOF_triggered()
{
    ui->dockWidgetTOF->show();
}

/* Press settings button */
void MainWindow::on_actionSettings_triggered()
{
    settings->show();
}


void MainWindow::slot_setChannels(int channels_code){
    if(mainPlotWindow->channels_time){
        switch(channels_code){
        case CHANNELS_256:
            mainPlotWindow->plot->xAxis->setRange(QCPRange(0.0,256));
            break;
        case CHANNELS_512:
            mainPlotWindow->plot->xAxis->setRange(QCPRange(0.0,512));
            break;
        case CHANNELS_1024:
            mainPlotWindow->plot->xAxis->setRange(QCPRange(0.0,1024));
            break;
        case CHANNELS_2048:
            mainPlotWindow->plot->xAxis->setRange(QCPRange(0.0,2048));
            break;
        case CHANNELS_4096:
            mainPlotWindow->plot->xAxis->setRange(QCPRange(0.0,4096));
            break;
        case CHANNELS_8192:
            mainPlotWindow->plot->xAxis->setRange(QCPRange(0.0,8192));
            break;
        case CHANNELS_16384:
            mainPlotWindow->plot->xAxis->setRange(QCPRange(0.0,16384));
            break;
        }
    }else{
        slot_setWidthChannel(ui->comboBoxWidthChannelTOF->currentIndex());
    }
    mainPlotWindow->plot->replot();
}

void MainWindow::slot_setWidthChannel(int width_code){
    if(!mainPlotWindow->channels_time){
        switch(width_code){
        case WIDTH_CHANNEL_0p1:
            mainPlotWindow->plot->xAxis->setRange(QCPRange(0.0,
            0.1*CodeToChannel(ui->comboBoxChannelsTOF->currentIndex())/1000));
            break;
        case WIDTH_CHANNEL_0p2:
            mainPlotWindow->plot->xAxis->setRange(QCPRange(0.0,
            0.2*CodeToChannel(ui->comboBoxChannelsTOF->currentIndex())/1000));
            break;
        case WIDTH_CHANNEL_0p5:
            mainPlotWindow->plot->xAxis->setRange(QCPRange(0.0,
            0.5*CodeToChannel(ui->comboBoxChannelsTOF->currentIndex())/1000));
            break;
        case WIDTH_CHANNEL_1p0:
            mainPlotWindow->plot->xAxis->setRange(QCPRange(0.0,
            1.0*CodeToChannel(ui->comboBoxChannelsTOF->currentIndex())/1000));
            break;
        case WIDTH_CHANNEL_2p0:
            mainPlotWindow->plot->xAxis->setRange(QCPRange(0.0,
            2.0*CodeToChannel(ui->comboBoxChannelsTOF->currentIndex())/1000));
            break;
        case WIDTH_CHANNEL_4p0:
            mainPlotWindow->plot->xAxis->setRange(QCPRange(0.0,
            4.0*CodeToChannel(ui->comboBoxChannelsTOF->currentIndex())/1000));
            break;
        case WIDTH_CHANNEL_8p0:
            mainPlotWindow->plot->xAxis->setRange(QCPRange(0.0,
            8.0*CodeToChannel(ui->comboBoxChannelsTOF->currentIndex())/1000));
            break;
        }
    }
    mainPlotWindow->plot->replot();
}

void MainWindow::slot_changesXaxis(bool state){
    if(state){
        mainPlotWindow->plot->xAxis->setLabel("channels");
        slot_setChannels(ui->comboBoxChannelsTOF->currentIndex());
    }else{
        mainPlotWindow->plot->xAxis->setLabel("time, ms");
        slot_setWidthChannel(ui->comboBoxWidthChannelTOF->currentIndex());
    }
    mainPlotWindow->channels_time = state;

    mainPlotWindow->plot->replot();
    buildDataOnPlot();
    mainPlotWindow->slot_AutoScale();
}

void MainWindow::on_pushButtonTOFStart_clicked()
{
    data_counts->clear();
    data_time->clear();
    data_channels->clear();

    for(int i=0;i<CodeToChannel(ui->comboBoxChannelsTOF->currentIndex());i++){
        data_channels->append((double)i);
        data_time->append((double)i*CodeToWidthChannel(ui->comboBoxWidthChannelTOF->currentIndex())/1000);
        data_counts->append(rand()%1000);
    }

    buildDataOnPlot();
}

int MainWindow::CodeToChannel(int code){
     switch(code){
        case CHANNELS_256:
            return 256;
        case CHANNELS_512:
            return 512;
        case CHANNELS_1024:
            return 1024;
        case CHANNELS_2048:
            return 2048;
        case CHANNELS_4096:
            return 4096;
        case CHANNELS_8192:
            return 8192;
        case CHANNELS_16384:
            return 16384;
     }
     return 0;
}


double MainWindow::CodeToWidthChannel(int code){
    switch(code){
        case WIDTH_CHANNEL_0p1:
            return 0.1;
        case WIDTH_CHANNEL_0p2:
            return 0.2;
        case WIDTH_CHANNEL_0p5:
            return 0.5;
        case WIDTH_CHANNEL_1p0:
            return 1.0;
        case WIDTH_CHANNEL_2p0:
            return 2.0;
        case WIDTH_CHANNEL_4p0:
            return 4.0;
        case WIDTH_CHANNEL_8p0:
            return 8.0;
    }
    return 0.0;
}


void MainWindow::buildDataOnPlot(){
    mainPlotWindow->plot->clearGraphs();
    mainPlotWindow->plot->clearPlottables();

    mainPlotWindow->plot->setStyle(PLOT_STYLE_BARS);
    if(mainPlotWindow->channels_time){
        mainPlotWindow->plot->addCurve(data_channels,data_counts,"blue","counts");
    }else{
        mainPlotWindow->plot->addCurve(data_time,data_counts,"blue","counts");
    }
    mainPlotWindow->plot->replot();
}
