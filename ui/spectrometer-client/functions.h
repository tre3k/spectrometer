#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <math.h>
#include "options.h"

#define PLANK_CONST 1.05457180013e-34  // J*s
#define NEUTRON_MASS 1.67492747121e-27 // kg

class Functions
{
public:
    static int CodeToChannel(int code);
    static double CodeToWidthChannel(int code);
    static double LengthWave(double time,double distance);
};

#endif // FUNCTIONS_H
