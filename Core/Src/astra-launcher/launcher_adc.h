//
// Created by Fir on 24-11-28.
//

#ifndef FUCKCLION_CORE_SRC_ADC_LAUNCHER_H_
#define FUCKCLION_CORE_SRC_ADC_LAUNCHER_H_

#include <stdint.h>
#include "main.h"
#include "adc.h"

extern double rawDataADC1[4]; //0->VOL 1->I Board 2->I Base 3->Inner Temp
extern double rawDataADC2[3]; //0->VOL 1->I Board 2->I Base

extern double volADC1;
extern double volADC2;
extern double iBoardADC1;
extern double iBoardADC2;
extern double iBaseADC1;
extern double iBaseADC2;
extern double innerTempADC1;

extern double vKalmanFilter(double inData);
extern double iKalmanFilter(double inData);

extern void getValueADC1(double *rawData);
extern void getValueADC2(double *rawData);

#endif //FUCKCLION_CORE_SRC_ADC_LAUNCHER_H_
