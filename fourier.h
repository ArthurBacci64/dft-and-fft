#ifndef FOURIER
#define FOURIER

typedef double fl;

struct complex
{
    fl real;
    fl imag;
};

struct fourier
{
    unsigned int freq;
    fl           ampl;
    fl           bias;
};

struct fourier *dft(fl *x, int N);

int main();

#endif
