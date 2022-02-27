
// MIT License

// Copyright (c) 2022 unmanned

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// Original MATLAB Code: http://www.chuacircuits.com/matlabsim.php



#pragma once
#include <cmath>

struct chua
{
    float x, y, z;  // Ins

    float alpha  =  15.6;
    float beta   =  28; 
    float m0     = -1.143;
    float m1     = -0.714;
    float h;
    
    float xdot;
    float ydot;
    float zdot;
   
    void iterate();
};

void chua::iterate()
{
    h    = m1*x+0.5*(m0-m1)*(abs(x+1)-abs(x-1));
    xdot = alpha*(y-x-h);
    ydot = x - y+ z;
    zdot = -beta*y;
}


struct realchua
{
    float x, y, z;         // In
    float C1    = 10e-9;   // 10nF
    float C2    = 100e-9;  // 100nF
    float R     = 1800;    // 1.8k Ohms
    float G     = 1/R;

    // Chua Diode //////////////////////
    float R1    = 220;
    float R2    = 220;
    float R3    = 2200;
    float R4    = 22000;
    float R5    = 22000;
    float R6    = 3300;

    float Esat  = 9; // 9V batteries
    float E1    = R3/(R2+R3)*Esat;
    float E2    = R6/(R5+R6)*Esat;

    float m12   = -1/R6;
    float m02   = 1/R4;
    float m01   = 1/R1;
    float m11   = -1/R3;

    float m0;
    float m1    = m12+m11;

    // Gyrator ////////////////////
    float R7    = 100;  // 100 Ohms
    float R8    = 1000; // 1k Ohms
    float R9    = 1000; // 1k Ohms
    float R10   = 1800;
    float C     = 100*10^(-9);    // 100nF
    float L     = R7*R9*C*R10/R8; // 18mH 

    float xdot;
    float ydot;
    float zdot;
    
    void iterate();
};


void realchua::iterate()
{
    if(E1>E2)
    m0 = m11 + m02;
    else
    m0 = m12 + m01;   

    float mm1 = m01 + m02;
    float Emax = std::max(E1, E2);
    float Emin = std::min(E1, E2);

    float g;
    if (abs(x) < Emin) g = x*m1;     
    else if(abs(x) < Emax )
    {
        g = x*m0;
        if (x > 0) g = g + Emin*(m1-m0);    
        else g = g + Emin*(m0-m1);  
    }

    else if (abs(x) >= Emax)
    {
        g = x*mm1;    
        if (x > 0)
            g = g + Emax*(m0-mm1) + Emin*(m1-m0);
        else
            g = g + Emax*(mm1-m0) +  Emin*(m0-m1);
    }
    xdot = (1/C1)*(G*(y-x)-g);
    ydot = (1/C2)*(G*(x-y)+z);
    zdot  = -(1/L)*y;
}
