////////////////////////////////////////////////////////////////////////////////////////
// SCALE MANIPULATION ROUTINES
// V.0.0.1 11-2021
////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SCALER 
#define __SCALER
#include <vector>
#include <cstdlib>

using std::abs;
using std::vector;

class scale
{
    private:
        uint                    wdth;
        mutable unsigned        octave;
        vector<bool>            intervals;
        vector<bool>  bin_to_vector(uint);

    public:
        uint  width()                const           { return wdth;      }
        vector<bool>  getintervals() const           { return intervals; }
        inline void   setintervals(vector<bool> t)   { intervals= t; wdth= intervals.size(); }   
        inline void   setintervals(uint t);
        void          reverse()                      { intervals= vector<bool> (intervals.rbegin(), intervals.rend()); }
        void          shift(int n);
        vector<float> getfrequencies(uint O)    const;
        vector<float> getfrequencies()          const;
        scale()              : wdth(12) , intervals(wdth,1) {};
        scale(vector<bool> t): intervals(t) {};
        scale(uint t);
        scale(vector<int> s);
        ~scale(); 
};

void scale::shift(int n){
        vector<bool> r = {};
        int sign = (n>=0? 1: -1);
        (abs(n)>=wdth)? n=abs(n)%wdth: n;
        int car= abs(n);

        if(n){
        if(sign==-1) {
            for (int i=0; i< wdth; i++)
            {
                car==wdth? car=0: car;
                r.push_back(intervals[car]);
                car++;
            }
        }
        if(sign==1) {
            car= wdth- abs(n);
            for (int i=0; i< wdth; i++)
            {  
                car==wdth? car=0: car;
                r.push_back(intervals[car]);
                car++;
            }
        }
        intervals= r;
        }        
}

vector<bool> scale::bin_to_vector(uint t){
        vector<bool> r = {};
        uint s= 0;
        int mask= 0b1;
        while (t)
        {
            s++;
            r.push_back(t&mask);
            t>>=1;
        }
        return vector<bool> (r.rbegin(),r.rend());
}

scale::scale(uint t){
        intervals= bin_to_vector(t);
        wdth= intervals.size();
}

inline void scale::setintervals(uint t){
        intervals= bin_to_vector(t);
        wdth= intervals.size();
}



/*
#include <iostream>
int main(void){

    scale *s= new scale;

        s->setintervals(0b111100001111);
    vector<bool> in= s->getintervals();
    for (auto i: in)
    {
        std::cout<<i<<" ";
    }

   
    std::cout<<" : "<<s->width()<< "\n";
     s->shift(-1);
    std::cout<<"\n";
    //s->reverse();
    in= s->getintervals();

    for (auto i: in)
    {
        std::cout<<i<<" ";
    }
    
    std::cout<<"\n";
    //std::cout<<s->width();

    std::vector<double> ff {};
    ff= f_list("A4");
    for(auto i: ff) std::cout<<i<<" ";
    
    return 0;
}
*/

#endif