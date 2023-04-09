#pragma once
#include "complex.hpp"
#include <omp.h>
#include <vector>

void SeqMergeSort(int l, int r, std::vector<Complex> &z);
//void ParMergeSort();
void ParMergeSort1(int l, int r, int Threads, std::vector<Complex> &z);
void ParMergeSort2(int l, int r, int Threads, std::vector<Complex> &z);
void ParMergeSort3(int l, int r, int Threads, std::vector<Complex> &z);