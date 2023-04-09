#include <cstdio>
#include <cmath>
#include <vector>
#include <immintrin.h>
#include <omp.h>

struct Complex
{
    double re, im;
};

void genvecComplex(std::vector<Complex> &z);
double ComplexNorm(Complex &z);

bool ComplexNormComp(Complex &z1, Complex &z2);
//Complex* VectorSum(Complex *z1, Complex *z2);
void printComplex(Complex &z);

void complexVecSum(std::vector<Complex> &z1, std::vector<Complex> &z2);
void complexVecSumPar(std::vector<Complex> &z1, std::vector<Complex> &z2, int Threads);
void complexVecSumSimd(std::vector<Complex> &z1, std::vector<Complex> &z2);
void complexVecSumSimdPar(std::vector<Complex> &z1, std::vector<Complex> &z2, int Threads);

void complexVecMul(std::vector<Complex> &z1, std::vector<Complex> &z2);
void complexVecMulSimd(std::vector<Complex> &z1, std::vector<Complex> &z2);

void complexVecMulSimdPar(std::vector<Complex> &z1, std::vector<Complex> &z2, int Threads);