//
// Created by Fir on 24-11-28.
//

#include <math.h>
#include "launcher_adc.h"

double _vKalmanFilter(double inData)
{
  static double prevData = 0;                                 //先前数�??
  static double p = 10;
  static double q = 0.01;
  static double r = 0.1;
  static double kGain = 0;      // q控制误差  r控制响应速度

  p = p + q;
  kGain = p / ( p + r );                                     //计算卡尔曼增�?
  inData = prevData + ( kGain * ( inData - prevData ) );     //计算本次滤波估计�?
  p = ( 1 - kGain ) * p;                                     //更新测量方差
  prevData = inData;
  return inData;                                             //返回滤波�?
}

double _iKalmanFilter(double inData)
{
  static double prevData = 0;                                 //先前数�??
  static double p = 10;
  static double q = 0.0005;
  static double r = 0.01;
  static double kGain = 0;      // q控制误差  r控制响应速度

  p = p + q;
  kGain = p / ( p + r );                                     //计算卡尔曼增�?
  inData = prevData + ( kGain * ( inData - prevData ) );     //计算本次滤波估计�?
  p = ( 1 - kGain ) * p;                                     //更新测量方差
  prevData = inData;
  return inData;                                             //返回滤波�?
}

double rawDataADC1[4] = {0.0f}; //0->VOL 1->IBoard 2->IBase 3->InnerTemp
double rawDataADC2[3] = {0.0f}; //0->VOL 1->IBoard 2->IBase

double volADC1 = 0.0f;
double volADC2 = 0.0f;
double iBoardADC1 = 0.0f;
double iBoardADC2 = 0.0f;
double iBaseADC1 = 0.0f;
double iBaseADC2 = 0.0f;
double innerTempADC1 = 0.0f;

void launcher_get_value_adc1(double *rawData) {
  HAL_ADCEx_Calibration_Start(&hadc1);
  for (int i = 0; i < 4; i++) {
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 50);
    rawData[i] = HAL_ADC_GetValue(&hadc1);
  }
  HAL_ADC_Stop(&hadc1);
  volADC1 = _vKalmanFilter((rawData[0] * 3.3f / 4095.0f) * 6.0f);
  iBoardADC1 = _iKalmanFilter((rawData[1] * 3.3f / 4095.0f) / 5.0f);
  iBaseADC1 = (rawData[2] * 3.3f / 4095.0f) / 5.0f;
  double vInnerTempADC1 = (rawData[3] * 3.3f / 4095.0f);
//  innerTempADC1 = vInnerTempADC1;
  innerTempADC1 =  (1.43 - vInnerTempADC1) / 0.0043 + 25;
}

void launcher_get_value_adc2(double *rawData) {
  HAL_ADCEx_Calibration_Start(&hadc2);
  for (int i = 0; i < 3; i++) {
    HAL_ADC_Start(&hadc2);
    HAL_ADC_PollForConversion(&hadc2, 50);
    rawData[i] = HAL_ADC_GetValue(&hadc2);
  }
  HAL_ADC_Stop(&hadc2);
  volADC2 = _vKalmanFilter((rawData[0] * 3.3f / 4095.0f) * 6.0f);
  iBoardADC2 = _iKalmanFilter((rawData[1] * 3.3f / 4095.0f) / 5.0f);
  iBaseADC2 = (rawData[2] * 3.3f / 4095.0f) / 5.0f;
}