
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

// Rabinovich-fabrikant & Chen Lee: https://marmphco.com/dynamical/writeup.pdf
// Original MATLAB Code for Chua oscillator: http://www.chuacircuits.com/matlabsim.php


#pragma once
#include <cmath>

////////////////////////////////////////////////////////////////////////////////////////
// Roessler ////////////////////////////////////////////////////////////////////////////
struct roessler
{
	float	dx = 0.0f;
	float	dy = 0.0f;
	float	dz = 0.0f;

	float	x = 1.0f;
	float	y = 1.0f;
	float	z = 1.0f;

	float	a = 0.2f;
	float	b = 0.2f;
	float	c = 5.7f;

	float	delta = 1.0f;
    
    void    iterate();
};

void roessler::iterate()
{
	dx = -y - z;
	dy = x + a * y;
	dz = b + z * (x - c);

	x += dx * delta;
	y += dy * delta;
	z += dz * delta;
}

////////////////////////////////////////////////////////////////////////////////////////
// Lorenz //////////////////////////////////////////////////////////////////////////////
class lorenz
{
    private:
        float xt = 0;
        float yt = 0;
        float zt = 0;

    public:
        float a = 10.0;
        float b = 28.0;
        float c = 8.0 / 3.0;
        float t = 0.01; 

        float x = 0.1; 
        float y = 0;
        float z = 0; 

        float	delta = 1.0f;

        void iterate(){
                xt = x + t * a * (y - x);
                yt = y + t * (x * (b - z) - y);
                zt = z + t * (x * y - c * z);
                x = xt * delta;
                y = yt * delta;
                z = zt * delta;
        }
};

////////////////////////////////////////////////////////////////////////////////////////
// Ikeda ///////////////////////////////////////////////////////////////////////////////
struct ikeda
{

	float u = 0.918;
	float x = 0.8;
	float y = 0.7;
	float t;
	float ax, ay;


    void iterate()
	{ 
        t = 0.4 - 6 / (1 + x*x + y*y);
        ax = 1 + u * (x * cos(t) - y * sin(t));
        ay = u * (x * sin(t) + y * cos(t));
        x = ax;
        y = ay;
	}
};

////////////////////////////////////////////////////////////////////////////////////////
// Duffing /////////////////////////////////////////////////////////////////////////////
struct duffing
{
	float x;
	float y;
	float ax;
	float ay;
	float a = 2.75f;
	float b = 0.2f;

	void iterate()
	{
		ax = y;
		ay = -b*x + a*y - y*y*y;
		x = ax;
		y = ay;
	}
};

////////////////////////////////////////////////////////////////////////////////////////
// Henon ///////////////////////////////////////////////////////////////////////////////
struct henon
{
	float x;
	float y;
	float ax;
	float ay;
	float a = 1.4f;
	float b = 0.3f;

	void iterate()
	{
		ax = 1-a*x*x+y;
		ay = b*x;
		x = ax;
		y = ay;
	}
};

////////////////////////////////////////////////////////////////////////////////////////
// Gingerbreadman //////////////////////////////////////////////////////////////////////
struct gingerbreadman
{
	float x;
	float y;
	float ax;
	float ay;

	void iterate()
	{
		ax = 1-y+abs(x);
		ay = x;
		x = ax;
		y = ay;
	}
};

////////////////////////////////////////////////////////////////////////////////////////
// Van Der Pol /////////////////////////////////////////////////////////////////////////
struct vanderpol
{
	float x;
	float y;
	float f = 1.2f;
	float mu;
	float delta;

	void iterate();
};

void vanderpol::iterate()
{
	float ax = x + y * delta;
	float ay = y + (mu * (f - x * x) * y - x + f*sinf(y*delta)) * delta;
    //float ay = y + (mu * (1 - x * x) * y - x + f*sinf(delta)) * delta;
	x = ax;
	y = ay;
}

////////////////////////////////////////////////////////////////////////////////////////
// Kaplan-Yorke ////////////////////////////////////////////////////////////////////////
struct kaplan_yorke
{
	float x;
	float y;
    int a   = 0xFFFFFFFF;
    int b   = 2147483647;
	float alpha;

	void iterate();
};

void kaplan_yorke::iterate()
{
    int   aa = 2 * a % b;
	float ax = float(a) / float(b);
	float ay = alpha*y + cos(4*M_PI*x);
    a = aa;
	x = ax;
	y = ay;
}

////////////////////////////////////////////////////////////////////////////////////////
// Rabinovich-Fabrikant ////////////////////////////////////////////////////////////////
struct rabinovich_fabrikant
{
    float gamma = 0.87f;
    float alpha = 1.1f;
    float x, y, z;
    float ax, ay, az;

    void iterate();

};

void rabinovich_fabrikant::iterate() 
{
    ax = y*(x-1+x*x)+gamma*x;
    ay = x*(3*z+1-x*x)+gamma*y;
    az = -2*z*(alpha+x*y);
    x = ax;
    y = ay;
    z = az;
}

////////////////////////////////////////////////////////////////////////////////////////
// Chen-Lee ////////////////////////////////////////////////////////////////////////////
struct chen_lee
{
    float a, b, c;
    float x, y, z;
    float ax, ay, az;

    void iterate();
};

void chen_lee::iterate()
{
    ax = -y*z+a*x;
    ay = x*z-b*y;
    az = (1/3)*x*y-c*z;
    x = ax;
    y = ay;
    z = az;
}

////////////////////////////////////////////////////////////////////////////////////////
// Chua ////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////////////
// Chua circuit emulation //////////////////////////////////////////////////////////////
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

    float Esat  = 9; // 9V phasecvbatteries
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
    float R8    = 1000; //phasecv 1k Ohms
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

struct julia
{
    float zx = 1.2, zy = 0.8;
    float cx = 0.2, cy = 0.3;

    void iterate()
    {
        float   xm = zx * zx - zy * zy;
                zy = 2 * zx * zy  + cy ;
                zx = xm + cx;
    }

};




