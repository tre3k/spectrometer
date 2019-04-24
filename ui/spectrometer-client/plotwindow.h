#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include <QtGui>
#include <QMdiSubWindow>
#include <QVBoxLayout>
#include <QWidget>
#include <QMenuBar>
#include <QMenu>
#include <QAction>


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
    QAction *actionExport;
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
public slots:
    void slot_AutoScale();

};

#endif // PLOTWINDOW_H
