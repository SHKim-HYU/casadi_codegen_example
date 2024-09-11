#ifndef CASADI_CODEGEN_H_
#define CASADI_CODEGEN_H_

#include <stdio.h>
#include "iostream"
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <Eigen/Dense>

#include <chrono>
#include <iostream>

#include <dlfcn.h>


using namespace std;
using namespace Eigen;
using namespace std::chrono;

typedef long long int casadi_int;

typedef void (*signal_t)(void);
typedef casadi_int (*getint_t)(void);
typedef int (*work_t)(casadi_int* sz_arg, casadi_int* sz_res, casadi_int* sz_iw, casadi_int* sz_w);
typedef const casadi_int* (*sparsity_t)(casadi_int ind);
typedef int (*eval_t)(const double** arg, double** res, casadi_int* iw, double* w, int mem);
typedef int (*casadi_checkout_t)(void);
typedef void (*casadi_release_t)(int);

void* F_handle;
eval_t F_eval;



const casadi_int sz_arg = 119; // Total size of input size
const casadi_int sz_res = 13; // Total size of output size
const casadi_int sz_iw = 11;
const casadi_int sz_w = 968;

const double* cs_arg[sz_arg];
double* cs_res[sz_res];
casadi_int cs_iw[sz_iw];
double cs_w[sz_w];
int cs_mem = 0;

// Set input values
double input_values[sz_arg];

// Set output buffers
double output_values[sz_res];

#endif  // /* CASADI_CODEGEN_H_ */