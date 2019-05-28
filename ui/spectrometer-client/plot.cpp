#include "plot.h"

Plot::Plot(QWidget *parent) : QCustomPlot(parent)
{
    this->setInteractions(QCP::iRangeZoom |
                          QCP::iRangeDrag |
                          //QCP::iMultiSelect |
                          //QCP::iSelectLegend |
                          //QCP::iSelectPlottables |
                          QCP::iSelectAxes);

    //this->xAxis2->setVisible(true);
    //this->yAxis2->setVisible(true);
    this->axisRect()->setupFullAxesBox();

    connect(this->xAxis, SIGNAL(rangeChanged(QCPRange)),this->xAxis2, SLOT(setRange(QCPRange)));
    connect(this->yAxis, SIGNAL(rangeChanged(QCPRange)), this->yAxis2, SLOT(setRange(QCPRange)));

    connect(this,SIGNAL(axisClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)),
            this,SLOT(slot_sAxies_drag_zoom(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)));
    connect(this,SIGNAL(mouseDoubleClick(QMouseEvent*)),
            this,SLOT(slot_full_drag_zoom(QMouseEvent*)));

    connect(this,SIGNAL(selectionChangedByUser()),
            this,SLOT(slot_selectionChanged()));

    return;

}

void Plot::addCurve(QVector<double> *x, QVector<double> *y, QString name){
    QCPGraph *graph;
    QCPBars *bars;

    switch(style){
    case PLOT_STYLE_LINE:
        graph = this->addGraph();
        graph->setPen(QPen(QColor(color),2,Qt::SolidLine,Qt::SquareCap,Qt::BevelJoin));
        graph->setData(*x,*y);
        this->rescaleAxes(autoscale);
        break;
    case PLOT_STYLE_POINTS:
        graph = this->addGraph();
        graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle,color,color,5));
        graph->setPen(QPen(QColor(color),1,Qt::NoPen,Qt::SquareCap,Qt::BevelJoin));
        graph->setData(*x,*y);
        this->rescaleAxes(autoscale);
        break;
    case PLOT_STYLE_BARS:
        bars = new QCPBars(this->xAxis,this->yAxis);
        bars->setData(*x,*y);
        this->rescaleAxes(autoscale);
        if(x->size()!=0) bars->setWidth(0.8*x->at(x->size()-1)/x->size());
        bars->setPen(Qt::NoPen);
        bars->setBrush(color);
        break;
    case PLOT_STYLE_POINT_WITH_BARS:
        bars = new QCPBars(this->xAxis,this->yAxis);
        bars->setData(*x,*y);
        this->rescaleAxes(autoscale);
        if(x->size()!=0) bars->setWidth(0.8*x->at(x->size()-1)/x->size());
        bars->setPen(Qt::NoPen);
        bars->setBrush(color);
        graph = this->addGraph();
        graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle,"black",color,5));
        graph->setPen(QPen(QColor(color),1,Qt::NoPen,Qt::SquareCap,Qt::BevelJoin));
        graph->setData(*x,*y);
        break;
    case PLOT_STYLE_LINE_WITH_BRUSH:
        graph = this->addGraph();
        graph->setPen(QPen(QColor(color),2,Qt::SolidLine,Qt::SquareCap,Qt::BevelJoin));
        color.setAlpha(color.alpha()/2);
        graph->setBrush(QBrush(color));
        graph->setData(*x,*y);
        this->rescaleAxes(autoscale);
        break;
    }

    this->replot();
    return;
}


void Plot::slot_sAxies_drag_zoom(QCPAxis* sAxis,QCPAxis::SelectablePart part,QMouseEvent* event){
    this->axisRect()->setRangeDrag(sAxis->orientation());
    this->axisRect()->setRangeZoom(sAxis->orientation());

    return;
}

void Plot::slot_full_drag_zoom(QMouseEvent *mouseEvent){
    this->axisRect()->setRangeDrag(this->xAxis->orientation()|
                                   this->yAxis->orientation()|
                                   this->xAxis2->orientation()|
                                   this->yAxis2->orientation());
    this->axisRect()->setRangeZoom(this->xAxis->orientation()|
                                   this->yAxis->orientation()|
                                   this->xAxis2->orientation()|
                                   this->yAxis2->orientation());
    return;
}

void Plot::slot_selectionChanged(){
    if (this->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || this->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
          this->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || this->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
      {
        this->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
        this->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
      }
      // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
      if (this->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || this->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
          this->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || this->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
      {
        this->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
        this->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
      }

      // synchronize selection of graphs with selection of corresponding legend items:
      for (int i=0; i<this->graphCount(); ++i)
      {
        QCPGraph *graph = this->graph(i);
        QCPPlottableLegendItem *item = this->legend->itemWithPlottable(graph);
        if (item->selected() || graph->selected())
        {
          item->setSelected(true);
          graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
        }
      }
    return;
}


void Plot::setTitle(QString title){
    this->plotLayout()->addElement(0,0,
                                   new QCPTextElement(this, title, QFont("sans", 12, QFont::Bold))
                                   );
}


void Plot::setStyle(int value){
    style = value;
}

void Plot::setColour(QColor value){
    color = value;
}
