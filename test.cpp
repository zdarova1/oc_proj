#include <cstdio>
#include <omp.h>
#include "complex.hpp"
#include <ctime>
#include <immintrin.h>


int main(){
    omp_set_num_threads(4);
    #pragma omp parallel
    {
        for(int i = 0; i < 10; ++i)
            printf("thread - %d\n", omp_get_thread_num());
    }

    // for(auto& x: v_z){
    //     printComplex(x);
    // }

    return 0;
}