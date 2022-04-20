#pragma once
#include <cmath>

double Pi2 = M_PI * M_PI;

float bhaskara_sin(float theta)
{
        return 16.0f*theta*(M_PI-theta)/(5.0f*Pi2-4.0f*theta*(M_PI-theta));
}

float bhaskara_cos(float theta)
{
        double Th = theta*theta;
        return (Pi2-4.0f*Th)/(Pi2+Th);
}


