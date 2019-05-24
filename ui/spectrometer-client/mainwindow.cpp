#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Spectrometer UI Client");

    ui->dockWidgetAmpl->hide();     //reserve

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
    data_wavelength = new QVector<double>;

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
    mainPlotWindow->setWindowIcon(QIcon(":/icons/graph.svg"));
    mainPlotWindow->setGeometry(20,20,800,600);
    mainPlotWindow->setMinimumSize(400,300);
    mainPlotWindow->xAxieType = options->xTypeAxis;
    createSpectraWin();
    mdiArea->addSubWindow(mainPlotWindow);
    mainPlotWindow->changesXAxieTyepe(options->xTypeAxis);

    /* set progressBar */
    progressBar = new QProgressBar();
    progressBar->setMaximumSize(120,13);
    ui->statusBar->addPermanentWidget(progressBar);

    /* Main Thread */
    mainThread = new MainThread();
    threadParameters = new ThreadParameters;        //struct

    /* Server */
    server = new Server();

    /* Logfiles class init */
    logfiles = new Logfiles();
    logfiles->logpath = options->logpath;
    logfiles->logfilename = options->username+".log";


    /* restory geometry widgets */
    removeToolBar(ui->mainToolBar);
    addToolBar(options->toolBarArea,ui->mainToolBar);

    ui->mainToolBar->show();

    removeDockWidget(ui->dockWidgetTOF);
    addDockWidget(options->dockWidgetAreaTOF,ui->dockWidgetTOF);
    ui->dockWidgetTOF->show();

    ui->mainToolBar->setGeometry(options->geometryToolBar);
    ui->dockWidgetTOF->setGeometry(options->geometryTOFWidget);

    ui->mainToolBar->setHidden(!options->showToolBar);
    ui->dockWidgetTOF->setHidden(!options->showDockTOF);


    /* connect signals and slots */
    connect(ui->comboBoxChannelsTOF,SIGNAL(currentIndexChanged(int)),
            this,SLOT(slot_setChannels(int)));
    connect(ui->comboBoxWidthChannelTOF,SIGNAL(currentIndexChanged(int)),
            this,SLOT(slot_setWidthChannel(int)));

    /* connect with mainThread */
    connect(this,SIGNAL(signal_sendParametersToThread(ThreadParameters *)),
            mainThread,SLOT(slot_recvParameters(ThreadParameters *)));
    connect(mainThread,SIGNAL(signal_dataCountDone()),
            this,SLOT(slot_dataCountDone()));

    /* connect with Settings dialog */
    connect(this,SIGNAL(signal_sendOptions(Options *)),
            settings,SLOT(slot_recvOptions(Options *)));

    /* connect Server and MainWindow */
    connect(server,SIGNAL(signal_reply(QByteArray)),
            this,SLOT(slot_reply(QByteArray)));
    connect(server,SIGNAL(signal_reply(QByteArray)),
            mainThread,SLOT(slot_reply(QByteArray)));

    /* progress bar */
    connect(mainThread,SIGNAL(signal_progressBar(int)),
            progressBar,SLOT(setValue(int)));

    connect(mainThread,SIGNAL(signal_Init()),
            this,SLOT(slot_Init()));
    connect(mainThread,SIGNAL(signal_ReadMem()),
            this,SLOT(slot_ReadMem()));
    connect(mainThread,SIGNAL(signal_Start()),
            this,SLOT(slot_Start()));
    connect(mainThread,SIGNAL(signal_Stop()),
            this,SLOT(slot_Stop()));

    /* sync settings set and rebuild plot */
    connect(settings,SIGNAL(signal_setSettings()),
            this,SLOT(buildDataOnPlot()));
    connect(settings,SIGNAL(signal_setSettings()),
            this,SLOT(slot_setLogFileNamesFromOptions()));

    /* sync changed x type */
    connect(mainPlotWindow,SIGNAL(signal_XAxisChanged()),
            this,SLOT(buildDataOnPlot()));

}

MainWindow::~MainWindow()
{
    options->cycles = ui->spinBoxCyclesTOF->value();
    options->width_channel = ui->comboBoxWidthChannelTOF->currentIndex();
    options->channels = ui->comboBoxChannelsTOF->currentIndex();
    options->time_of_cycles = ui->doubleSpinBoxTimeOfCycleTOF->value();
    options->xTypeAxis = mainPlotWindow->xAxieType;

    options->geometryToolBar = ui->mainToolBar->geometry();
    options->toolBarArea = this->toolBarArea(ui->mainToolBar);
    options->geometryTOFWidget = ui->dockWidgetTOF->geometry();
    options->dockWidgetAreaTOF = this->dockWidgetArea(ui->dockWidgetTOF);

    options->showToolBar = !ui->mainToolBar->isHidden();
    options->floatingToolBar = ui->mainToolBar->isFloating();
    options->showDockTOF = !ui->dockWidgetTOF->isHidden();
    options->floatingDockTOF = ui->dockWidgetTOF->isFloating();

    options->saveOptions();
    delete ui;
}

