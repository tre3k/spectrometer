#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include <QtGui>
#include <QMdiSubWindow>
#include <QVBoxLayout>
#include <QWidget>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QTextStream>

#include "plot.h"

class PlotWindow : public QMdiSubWindow
{
    Q_OBJECT

public:
    PlotWindow(QWidget *parent = 0);

    Plot *plot;
    bool channels_time = true;

private:
    QWidget *centralWidget;
    QMenuBar *menuBar;

    QMenu *menuFile;
    QMenu *menuExport;
    QAction *actionExportTXT;
    QAction *actionExportPDF;
    QAction *actionExportPNG;
    QAction *actionExportJPG;
    QAction *actionExportBMP;
    QAction *actionClose;

    QMenu *menuAxes;
    QMenu *menuAxis_x;
public:
    QAction *actionChannels_time;
private:
    QAction *actionAxis_x_log_scale;
    QMenu *menuAxis_y;
    QAction *actionAxis_y_log_scale;
    QAction *actionAutoScale;

public:
    void generateElements();

private slots:
    void slot_CloseThis();
    void slot_AxisXLogScale(bool);
    void slot_AxisYLogScale(bool);
    void slot_ExportJPG();
    void slot_ExportBMP();
    void slot_ExportPNG();
    void slot_ExportTXT();
    void slot_ExportPDF();
public slots:
    void slot_AutoScale();

};

#endif // PLOTWINDOW_H
