#ifndef PLOT_H
#define PLOT_H

#include "qcustomplot.h"

enum{
    PLOT_STYLE_LINE,
    PLOT_STYLE_POINTS,
    PLOT_STYLE_BARS,
    PLOT_STYLE_POINT_WITH_BARS,
    PLOT_STYLE_LINE_WITH_BRUSH
};


class Plot : public QCustomPlot
{
    Q_OBJECT

private:
    int style;
    QColor color;

public:
    Plot(QWidget *parent = 0);
    void addCurve(QVector<double> *,QVector<double> *,QString name="");

    void setTitle(QString title);
    void setStyle(int value);
    void setColour(QColor value);

private slots:
    void slot_sAxies_drag_zoom(QCPAxis *,QCPAxis::SelectablePart,QMouseEvent *);
    void slot_full_drag_zoom(QMouseEvent *);
    void slot_selectionChanged();

};

#endif // PLOT_H
