#ifndef FFT_H
#define FFT_H

typedef struct complex_t {
    float real;
    float imag;
} complex_t;


void fft(complex_t *x, int n);
void fft_print(complex_t *x, int n, float sample_rate);
float cabs(complex_t num);
complex_t cvalue(float real, float imag);
complex_t cadd(complex_t num1, complex_t num2);
complex_t cdiff(complex_t num1, complex_t num2);
complex_t cmultiply(complex_t num1, complex_t num2);
complex_t cexp(complex_t num);

#endif