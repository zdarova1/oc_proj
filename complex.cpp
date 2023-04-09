#include "complex.hpp"

static const int k = 256;
static auto rand_f = []() -> float
{ return static_cast<float>(rand() * 1.0f / (RAND_MAX / k)) * pow(-1, rand()); };

double ComplexNorm(Complex &z) { return sqrt(z.re * z.re + z.im * z.im); }

void printComplex(Complex &z) { printf("Real: %4.2lf, Im: %4.2lfi\n", z.re, z.im); }

bool ComplexNormComp(Complex &z1, Complex &z2)
{
    return (z1.re * z1.re + z1.im * z1.im) <= (z2.re * z2.re + z2.im * z2.im);
}

void genvecComplex(std::vector<Complex> &z)
{
    for (auto &z_ : z)
    {
        z_.re = rand_f();
        z_.im = rand_f();
    }
}
static int min_(int a, int b) { return a < b ? a : b; }

void complexVecSumPar(std::vector<Complex> &z1, std::vector<Complex> &z2, int Threads)
{
    int i, size = min_(z1.size(), z2.size());
    omp_set_num_threads(Threads);
// omp_set_dynamic(4);

// #pragma omp shared(z1, z2)

#pragma omp parallel 
    {
        #pragma omp for
        for (i = 0; i < size; ++i)
        {
            z1[i].re += z2[i].re;
            z1[i].im += z2[i].im;
            // printf("thread: %d\n", omp_get_thread_num());
        }
    }
}

void complexVecSum(std::vector<Complex> &z1, std::vector<Complex> &z2)
{
    int size = min_(z1.size(), z2.size());
    omp_set_num_threads(10);
    // omp_set_dynamic(4);

    // #pragma omp shared(z1, z2)

    for (int i = 0; i < size; ++i)
    {
        z1[i].re += z2[i].re;
        z1[i].im += z2[i].im;
        // printf("thread: %d\n", omp_get_thread_num());
    }
}
void complexVecSumSimd(std::vector<Complex> &z1, std::vector<Complex> &z2)
{
    int size = min_(z1.size(), z2.size()), i = 0;
    for (i = 0; i + 2 <= size; i += 2)
    {
        __m256d z1_4word_, z2_4word_, res; // 4*64=256

        z1_4word_ = _mm256_loadu_pd(&(z1[i].re));
        z2_4word_ = _mm256_loadu_pd(&(z2[i].re));

        // z1_4word_im = _mm256_loadu_pd(&(z1[i].im));
        // z2_4word_im = _mm256_loadu_pd(&(z2[i].im));

        res = _mm256_add_pd(z1_4word_, z2_4word_);
        // res_im = _mm256_add_pd(z1_4word_im, z2_4word_im);

        // printf("_______________________-\n");
        _mm256_storeu_pd(&(z1[i].re), res);
        //_mm256_storeu_pd(&(z1[i].im), res_im);
    }
    for (i; i < size; ++i)
    {
        z1[i].re += z2[i].re;
        z1[i].im += z2[i].im;
    }
}

void complexVecSumSimdPar(std::vector<Complex> &z1, std::vector<Complex> &z2, int Threads)
{
    int size = min_(z1.size(), z2.size()), i = 0;
#pragma omp parallel 
    {
        #pragma omp for
        for (i; i + 2 <= size; i += 2)
        {
            __m256d z1_4word_, z2_4word_, res; // 4*64=256

            z1_4word_ = _mm256_loadu_pd(&(z1[i].re));
            z2_4word_ = _mm256_loadu_pd(&(z2[i].re));

            // z1_4word_im = _mm256_loadu_pd(&(z1[i].im));
            // z2_4word_im = _mm256_loadu_pd(&(z2[i].im));

            res = _mm256_add_pd(z1_4word_, z2_4word_);
            // res_im = _mm256_add_pd(z1_4word_im, z2_4word_im);

            // printf("_______________________-\n");
            _mm256_storeu_pd(&(z1[i].re), res);
            //_mm256_storeu_pd(&(z1[i].im), res_im);
        }
    }
    for (i; i < size; ++i)
    {
        z1[i].re += z2[i].re;
        z1[i].im += z2[i].im;
    }
}
void complexVecMul(std::vector<Complex> &z1, std::vector<Complex> &z2)
{
    // z1*z2 = (a1a2-b1b2)+(a1b2+a2b2)i
    int size = min_(z1.size(), z2.size());
    Complex a, b;
    for (int i = 0; i < size; ++i)
    {
        a.re = z1[i].re;
        a.im = z1[i].im;

        b.re = z2[i].re;
        b.im = z2[i].im;

        z1[i].re = a.re * b.re - a.im * b.im;
        z1[i].im = (a.re * b.im + a.im * b.re);
    }
}
void complexVecMulSimd(std::vector<Complex> &z1, std::vector<Complex> &z2)
{

    // [a.im, a.re]
    // [b.im, b.re]
    //
    Complex a, b;
    int i, size = min_(z1.size(), z2.size());
    __m256d z1_, z2_, res, mul_res1, mul_res2, z1_perm, z2_perm, mask, mul_masked, res_;
    mask = _mm256_set_pd(-1, 1, -1, 1);

    for (i = 0; i + 2 <= size; i += 2)      // a*b= are*bre - aim*bim + (are*bim + aim*bre)i
    {                                       //   3   2   1    0
        z1_ = _mm256_loadu_pd(&(z1[i].re)); // [ im3, re2, im1, re0]
        z2_ = _mm256_loadu_pd(&(z2[i].re)); // [ im3, re2, im1, re0]

        z1_perm = _mm256_permute4x64_pd(z1_, 0b10100000); // [ re2, re2, re0, re0]

        mul_res1 = _mm256_mul_pd(z1_perm, z2_); // z1*z2: [ re[2]*im[3], re[2]*re[2], re[0]*im[1], re[0]*re[0]]

        z2_perm = _mm256_permute4x64_pd(z2_, 0b11110101); //[im[3], im[3], im[1], im[1]]

        z1_perm = _mm256_permute4x64_pd(z1_, 0b10110001); // [re2, im3, re0, im1]
        // z1*z2: [im[3]re[2],im[3]*im[3] ,im[1]re[0],im[1]im[1]]
        mul_res2 = _mm256_mul_pd(z1_perm, z2_perm);

        mul_masked = _mm256_mul_pd(mul_res2, mask);

        res = _mm256_add_pd(mul_res1, mul_masked);
        // res_ = _mm256_permute4x64_pd(res, 0b10110001);
        _mm256_storeu_pd(&(z1[i].re), res_);
    }
    for (i; i < size; ++i)
    {
        a.re = z1[i].re;
        a.im = z1[i].im;

        b.re = z2[i].re;
        b.im = z2[i].im;

        z1[i].re = a.re * b.re - a.im * b.im;
        z1[i].im = (a.re * b.im + a.im * b.re);
    }
}