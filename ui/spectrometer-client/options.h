#ifndef OPTIONS_H
#define OPTIONS_H

#include <QString>
#include <QDir>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QMessageBox>

#include "plot.h"


#define OPTIONS_PATH QDir::toNativeSeparators(QDir::homePath() + "/.spectrometer-client")
#define OPTIONS_FILE QDir::toNativeSeparators(QDir::homePath() + "/.spectrometer-client/options.xml")

#define XML_DTD "<!DOCTYPE spectrometer-client>"
#define XML_START "options"
#define XML_HOST "host"
#define XML_CHANNELS "channels"
#define XML_WIDTH_CHANNELS "width_channels"
#define XML_CYCLES "cycles"
#define XML_TIME_OF_CYCLES "time_of_cycles"
#define XML_SPECRA_PLOT_STYLE "spector_plot_style"

#define XML_PLOT_COLOUR_R "plot_colour_r"
#define XML_PLOT_COLOUR_G "plot_colour_g"
#define XML_PLOT_COLOUR_B "plot_colour_b"
#define XML_PLOT_COLOUR_ALPHA "plot_colour_alpha"


enum{
    CHANNELS_256,
    CHANNELS_512,
    CHANNELS_1024,
    CHANNELS_2048,
    CHANNELS_4096,
    CHANNELS_8192,
    CHANNELS_16384
};

enum{
    WIDTH_CHANNEL_8p0,
    WIDTH_CHANNEL_4p0,
    WIDTH_CHANNEL_2p0,
    WIDTH_CHANNEL_1p0,
    WIDTH_CHANNEL_0p5,
    WIDTH_CHANNEL_0p2,
    WIDTH_CHANNEL_0p1
};


class Options
{
public:
    QString host;
    int SpectraPlotType;
    int plot_colour_r;
    int plot_colour_g;
    int plot_colour_b;
    int plot_colour_alpha;

    int channels;
    int width_channel;
    int cycles;
    double time_of_cycles;

    double distance = 2.0;

public:
    Options();
    void loadOptions();
    void saveOptions();
};

#endif // OPTIONS_H
