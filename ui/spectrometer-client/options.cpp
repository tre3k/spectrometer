#include "options.h"

Options::Options()
{
    /* default options value */
    channels = CHANNELS_1024;
    width_channel = WIDTH_CHANNEL_1p0;
    cycles = 100;
    time_of_cycles = 1;
    SpectraPlotType = PLOT_STYLE_LINE_WITH_BRUSH;

    host="http://localhost:8080/";
    logpath=QDir::homePath()+"/spectrometer-logs";

    username = "user";


    plot_colour_r = 20;
    plot_colour_g = 100;
    plot_colour_b = 50;
    plot_colour_alpha = 255;

    distance = 5.0;

    xTypeAxis = 0;

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
    int opt = XMLS_TOF_OPTIONS;

    while(!xml.atEnd() &&  !xml.hasError()){
        if(xml.isStartElement()){
            if(xml.name()==XML_TOF) opt=XMLS_TOF_OPTIONS;
            if(xml.name()==XML_TOOLBAR) opt=XMLS_TOOLBAR;
            if(xml.name()==XML_TOF_DOCK) opt=XMLS_TOF_DOCK;

            switch (opt) {
            case XMLS_TOF_OPTIONS:
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
                if(xml.name()==XML_DISTANCE) distance = xml.readElementText().toDouble();
                if(xml.name()==XML_X_AXYS_TYPE) xTypeAxis = xml.readElementText().toInt();
                break;

            case XMLS_TOOLBAR:
                if(xml.name()==XML_GEOMETRY_X) geometryToolBar.setX(xml.readElementText().toInt());
                if(xml.name()==XML_GEOMETRY_Y) geometryToolBar.setY(xml.readElementText().toInt());
                if(xml.name()==XML_GEOMETRY_WIDTH) geometryToolBar.setWidth(xml.readElementText().toInt());
                if(xml.name()==XML_GEOMETRY_HEIGHT) geometryToolBar.setHeight(xml.readElementText().toInt());
                break;

            case XMLS_TOF_DOCK:
                if(xml.name()==XML_GEOMETRY_X) geometryTOFWidget.setX(xml.readElementText().toInt());
                if(xml.name()==XML_GEOMETRY_Y) geometryTOFWidget.setY(xml.readElementText().toInt());
                if(xml.name()==XML_GEOMETRY_WIDTH) geometryTOFWidget.setWidth(xml.readElementText().toInt());
                if(xml.name()==XML_GEOMETRY_HEIGHT) geometryTOFWidget.setHeight(xml.readElementText().toInt());
                break;
            }
        }
        xml.readNextStartElement();

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

    xml.writeStartElement(XML_TOF);
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
    xml.writeTextElement(XML_DISTANCE,QString::number(distance));
    xml.writeTextElement(XML_X_AXYS_TYPE,QString::number(xTypeAxis));
    xml.writeEndElement();

    xml.writeStartElement(XML_TOOLBAR);
    xml.writeTextElement(XML_GEOMETRY_X,QString::number(geometryToolBar.x()));
    xml.writeTextElement(XML_GEOMETRY_Y,QString::number(geometryToolBar.y()));
    xml.writeTextElement(XML_GEOMETRY_WIDTH,QString::number(geometryToolBar.width()));
    xml.writeTextElement(XML_GEOMETRY_HEIGHT,QString::number(geometryToolBar.height()));
    xml.writeEndElement();

    xml.writeStartElement(XML_TOF_DOCK);
    xml.writeTextElement(XML_GEOMETRY_X,QString::number(geometryTOFWidget.x()));
    xml.writeTextElement(XML_GEOMETRY_Y,QString::number(geometryTOFWidget.y()));
    xml.writeTextElement(XML_GEOMETRY_WIDTH,QString::number(geometryTOFWidget.width()));
    xml.writeTextElement(XML_GEOMETRY_HEIGHT,QString::number(geometryTOFWidget.height()));
    xml.writeEndElement();

    xml.writeEndElement(); //XML_START
    xml.writeEndDocument();

    f.close();
}
