#include "mainthread.h"


MainThread::MainThread(QWidget *parent) : QThread(parent)
{


    /* for authoresation */
    //NetworkManager->setCookieJar(NetworkCookie);


}

void MainThread::run(){

    /*
    double maxwell_coeff = 1e-5;
    for(int i=0;i<Functions::CodeToChannel(parameters->code_channels);i++){
        parameters->data_counts->append(
                    (double)(rand()%500)/100+100*i*i*(4/sqrt(M_PI))*pow(maxwell_coeff,3/2)*exp(-maxwell_coeff*i*i)
                    );
    }

    this->msleep(parameters->time_of_cycle);
    emit signal_dataCountDone();
    */

    for(int i=0;i<parameters->cycles;i++){
        this->msleep(parameters->time_of_cycle);
        // Start, Stop, ReadMem ?
        emit signal_progressBar(i);
    }

}

void MainThread::slot_recvParameters(ThreadParameters *p){
    parameters = p;

}

