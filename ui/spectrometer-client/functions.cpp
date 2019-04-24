#include "functions.h"

/* convert UI code from comboBox to value */
int Functions::CodeToChannel(int code){
     switch(code){
        case CHANNELS_256:
            return 256;
        case CHANNELS_512:
            return 512;
        case CHANNELS_1024:
            return 1024;
        case CHANNELS_2048:
            return 2048;
        case CHANNELS_4096:
            return 4096;
        case CHANNELS_8192:
            return 8192;
        case CHANNELS_16384:
            return 16384;
     }
     return 0;
}

/* convert UI code from comboBox to value */
double Functions::CodeToWidthChannel(int code){
    switch(code){
        case WIDTH_CHANNEL_0p1:
            return 0.1;
        case WIDTH_CHANNEL_0p2:
            return 0.2;
        case WIDTH_CHANNEL_0p5:
            return 0.5;
        case WIDTH_CHANNEL_1p0:
            return 1.0;
        case WIDTH_CHANNEL_2p0:
            return 2.0;
        case WIDTH_CHANNEL_4p0:
            return 4.0;
        case WIDTH_CHANNEL_8p0:
            return 8.0;
    }
    return 0.0;
}
