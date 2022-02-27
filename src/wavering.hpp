////////////////////////////////////////////////////////////////////////////////////////
// Circular Audio Buffer 
// V.0.1.1 2022-01-30
////////////////////////////////////////////////////////////////////////////////////////
#ifndef __WAVERING
#define __WAVERING

#include <vector>
#include <cstdlib>
using std::vector;


class circular
{
    private:
        float*          data;
        const int       size;           // Buffer size
        int             current;        // Write cursor
        int             prior;          // Read cursor
        inline void     next(void);     // Next value
        
    public:
        inline float*   get();                          // Return buffer
        inline float    at();                           // Return current value
        inline float    at(const int&);                 // Return value at given position
        inline void     set(const float&);              // Fill current value
        inline void     add(const float&, const int&);  // Fill value at shifted position
        inline void     clear();                        // Clear buffer
        inline int      length();                       // Get size
        circular(int);
        ~circular();
};

inline void circular::next()
{
        current++;
        prior = current - 1;
        if(current==size)
        {
            current = 0;
            prior = size;
        }
}

inline float* circular::get()
{
        return data;
}

inline float circular::at()
{
        return data[prior];
}

inline float circular::at(const int& i)
{
        int pos = prior+i;
        if(pos>size) pos%=size;
        return data[pos];
}

inline void circular::set(const float& d)
{
        data[current] = d;
        next();
}

inline void circular::add(const float& d, const int& shift)
{
        int p = ((current+shift)>=size? (current+shift)%size: (current+shift));
        data[p] += d;
}


inline void circular::clear()
{
        for(int i= 0; i< size; i++)
        {
            data[i]= 0.0f;
        }
}

inline int  circular::length()
{
        return size;
}

circular::circular(int s): size(s)
{ 
        data = new float[size];
        clear();
        current = 0;
}

circular::~circular() 
{
        delete data;    
}



////////////////////////////////////////////////////////////////////////////////////////
// Wavering ////////////////////////////////////////////////////////////////////////////

class wavering
{
    private:
        std::vector<float*>     data;
        int                     width;     // Number of frames
        int                     size;      // Buffer size
        int                     current;   // Frame counter
        int                     i;         // Sample counter
        
    public:
        inline float *at();                // Return current frame
        inline float *shift(int);          // Return frame at given position
        inline void set (float*);          // Fill current frame
        inline void set (float*, int);     // Fill frame at give position
        inline void next ();               // Next frame
        inline void prior();               // Previous frame
        inline void clear();               // Clear current frame
        inline void flush();               // Clear buffer
        inline int  frames_count();
        inline int  buffer_size();
        wavering(const int&, const int&);
        ~wavering();
};

inline float *wavering::at()
{
        return data[current];
}

inline float *wavering::shift(int position)
{
        int f= current + position;
        f>=width? f%=width: f;
        return data[f];
}

inline void wavering::set (float* d)
{
        for(int i= 0; i< size; i++)
        {
            data[current][i]=d[i];
        }
}

inline void wavering::set (float* d, int position)
{
        int f= position;
        f>=width? f%=width: f;
        for(int i= 0; i< size; i++)
        {
            data[f][i]=d[i];
        }
}

inline void wavering::next() 
{ 
        current++;
        current==width? current= 0: current;
}

inline void wavering::prior() 
{ 
        current--;
        current<0? current= width: current;
}

inline void wavering::clear()
{
        for(int i= 0; i< size; i++)
        {
            data[current][i]= .0;
        }
}

inline void wavering::flush()
{
        for(int i= 0; i< width; i++)
        {
            this->next();
            this->clear();
        }
}

inline int  wavering::frames_count()
{
        return width;
}
inline int  wavering::buffer_size()
{
        return size;
}

wavering::wavering(const int& bsize, const int& nframes)
{ 
        width= nframes;
        size = bsize;
        data.reserve(width);
        for(int i=0; i< width; i++)
        {
            data.push_back(new float[size]);
        }
        current= 0;
        i= 0;
}

wavering::~wavering() 
{
        for(int i=0; i< width; i++)
        {
            delete data[i];
        }
}

#endif /////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
