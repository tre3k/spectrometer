#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QProgressBar>
#include <QVector>

#include "about.h"
#include "plotwindow.h"
#include "options.h"
#include "settings.h"
#include "mainthread.h"
#include "functions.h"
#include "server.h"

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

    void createSpectraWin();

public:
    QMdiArea *mdiArea;

    /* about dialog */
    About *about;

    /* plot window */
    PlotWindow *mainPlotWindow;
    QProgressBar *progressBar;

    /* options class */
    Options *options;

    /* setting dialog */
    Settings *settings;

    /* Main Thread */
    MainThread *mainThread;
    ThreadParameters *threadParameters;

    QVector <double> *data_counts;
    QVector <double> *data_channels;
    QVector <double> *data_time;

    /* Server class for http request */
    Server *server;

public:

    void buildDataOnPlot();

private slots:
    void slot_setChannels(int);
    void slot_setWidthChannel(int);
    void slot_changesXaxis(bool);
    void slot_dataCountDone();
    void slot_ReadMem();
    void slot_Init();
    void slot_Start();
    void slot_Stop();
    void slot_reply(QByteArray);

private slots:
    void on_actionAbout_triggered();
    void on_action_Quit_triggered();
    void on_actionSpectra_triggered();
    void on_actionTOF_triggered();
    void on_actionSettings_triggered();

    void on_pushButtonTOFStart_clicked();

    void on_actionConnect_triggered();

signals:
    void signal_sendParametersToThread(ThreadParameters *p);
    void signal_sendOptions(Options *);
};


class MdiArea : public QMdiArea{
public:
    MdiArea(QWidget *parent = 0) : QMdiArea(parent), m_pixmap(":/icons/pnpi.svg"){}
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
