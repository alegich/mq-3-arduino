#include "mq3.h"
#include <util/delay.h>

MQ3::MQ3()
    : R0(0)
{
}

void MQ3::WarmUp()
{
    _delay_ms(600000); // 10 min
}

void MQ3::Calibrate(float voltage)
{
    R0 = getR0(voltage);
}

float MQ3::GetAlcohol(float voltage)
{
    return getConcentrationDiscrete(getGasRatio(voltage));
}

float MQ3::getR0()
{
    return R0;
}

float MQ3::getR0(float voltage) {
    float RS_air; //  Get the value of RS via in a clear air
    float R0;  // Get the value of R0 via in Alcohol

    RS_air = (5.0 - voltage) / voltage; // omit *RL
    R0 = RS_air / 60.0; // The ratio of RS/R0 is 60 in a clear air from Graph (Found using WebPlotDigitizer)

        return R0;
}

float MQ3::getGasRatio(float voltage) {
    float sensor_volt;
    float RS_gas; // Get value of RS in a GAS
    float ratio; // Get ratio RS_GAS/RS_air

          sensor_volt = voltage;
    RS_gas = (5.0 - sensor_volt) / sensor_volt; // omit *RL

    /*-Replace the name "R0" with the value of R0 in the demo of First Test -*/
    ratio = RS_gas / R0;  // ratio = RS/R0

          return ratio;
}

typedef struct {
        float x;
        float y;
} Func;

float MQ3::getConcentrationDiscrete(float gasRatio) {
        const unsigned int Count = 13;
  float X[Count] = {0.0, 0.02, 0.04, 0.07, 0.2, 0.3, 0.4, 0.53, 1.0, 1.7, 2.2, 60.0, 100.0};
        float Y[Count] = {20, 10.0, 8.0, 6.0, 4.0, 2.6, 1.7, 1.0, 0.4, 0.22, 0.1, 0.0, 0.0};
        float result = 0;

        unsigned int i = 1;

        while (i < Count && gasRatio > X[i]) { ++i; }

        result = Y[i - 1] + (gasRatio - X[i - 1]) * (Y[i] - Y[i - 1]) / (X[i] - X[i - 1]);
        return result;
}