void MainWindow::createSpectraWin(){
    mainPlotWindow->generateElements();
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
    emit signal_sendOptions(options);
    settings->show();
}


void MainWindow::slot_setChannels(int channels_code){

    mainPlotWindow->plot->replot();
}

void MainWindow::slot_setWidthChannel(int width_code){
    if(!mainPlotWindow->channels_time){
        switch(width_code){
        case WIDTH_CHANNEL_0p1:
            mainPlotWindow->plot->xAxis->setRange(QCPRange(0.0,
            0.1*Functions::CodeToChannel(ui->comboBoxChannelsTOF->currentIndex())/1000));
            break;
        case WIDTH_CHANNEL_0p2:
            mainPlotWindow->plot->xAxis->setRange(QCPRange(0.0,
            0.2*Functions::CodeToChannel(ui->comboBoxChannelsTOF->currentIndex())/1000));
            break;
        case WIDTH_CHANNEL_0p5:
            mainPlotWindow->plot->xAxis->setRange(QCPRange(0.0,
            0.5*Functions::CodeToChannel(ui->comboBoxChannelsTOF->currentIndex())/1000));
            break;
        case WIDTH_CHANNEL_1p0:
            mainPlotWindow->plot->xAxis->setRange(QCPRange(0.0,
            1.0*Functions::CodeToChannel(ui->comboBoxChannelsTOF->currentIndex())/1000));
            break;
        case WIDTH_CHANNEL_2p0:
            mainPlotWindow->plot->xAxis->setRange(QCPRange(0.0,
            2.0*Functions::CodeToChannel(ui->comboBoxChannelsTOF->currentIndex())/1000));
            break;
        case WIDTH_CHANNEL_4p0:
            mainPlotWindow->plot->xAxis->setRange(QCPRange(0.0,
            4.0*Functions::CodeToChannel(ui->comboBoxChannelsTOF->currentIndex())/1000));
            break;
        case WIDTH_CHANNEL_8p0:
            mainPlotWindow->plot->xAxis->setRange(QCPRange(0.0,
            8.0*Functions::CodeToChannel(ui->comboBoxChannelsTOF->currentIndex())/1000));
            break;
        }
    }
    mainPlotWindow->plot->replot();
}

void MainWindow::buildDataOnPlot(){
    mainPlotWindow->plot->clearGraphs();
    mainPlotWindow->plot->clearPlottables();
    mainPlotWindow->plot->setColour(QColor(options->plot_colour_r,
                                           options->plot_colour_g,
                                           options->plot_colour_b,
                                           options->plot_colour_alpha));
    mainPlotWindow->plot->setStyle(options->SpectraPlotType);

    /* anyway form channels array (for log) */
    data_channels->clear();
    for(int i=0;i<Functions::CodeToChannel(ui->comboBoxChannelsTOF->currentIndex());i++) data_channels->append(i);

    switch (mainPlotWindow->xAxieType) {
    case X_AXIE_TYPE_CHANNEL:
        mainPlotWindow->plot->addCurve(data_channels,data_counts,"counts");
        break;

    case X_AXIE_TYPE_TIME:
        data_time->clear();
        for(int i=0;i<Functions::CodeToChannel(ui->comboBoxChannelsTOF->currentIndex());i++){
            data_time->append(Functions::CodeToWidthChannel(ui->comboBoxWidthChannelTOF->currentIndex())*i/1000.0);
        }
        mainPlotWindow->plot->addCurve(data_time,data_counts,"counts");
        break;

    case X_AXIE_TYPE_WAVELENGTH:
        data_wavelength->clear();
        for(int i=0;i<Functions::CodeToChannel(ui->comboBoxChannelsTOF->currentIndex());i++){
            double time = Functions::CodeToWidthChannel(ui->comboBoxWidthChannelTOF->currentIndex())*1e-6*i;
            data_wavelength->append(Functions::LengthWave(time,options->distance));
        }
        mainPlotWindow->plot->addCurve(data_wavelength,data_counts,"counts");
        break;
    }

    mainPlotWindow->plot->replot();
}



/* run when data_count array in mainThread is full */
void MainWindow::slot_dataCountDone(){
    buildDataOnPlot();
}

/* slots for connect from thread */
void MainWindow::slot_Init(){
    server->setURL(options->host);
    QUrlQuery query;
    server->Request("init",query);
}

