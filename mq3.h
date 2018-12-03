#ifndef MQ3_H
#define MQ3_H


class MQ3
{
public:
    MQ3();
    void WarmUp();
    void Calibrate(float voltage);
    float GetAlcohol(float voltage);

    float getR0();

private:

    float getR0(float voltage);
    float getGasRatio(float voltage);
    float getConcentrationDiscrete(float gasRatio);

    float R0;
};

#endif // MQ3_H
