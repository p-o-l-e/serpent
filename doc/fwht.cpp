#include <iostream>
#include <bitset>
// In-place Fast Walsh–Hadamard Transform of array a.

struct hadamard
{
    int a[16] = {   0, 0, 1, 1,
                    1, 0, 0, 0,
                    0, 1, 0, 0,
                    1, 0, 0, 1 };
                
    int w = 16;
    int h = 1;

    void fwht()
    {
        while (h < w)
        {
            for(int i=0; i<w; i+= h*2)
            {
                for(int j=i; j<i+h; j++)
                {
                    int x = a[j];
                    int y = a[j + h];
                    a[j] = x + y;
                    a[j + h] = x - y;
                }
            }
            h *= 2;
        }
    }
};


int main ()
{
    hadamard h;
    h.fwht();
    for(int j=1; j<=4; j++)
    {
        for(int i=1; i<=4; i++)
        {
            std::cout<<h.a[j*i-i]<<" ";
        }
        std::cout<<"\n";
    }

    return 0;
}