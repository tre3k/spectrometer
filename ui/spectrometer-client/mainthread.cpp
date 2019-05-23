#include "mainthread.h"


MainThread::MainThread(QWidget *parent) : QThread(parent)
{

}

void MainThread::run(){
    // init device and clean memory
    emit signal_Init();
    REPLY_DELAY

    // start measurement
    emit signal_Start();
    REPLY_DELAY

    for(int i=0;i<parameters->cycles;i++){
        emit signal_progressBar(i);
        this->msleep((unsigned long)(1000*parameters->time_of_cycle));

        emit signal_ReadMem();
        REPLY_DELAY
    }

    emit signal_Stop();
    REPLY_DELAY
}

void MainThread::slot_recvParameters(ThreadParameters *p){
    parameters = p;

}

void MainThread::slot_reply(QByteArray content){
    busy = false;

}
