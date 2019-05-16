#include "mainthread.h"


MainThread::MainThread(QWidget *parent) : QThread(parent)
{

    /* for authoresation */
    //NetworkManager->setCookieJar(NetworkCookie);

}

void MainThread::run(){

    // just delay for wait init //need reorgonize!
    this->msleep(1000);
    emit signal_Start();
    this->msleep(1000);

    for(int i=0;i<parameters->cycles;i++){
        emit signal_progressBar(i);
        emit signal_ReadMem();
        this->msleep((unsigned long)1000*parameters->time_of_cycle);
    }
    emit signal_Stop();
    this->msleep(600);
    emit signal_ReadMem();
}

void MainThread::slot_recvParameters(ThreadParameters *p){
    parameters = p;

}

