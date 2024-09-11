#include "CasadiCodegen_ex.h"

#ifndef CASADI_RESOURCE_PATH
#define CASADI_RESOURCE_PATH ""
#endif

int main(int argc, char *argv[])
{
    string func_path = CASADI_RESOURCE_PATH;
    func_path += "G.so";
    
	// Function load
	F_handle = dlopen(func_path.c_str(), RTLD_LAZY);
    if (!F_handle) {
        throw std::runtime_error("Cannot open G.so");
    }

    // Function evaluation
    F_eval = (eval_t)dlsym(F_handle, "generalized_gravity");
    if (!F_eval) {
        throw std::runtime_error("Function evaluation failed.");
    }
	// Allocate input/output buffers and work vectors
    casadi_int sz_arg = 6;
    casadi_int sz_res = 6;
    casadi_int sz_iw = 0;
    casadi_int sz_w = 0;

    const double* arg[sz_arg];
    double* res[sz_res];
    casadi_int iw[sz_iw];
    double w[sz_w];

	double _q[6] = {0,1,0,0,0,0};

    // Set input values
    double input_values[sz_arg];
    for (casadi_int i = 0; i < sz_arg; ++i) {
        input_values[i] = _q[i];
        arg[i] = &input_values[i];
    }

    // Set output buffers
    double output_values[sz_res]; // 6x6 matrix
    for (casadi_int i = 0; i < sz_res; ++i) {
        res[i] = &output_values[i];
    }

    // Evaluate the function
    int mem = 0;  // No thread-local memory management
    system_clock::time_point StartTime = system_clock::now();
    if (F_eval(arg, res, iw, w, mem)) {
        throw std::runtime_error("Function evaluation failed.\n");
    }
	system_clock::time_point EndTime = system_clock::now();
    microseconds micro = duration_cast<microseconds>(EndTime - StartTime);
    
    cout << "function time: " << micro.count()/1000.0 << "ms" << endl;
    cout<<output_values[0]<<","<<output_values[1]<<","<<output_values[2]<<","<<output_values[3]<<","<<output_values[4]<<","<<output_values[5]<<endl;

}