void MainWindow::slot_Start(){
    server->setURL(options->host);
    QUrlQuery query;
    int channels_code = 1;
    int widht_code = 2;

    ui->comboBoxChannelsTOF->setDisabled(true);
    ui->comboBoxWidthChannelTOF->setDisabled(true);

    switch (ui->comboBoxChannelsTOF->currentIndex()) {
    case CHANNELS_256:
        channels_code = 1;
        break;
    case CHANNELS_512:
        channels_code = 2;
        break;
    case CHANNELS_1024:
        channels_code = 4;
        break;
    case CHANNELS_2048:
        channels_code = 8;
        break;
    case CHANNELS_4096:
        channels_code = 16;
        break;
    case CHANNELS_8192:
        channels_code = 32;
        break;
    case CHANNELS_16384:
        channels_code = 64;
        break;
    }

    switch (ui->comboBoxWidthChannelTOF->currentIndex()) {
    case WIDTH_CHANNEL_0p1:
        widht_code = 128;
        break;
    case WIDTH_CHANNEL_0p2:
        widht_code = 64;
        break;
    case WIDTH_CHANNEL_0p5:
        widht_code = 32;
        break;
    case WIDTH_CHANNEL_1p0:
        widht_code = 16;
        break;
    case WIDTH_CHANNEL_2p0:
        widht_code = 8;
        break;
    case WIDTH_CHANNEL_4p0:
        widht_code = 4;
        break;
    case WIDTH_CHANNEL_8p0:
        widht_code = 2;
        break;
    }

    query.addQueryItem("channels",QString::number(channels_code));
    query.addQueryItem("width",QString::number(widht_code));
    query.addQueryItem("delay",QString::number(ui->spinBoxDelay->value()));
    server->Request("start",query);
    logfiles->writeDate();
    logfiles->write("Start measurement");
    logfiles->write("\tURL: "+options->host+"start"+"?"+query.toString().toLocal8Bit());
    logfiles->write("\tchannels: "+ui->comboBoxChannelsTOF->currentText());
    logfiles->write("\twidth of channel: "+ui->comboBoxWidthChannelTOF->currentText());
    logfiles->write("\tcycles: "+QString::number(ui->spinBoxCyclesTOF->value()));
    logfiles->setDataFileName(options->username);
}

void MainWindow::slot_Stop(){
    server->setURL(options->host);
    QUrlQuery query;
    server->Request("stop",query);
    logfiles->writeDate();
    logfiles->write("Stop measurement\n");

    ui->comboBoxChannelsTOF->setDisabled(false);
    ui->comboBoxWidthChannelTOF->setDisabled(false);
}

void MainWindow::slot_ReadMem(){
    server->setURL(options->host);
    QUrlQuery query;
    query.addQueryItem("size",ui->comboBoxChannelsTOF->currentText());
    server->Request("readmem",query);
}

/* slot for reply from server */
void MainWindow::slot_reply(QByteArray content){
    QStringList lst = QString(content).split('\n');

    if(lst.at(0)=="readmem"){
        data_counts->clear();
        for(int i=1;i<lst.size()-1;i++){
            data_counts->append(QString(lst.at(i)).toDouble());
        }
        slot_dataCountDone();

        logfiles->saveData("# Width of channel: "+ui->comboBoxWidthChannelTOF->currentText()+"\r\n"
                           "# distance chooper to detector: "+QString::number(options->distance)+" m \r\n"
                           "# channel\tcounts\r\n",
                           data_channels,data_counts);
    }

    if(lst.at(0)=="start"){

    }

    if(lst.at(0)=="stop"){

        progressBar->setMaximum(1);
        progressBar->setValue(1);
    }

    if(lst.at(0)=="init"){

    }
}

void MainWindow::on_actionConnect_triggered()
{

}

void MainWindow::on_pushButtonTOFStart_clicked()
{
    data_counts->clear();
    data_time->clear();
    data_channels->clear();

    threadParameters->data_counts = data_counts;
    threadParameters->cycles = ui->spinBoxCyclesTOF->value();
    progressBar->setRange(0,threadParameters->cycles-1);
    threadParameters->time_of_cycle = ui->doubleSpinBoxTimeOfCycleTOF->value();
    threadParameters->code_width = ui->comboBoxWidthChannelTOF->currentIndex();
    threadParameters->code_channels = ui->comboBoxChannelsTOF->currentIndex();
    threadParameters->channels = QString(ui->comboBoxChannelsTOF->currentText()).toInt();
    threadParameters->mainURL = options->host;
    emit signal_sendParametersToThread(threadParameters);

    mainThread->start();
}


void MainWindow::on_pushButtonTOFStop_clicked()
{
    mainThread->terminate();
    slot_Stop();
    QThread::sleep(1);
    slot_ReadMem();
}

void MainWindow::on_actionRead_data_triggered()
{
    data_channels->clear();
    data_time->clear();
    data_wavelength->clear();

    slot_ReadMem();
}

void MainWindow::on_actionInit_device_triggered()
{
    slot_Init();
}

void MainWindow::slot_setLogFileNamesFromOptions(){
    logfiles->logpath = options->logpath;
    logfiles->logfilename = options->username+".log";
}
