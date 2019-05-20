#include "options.h"

Options::Options()
{
    /* default options value */
    channels = CHANNELS_1024;
    width_channel = WIDTH_CHANNEL_1p0;
    cycles = 100;
    time_of_cycles = 1;
    SpectraPlotType = PLOT_STYLE_BARS;

    host="http://localhost:8080/";
    logpath=QDir::homePath();


    plot_colour_r = 20;
    plot_colour_g = 100;
    plot_colour_b = 50;
    plot_colour_alpha = 200;

    loadOptions();
}

void Options::loadOptions(){
    /* just first run */
    if(!QDir(OPTIONS_PATH).exists()){
        QMessageBox::warning(NULL,"Options directory not found!","Directory: "+OPTIONS_PATH+" not found! Maybe this is the first launch? This directory will be created.");
        QDir().mkdir(OPTIONS_PATH);
        saveOptions();
        // keep default options
        return;
    }

    if(!QFile(OPTIONS_FILE).exists()){
        QMessageBox::warning(NULL,"Options file not found!","File: "+OPTIONS_FILE+" not found! Load default options.");
        return;
    }

    /* load options from xml file */
    QFile f(OPTIONS_FILE);
    f.open(QIODevice::ReadOnly);

    QXmlStreamReader xml(&f);
    while(!xml.atEnd() && !xml.hasError()){
        xml.readNext();
        if(xml.isStartElement()){
            if(xml.name()==XML_HOST) host = xml.readElementText();
            if(xml.name()==XML_USERNAME) username = xml.readElementText();
            if(xml.name()==XML_LOGPATH) logpath = xml.readElementText();
            if(xml.name()==XML_CHANNELS) channels = xml.readElementText().toInt();
            if(xml.name()==XML_WIDTH_CHANNELS) width_channel = xml.readElementText().toInt();
            if(xml.name()==XML_CYCLES) cycles = xml.readElementText().toInt();
            if(xml.name()==XML_TIME_OF_CYCLES) time_of_cycles = xml.readElementText().toDouble();
            if(xml.name()==XML_SPECRA_PLOT_STYLE) SpectraPlotType = xml.readElementText().toInt();
            if(xml.name()==XML_PLOT_COLOUR_R) plot_colour_r = xml.readElementText().toInt();
            if(xml.name()==XML_PLOT_COLOUR_G) plot_colour_g = xml.readElementText().toInt();
            if(xml.name()==XML_PLOT_COLOUR_B) plot_colour_b = xml.readElementText().toInt();
            if(xml.name()==XML_PLOT_COLOUR_ALPHA) plot_colour_alpha = xml.readElementText().toInt();
        }
    }

    f.close();
}

void Options::saveOptions(){
    /* save options to xml files */
    QFile f(OPTIONS_FILE);
    f.open(QIODevice::WriteOnly);
    QXmlStreamWriter xml(&f);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();
    xml.writeDTD(XML_DTD);

    xml.writeStartElement(XML_START);
    xml.writeTextElement(XML_HOST,host);
    xml.writeTextElement(XML_USERNAME,username);
    xml.writeTextElement(XML_LOGPATH,logpath);
    xml.writeTextElement(XML_CHANNELS,QString::number(channels));
    xml.writeTextElement(XML_WIDTH_CHANNELS,QString::number(width_channel));
    xml.writeTextElement(XML_CYCLES,QString::number(cycles));
    xml.writeTextElement(XML_TIME_OF_CYCLES,QString::number(time_of_cycles));
    xml.writeTextElement(XML_SPECRA_PLOT_STYLE,QString::number(SpectraPlotType));
    xml.writeTextElement(XML_PLOT_COLOUR_R,QString::number(plot_colour_r));
    xml.writeTextElement(XML_PLOT_COLOUR_G,QString::number(plot_colour_g));
    xml.writeTextElement(XML_PLOT_COLOUR_B,QString::number(plot_colour_b));
    xml.writeTextElement(XML_PLOT_COLOUR_ALPHA,QString::number(plot_colour_alpha));

    xml.writeEndElement();
    xml.writeEndDocument();

    f.close();
}
