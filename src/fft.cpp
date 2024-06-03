#include <Arduino.h>
#include "fft.h"


void fft(complex_t *x, int n) {
    if (n <= 1) return;

    // Divide
   complex_t even[n/2];
   complex_t odd[n/2];
    for (int i = 0; i < n/2; i++) {
        even[i] = x[i*2];
        odd[i] = x[i*2 + 1];
    }

    // Conquer
    fft(even, n/2);
    fft(odd, n/2);

    // Combine
    for (int k = 0; k < n/2; k++) {
        // complex_t t = cexp(-2 * I * M_PI * k / n) * odd[k];
        complex_t t = cmultiply(cvalue(-2 * M_PI * k / n, 0.0f), cvalue(0.0f, 1.0f));
        t = cexp(t);
        t = cmultiply(t, odd[k]);
        x[k] = cadd(even[k], t);
        x[k +n/2] = cdiff(even[k], t);
    }
}

void fft_print(complex_t *x, int n, float sample_rate) {
    for(int i = 0; i < n/2+1; i++) {
        Serial.println("Frequency= " + String(i * sample_rate / n) = " Hz   Amplitude = " + String(cabs(x[i]) * 2 / n));
    }
}

float cabs(complex_t num) {
    return sqrt(num.real * num.real + num.imag * num.imag);
}

complex_t cvalue(float real, float imag) {
    complex_t result = {real, imag};
    return result;
}

complex_t cadd(complex_t num1, complex_t num2) {
    complex_t result;
    result.real = num1.real + num2.real;
    result.imag = num1.imag + num2.imag;
    return result;
}

complex_t cdiff(complex_t num1, complex_t num2) {
    complex_t result;
    result.real = num1.real - num2.real;
    result.imag = num1.imag - num2.imag;
    return result;
}

complex_t cmultiply(complex_t num1, complex_t num2) {
    complex_t result;
    result.real = num1.real * num2.real - num1.imag * num2.imag;
    result.imag = num1.real * num2.imag + num1.imag * num2.real;
    return result;
}

complex_t cexp(complex_t num) {
    complex_t result;
    complex_t tmp = cvalue(num.real, num.imag);
    result.real = pow(M_E, num.real) * cos(num.imag);
    result.imag = pow(M_E, num.real) * sin(num.imag);
    return result;
}
