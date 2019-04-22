#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QMdiArea>
#include <QMdiSubWindow>

#include "about.h"
#include "plotwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public:
    QMdiArea *mdiArea;
    About *about;
    PlotWindow *mainPlotWindow;

private slots:
    void on_actionAbout_triggered();
};


class MdiArea : public QMdiArea{
public:
    MdiArea(QWidget *parent = 0) : QMdiArea(parent), m_pixmap(":/pnpi.svg"){}
protected:
    void paintEvent(QPaintEvent *event)
    {
        QMdiArea::paintEvent(event);
        QPainter painter(viewport());
        int x = width() - m_pixmap.width()-30;
        int y = height() - m_pixmap.height()-30;

        painter.drawPixmap(x, y, m_pixmap.scaled(300,300,Qt::IgnoreAspectRatio,Qt::FastTransformation));
    }
private:
    QPixmap m_pixmap;
};

#endif // MAINWINDOW_H
