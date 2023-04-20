#include <stdio.h>
#include "sort.hpp"
#include <ctime>
#include <string>
#include <algorithm>
// ./main  < func_name > < vector size > < threads>




int main(int argc, char** argv){
    int size, threads_number;
    if(argc < 4){
        printf("Use: ./main  < func_name > < vector size > < threads> to run\n");
        return 1;
    }
    srand(time(0));
    double prog_start = omp_get_wtime(), start = omp_get_wtime();
    std::string func_name(argv[1]);
    size = std::stoi(std::string(argv[2]));
    threads_number = std::stoi(std::string(argv[3]));
    
    std::vector<Complex> v_z(size);
    std::vector<Complex> v_t(size); 
    omp_set_nested(1);
    omp_set_num_threads(size);
    omp_set_dynamic(size);

    genvecComplex(v_z);
    genvecComplex(v_t);
    
    //printVectorComplex(v_z);
    //printVectorComplex(v_t);
    

    if(func_name == "SeqMergeSort"){
        start = omp_get_wtime();
        SeqMergeSort(0, size, v_z);
        printf("Executing time: %6.4lf", omp_get_wtime() - start);
    }
    else if(func_name == "ParMergeSort1"){
        start = omp_get_wtime();
        ParMergeSort1(0, size, threads_number, v_z);
        printf("Executing time: %6.4lf", omp_get_wtime() - start);
    }
    else if(func_name == "ParMergeSort2"){
        start = omp_get_wtime();
        ParMergeSort2(0, size, threads_number, v_z);
        printf("Executing time: %6.4lf", omp_get_wtime() - start);

    }
    else if(func_name == "ParMergeSort3"){     
        start = omp_get_wtime();
        ParMergeSort3(0, size, threads_number, v_z);
        printf("Executing time: %6.4lf", omp_get_wtime() - start);
    }
    else if(func_name == "complexVecSum"){
        start = omp_get_wtime();
        complexVecSum(v_z, v_t);
        printf("Executing time: %6.4lf", omp_get_wtime() - start);
    }
    else if(func_name == "complexVecSumPar"){
        start = omp_get_wtime();
        complexVecSumPar(v_z, v_t, threads_number);
        printf("Executing time: %6.4lf", omp_get_wtime() - start);
    }
    
    else if(func_name == "complexVecSumSimd"){
        
        start = omp_get_wtime();

        complexVecSumSimd(v_z, v_t);
        printf("Executing time: %6.4lf", omp_get_wtime() - start);
    }
    else if(func_name == "complexVecSumSimdPar"){
        
        start = omp_get_wtime();

        complexVecSumSimdPar(v_z, v_t, threads_number);
        printf("Executing time: %6.4lf\n", omp_get_wtime() - start);

    }
    else if(func_name == "complexVecMul"){
        //ParMergeSort3(0, size, threads_number, v_z);
        start = omp_get_wtime();
        complexVecMul(v_z, v_t);
        printf("Executing time: %6.4lf", omp_get_wtime() - start);
    }
    else if(func_name == "complexVecMulSimd"){
        //ParMergeSort3(0, size, threads_number, v_z);
        start = omp_get_wtime();
        complexVecMulSimd(v_z, v_t);
        printf("Executing time: %6.4lf", omp_get_wtime() - start);
    }
    else if(func_name == "complexVecMulSimdPar"){
        //ParMergeSort3(0, size, threads_number, v_z);
        start = omp_get_wtime();
        complexVecMulSimdPar(v_z, v_t, threads_number);
        printf("Executing time: %6.4lf\n", omp_get_wtime() - start);
    }
    else if(func_name == "complexVecMulPar"){
        //ParMergeSort3(0, size, threads_number, v_z);
        start = omp_get_wtime();
        complexVecMulPar(v_z, v_t, threads_number);
        printf("Executing time: %6.4lf\n", omp_get_wtime() - start);
    }
    
    
    //printVectorComplex(v_z);
    


    return 0;
}