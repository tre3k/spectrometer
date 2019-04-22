#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include <QtGui>
#include <QMdiSubWindow>

class PlotWindow : public QMdiSubWindow
{
    Q_OBJECT

public:
    PlotWindow(QWidget *parent = 0);
};

#endif // PLOTWINDOW_H
