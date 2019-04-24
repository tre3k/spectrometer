#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    this->setLayout(ui->mainLayout);

    /* ============== TAB1 ============== */
    /* add widegets to layout */
    urlWidget.layout.addRow("hostname URL: ",&urlWidget.lineEditURL);
    ui->tab->setLayout(&urlWidget.layout);


    /* ============== TAB2 ============== */
    /* set labelStyle text */
    plotWidgets.labelStyle.setText("plot style: ");
    /* plot settings */
    // add items in combo box
    plotWidgets.comboBox.addItem("line");
    plotWidgets.comboBox.addItem("points");
    plotWidgets.comboBox.addItem("bars");
    plotWidgets.comboBox.addItem("points with bars");
    plotWidgets.comboBox.addItem("line with brush");
    plotWidgets.comboBox.setCurrentIndex(PLOT_STYLE_LINE);

    // set plot
    plotWidgets.plot.xAxis->setLabel("channels");
    plotWidgets.plot.yAxis->setLabel("counts");

    double maxwell_coeff = 123e-6;
    for(int i=0;i<256;i++){
        plotWidgets.plot_data_x.append(i);
        plotWidgets.plot_data_y.append(
                    (rand()%500)/100+100*i*i*(4/sqrt(M_PI))*pow(maxwell_coeff,3/2)*exp(-maxwell_coeff*i*i)
                    );
    }
    plotWidgets.plot.setStyle(PLOT_STYLE_LINE);
    plotWidgets.plot.addCurve(&plotWidgets.plot_data_x,&plotWidgets.plot_data_y,"test");

    /* set color group */
    plotWidgets.labelColor.setText("color: ");
    plotWidgets.paletteColorButton.setColor(QPalette::Button,
                                            QColor(255,0,0,255));
    plotWidgets.pushButtonColor.setPalette(plotWidgets.paletteColorButton);
    colorDialog = new QColorDialog();
    colorDialog->setOption(QColorDialog::ShowAlphaChannel,true);

    // set in layout
    plotWidgets.inLayoyt.addWidget(&plotWidgets.labelStyle);
    plotWidgets.inLayoyt.addWidget(&plotWidgets.comboBox);

    plotWidgets.inLayout2.addWidget(&plotWidgets.labelColor);
    plotWidgets.inLayout2.addWidget(&plotWidgets.pushButtonColor);

    plotWidgets.mainLayout.addLayout(&plotWidgets.inLayoyt);
    plotWidgets.mainLayout.addLayout(&plotWidgets.inLayout2);
    plotWidgets.mainLayout.addWidget(&plotWidgets.plot);

    ui->tab_2->setLayout(&plotWidgets.mainLayout);


    /* CONNECTION Slots and signals */
    connect(&plotWidgets.comboBox,SIGNAL(currentIndexChanged(int)),
            this,SLOT(slot_changePlotStyle(int)));

    connect(&plotWidgets.pushButtonColor,SIGNAL(clicked()),
            this,SLOT(slot_pushButtonColor_clicked()));

    connect(colorDialog,SIGNAL(colorSelected(QColor)),
            this,SLOT(slot_changePlotColor(QColor)));

}

Settings::~Settings()
{
    delete ui;
}

void Settings::slot_recvOptions(Options *o){
    options = o;
    localOptions = *options;

    /* Server */
    urlWidget.lineEditURL.setText(localOptions.host);

    /* Plot */
    slot_changePlotStyle(localOptions.SpectraPlotType);
    plotWidgets.comboBox.setCurrentIndex(localOptions.SpectraPlotType);
    slot_changePlotColor(QColor(localOptions.plot_colour_r,
                                localOptions.plot_colour_g,
                                localOptions.plot_colour_b,
                                localOptions.plot_colour_alpha));

}

void Settings::on_pushButtonOk_clicked()
{
    localOptions.host = urlWidget.lineEditURL.text();
    *options = localOptions;
    this->close();
}


void Settings::on_pushButtonClose_clicked()
{
    this->close();
}


void Settings::slot_pushButtonColor_clicked(){
    colorDialog->setCurrentColor(QColor(options->plot_colour_r,
                                        options->plot_colour_g,
                                        options->plot_colour_b,
                                        options->plot_colour_alpha));
    colorDialog->show();
}

void Settings::slot_changePlotStyle(int style_code){
    localOptions.SpectraPlotType = style_code;

    plotWidgets.plot.clearGraphs();
    plotWidgets.plot.clearPlottables();
    plotWidgets.plot.setStyle(style_code);
    plotWidgets.plot.setColour(QColor(localOptions.plot_colour_r,
                                      localOptions.plot_colour_g,
                                      localOptions.plot_colour_b,
                                      localOptions.plot_colour_alpha));
    plotWidgets.plot.addCurve(&plotWidgets.plot_data_x,&plotWidgets.plot_data_y,"test");
}

void Settings::slot_changePlotColor(QColor color){
    localOptions.plot_colour_r = color.red();
    localOptions.plot_colour_g = color.green();
    localOptions.plot_colour_b = color.blue();
    localOptions.plot_colour_alpha = color.alpha();

    plotWidgets.paletteColorButton.setColor(QPalette::Button,color);
    plotWidgets.pushButtonColor.setPalette(plotWidgets.paletteColorButton);

    plotWidgets.plot.clearGraphs();
    plotWidgets.plot.clearPlottables();
    plotWidgets.plot.setColour(color);
    plotWidgets.plot.addCurve(&plotWidgets.plot_data_x,&plotWidgets.plot_data_y,"test");
}
