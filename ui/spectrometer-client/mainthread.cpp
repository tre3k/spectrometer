#include "mainthread.h"


MainThread::MainThread(QWidget *parent) : QThread(parent)
{

}

void MainThread::run(){
    for(int i=0;i<Functions::CodeToChannel(parameters->code_channels);i++){
        parameters->data_counts->append(rand()%1000);
    }
    this->msleep(3000);
    emit signal_dataCountDone();
}

void MainThread::slot_recvParameters(ThreadParameters *p){
    parameters = p;
}
