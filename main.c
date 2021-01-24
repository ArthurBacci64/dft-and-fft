#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "show.h"
#include "fourier.h"

struct fourier *dft(fl *x, int N)
{
    struct fourier *X = malloc(N * sizeof(struct fourier));

    for (unsigned int k = 0; k < N; k++)
    {
	struct complex num = {0, 0};
	for (unsigned int n = 0; n < N; n++)
	{
	    num.real += x[n] * cos((2 * M_PI * k * n) / N);
	    num.imag -= x[n] * sin((2 * M_PI * k * n) / N);
	}
	num.real /= N;
	num.imag /= N;

	X[k].freq = k;
	X[k].ampl = sqrt(num.real * num.real + num.imag * num.imag);
	X[k].bias = atan2(num.imag, num.real);
	
    }
    return X;
}

struct complex *fft(struct complex *x, int N)
{
    if (N == 1)
    {
	return x;
    }
    
    struct complex *X  = malloc(N     * sizeof(struct complex));
    
    struct complex *xa = malloc(N / 2 * sizeof(struct complex));
    struct complex *xb = malloc(N / 2 * sizeof(struct complex));

    for (unsigned int i = 0; i < N / 2; i++)
    {
	xa[i].real = x[2 * i    ].real;
	xa[i].imag = x[2 * i    ].imag;
	xb[i].real = x[2 * i + 1].real;
	xb[i].imag = x[2 * i + 1].imag;
    }

    struct complex *Xa = fft(xa, N / 2);
    struct complex *Xb = fft(xb, N / 2);

    const struct complex wn =
	{
	    cos(2 * M_PI / (double)N),
	    -sin(2 * M_PI / (double)N)
	};
    
    struct complex w = {1, 0};

    for (unsigned int i = 0; i < N / 2; i++)
    {
	const struct complex multi =
	    {
		w.real * Xb[i].real - w.imag * Xb[i].imag,
		w.real * Xb[i].imag + w.imag * Xb[i].real
	    };

	X[i        ].real = Xa[i].real + multi.real;
	X[i        ].imag = Xa[i].imag + multi.imag;
	X[i + N / 2].real = Xa[i].real - multi.real;
	X[i + N / 2].imag = Xa[i].imag - multi.imag;

	fl temp = w.real;
	w.real = w.real * wn.real - w.imag  * wn.imag;
	w.imag = temp   * wn.imag + wn.real * w.imag;
    }


    free(xa);
    free(xb);

    // If N <= 3, then xa == Xa and xb == Xb
    if (N > 3)
    {
	free(Xa);
	free(Xb);
    }
    
    return X;
}

int main()
{
    fl inputs[10];
    
    for (unsigned int i = 0; i < 10; i++)
    {
        inputs[i] = (fl)i / (fl)10;
    }
    
    const int len = sizeof(inputs) / sizeof(fl);
    
    //struct fourier *d = dft(inputs, len);

    
    struct complex complex_inputs[len];

    for (unsigned int i = 0; i < len; i++)
    {
	complex_inputs[i].real = inputs[i];
	complex_inputs[i].imag = 0;
    }

    struct complex *f = fft(complex_inputs, len);

    
    for (unsigned int i = 0; i < len; i++)
    {
	if (i < len - 1)
	{
	    printf("cos(x * %d + %f) * %f + ", i, atan2(f[i].imag, f[i].real), sqrt(f[i].real * f[i].real + f[i].imag * f[i].imag));
	}
	else
	{
	    printf("cos(x * %d + %f) * %f\n\n", i, atan2(f[i].imag, f[i].real), sqrt(f[i].real * f[i].real + f[i].imag * f[i].imag));
	}
    }
    

    for (unsigned int i = 1; i < len; i++)
    {
	printf("%d -> %f\n", i, sqrt(f[i].real * f[i].real + f[i].imag * f[i].imag));
    }

    //show(d, len);
    
    //free(d);
    free(f);
    
    return 0;
}

