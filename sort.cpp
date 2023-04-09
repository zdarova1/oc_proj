#include "sort.hpp"



static void merge(int l, int mid, int r, std::vector<Complex> &z){
    std::vector<Complex> res;
    int i = l, j = mid;
    while(i < mid && j < r){
        if(ComplexNormComp(z[i], z[j])){
            res.push_back(z[i++]);
        }
        else{
            res.push_back(z[j++]);
        }
    }
    while (i < mid)
    {
        res.push_back(z[i++]);
    }
    while (j < r)
    {
        res.push_back(z[j++]);
    }

    for(i = l, j = l; i < r; ++i)
        z[i] = res[i-j];
}

void SeqMergeSort(int l, int r, std::vector<Complex> &z){
    if(l + 1 >= r)
        return;
    int mid = (r-l)/2+l;
    SeqMergeSort(l, mid, z);
    SeqMergeSort(mid, r, z);
    merge(l, mid, r, z);
}
// || z1 || < || z2 || <=> x1^2+y1^2 < x2^2+y2^2 <=> (x1-x2)(x1+x2)
void ParMergeSort1(int l, int r, int Threads, std::vector<Complex> &z){
    if(l + 1 >= r)
        return;
    int mid = (r-l)/2+l;
    
    if(Threads <= 1){
        SeqMergeSort(l, r, z);
        //SeqMergeSort(mid, r, z);
    }
    else if(Threads > 1){

    double begin = omp_get_wtime();
#pragma omp parallel sections 
{
    #pragma omp section
    {
        ParMergeSort1(l, mid, Threads/2, z);
        //printf("sect1:  thread - %d, %5.3lf\n", omp_get_thread_num(), omp_get_wtime() - begin);
    }
    #pragma omp section
    {
        ParMergeSort1(mid, r, Threads-Threads/2, z);
        //printf("sect2: thread - %d, %5.3lf\n", omp_get_thread_num(), omp_get_wtime() - begin);
    }
    
}

    }

    merge(l, mid, r, z);
}

void ParMergeSort2(int l, int r, int Threads, std::vector<Complex> &z){
    if(l + 1 >= r)
        return;
    int mid = (r-l)/2+l;
    
    if(Threads <= 1){
        SeqMergeSort(l, r, z);
        //SeqMergeSort(mid, r, z);
    }
    else if(Threads > 1){

    double begin = omp_get_wtime();



    // (l:r) - (l;mid) & (mid: r) - (l: mid+l / 2) & (mid+l/2: mid) & (mid: mid+r/ 2) & (mid+r / 2; r) 
#pragma omp parallel sections 
{
    #pragma omp section
    {
        ParMergeSort2(l, (mid+l)/2, Threads/4, z);
        //printf("sect1:  thread - %d, %5.3lf\n", omp_get_thread_num(), omp_get_wtime() - begin);
    }
    #pragma omp section
    {
        ParMergeSort2((mid+l)/2, mid, Threads/4, z);
        //printf("sect2: thread - %d, %5.3lf\n", omp_get_thread_num(), omp_get_wtime() - begin);
    }
    #pragma omp section
    {
        ParMergeSort2(mid, (mid+r)/2, Threads/4, z);
        //printf("sect2: thread - %d, %5.3lf\n", omp_get_thread_num(), omp_get_wtime() - begin);
    }
    #pragma omp section
    {
        ParMergeSort2((mid+r)/2, r, Threads-Threads/4*3, z);
        //printf("sect2: thread - %d, %5.3lf\n", omp_get_thread_num(), omp_get_wtime() - begin);
    }
    
    
}

    }

    merge(l, mid, r, z);
}

void ParMergeSort3(int l, int r, int Threads, std::vector<Complex> &z){
    if(l+1 >= r)
        return;

    int mid = (r-l)/2+l;
    
    if(Threads <= 1){
        SeqMergeSort(l,r,z);
    }
    else
    {
    #pragma omp parallel
    {
        #pragma omp single 
        {
            #pragma omp task shared(z) 
            {
                ParMergeSort3(l, mid, Threads/2, z);
            }
            #pragma omp task shared(z)
            {
                ParMergeSort3(mid, r, Threads-Threads/2, z);
            }
            #pragma omp taskwait
            {
                merge(l, mid, r, z);
            }
        }

    }

    }


}