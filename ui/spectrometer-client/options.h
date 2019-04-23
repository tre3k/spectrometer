#ifndef OPTIONS_H
#define OPTIONS_H

#include <QString>
#include <QDir>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QMessageBox>


#define OPTIONS_PATH QDir::toNativeSeparators(QDir::homePath() + "/.spectrometer-client")
#define OPTIONS_FILE QDir::toNativeSeparators(QDir::homePath() + "/.spectrometer-client/options.xml")

#define XML_DTD "<!DOCTYPE spectrometer-client>"
#define XML_START "options"
#define XML_HOST "host"
#define XML_CHANNELS "channels"
#define XML_WIDTH_CHANNELS "width_channels"
#define XML_CYCLES "cycles"
#define XML_TIME_OF_CYCLES "time_of_cycles"



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
    int channels;
    int width_channel;
    int cycles;
    double time_of_cycles;

public:
    Options();
    void loadOptions();
    void saveOptions();
};

#endif // OPTIONS_H
