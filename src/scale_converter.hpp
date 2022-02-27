////////////////////////////////////////////////////////////////////////////////////////
// SCALE CONVERTER
// V.0.0.2  09-01-2022 
////////////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>
#include "constants.hpp"

#ifndef __SCALE_CONVERTER
#define __SCALE_CONVERTER

//  TODO: Hz to Notes
using std::vector;
using std::string;

vector<string> chromatic {"A","A#","B","C","C#","D","D#","E","F","F#","G","G#"};
string get_scale(vector<bool> notes, vector<int> octaves)
{
    string r("");
    for(int i= 0; i<12; i++)
    {
        if(notes[i])
        {
            r+=chromatic[i];
            r+=std::to_string(octaves[i]);
        }
    }
    return r;
}

vector<float> string_to_hz(const string & notes, float A4= 440.0, int MIDI_N=69)
{
    vector<float> list {};
    string::const_iterator i = notes.cbegin();

    while (i!=notes.cend()){
        int St = 0; 
        int Oct= 1;
        switch (*i)
        {
            case ' ' :           i++; break;
            case 'C' : St=0 ;    i++; break;
            case 'D' : St=2 ;    i++; break;
            case 'E' : St=4 ;    i++; break;
            case 'F' : St=5 ;    i++; break;
            case 'G' : St=7 ;    i++; break;
            case 'A' : St=9 ;    i++; break;
            case 'B' : St=11;    i++; break;
            default  :                break;
        } 
    bool on=true;
    while (on)
    {
        switch(*i)
        {
            case ' ' :           i++; break;
            case '#' : St++ ;    i++; break;
            case 'b' : St-- ;    i++; break;
            case '-' : Oct=-Oct; i++; break;
            default  : on=false;      break;        
        }
    }

    (((*i-'0')>=0)&&((*i-'0')<=9))? Oct*=(*i-'0'): 0;
    i++;
    while (isdigit(*i))
    {
        Oct*=10;
        Oct>=0? Oct+=(*i-'0') : Oct-=(*i-'0');
        i++;
    }
        float d = St + (Oct+1)*12 - MIDI_N;
        float f = A4 * pow( 2, d/12);  
        list.push_back(f);
    }
    return list;
}

#endif