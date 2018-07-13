#ifndef FOURIER_H
#define FOURIER_H
#include <math.h>
#include <stdlib.h>
#define VOID 0
#define UCHAR 1
#define CHAR 2
#define INT 3
#define FLOAT 4
#define DOUBLE 5

#ifndef M_PI
#define M_PI 3.14159265358979323846
#define W_PI 57.29577951308233
#endif

#define SIZE 16*16
#define VALUE_MAX 100

struct Complex_{
    double real;
    double imagin;
};
typedef struct Complex_ Complex;

void DFT(double * src,Complex * dst,int size);
void IDFT(Complex *src,Complex *dst,int size);
int FFTReal_remap(double * src,int size_n);
int FFTComplex_remap(Complex * src,int size_n);
void RealFFT(double * src,Complex * dst,int size_n);
void FFT(Complex * src,Complex * dst,int size_n);
void IFFT(Complex * src,Complex * dst,int size_n);
void ColumnVector(Complex * src,Complex * dst,int size_w,int size_h);
int DFT2D(double *src,Complex *dst,int size_w,int size_h);
int IDFT2D(Complex *src,Complex *dst,int size_w,int size_h);
int FFT2D(double *src,Complex *dst,int size_w,int size_h);
int IFFT2D(Complex *src,Complex *dst,int size_w,int size_h);
int isBase2(int size_n);
#endif // FOURIER_H
