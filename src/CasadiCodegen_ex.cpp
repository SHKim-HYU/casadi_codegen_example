#include "CasadiCodegen_ex.h"

#ifndef CASADI_RESOURCE_PATH
#define CASADI_RESOURCE_PATH ""
#endif

int main(int argc, char *argv[])
{
    string func_path = CASADI_RESOURCE_PATH;
    func_path += "GravOpt_Wrench.so";
    
	// Function load
	F_handle = dlopen(func_path.c_str(), RTLD_LAZY);
    if (!F_handle) {
        throw std::runtime_error("Cannot open GravOpt_Wrench.so");
    }

    // Function evaluation
    F_eval = (eval_t)dlsym(F_handle, "GravOpt_Wrench");
    if (!F_eval) {
        throw std::runtime_error("Function evaluation failed.");
    }

	// Input parameters
	double input_J[6*7];
	double input_Gvec[7];
	double input_R[3*3];
	double input_W_tau[7*7];
	double input_W_gamma[3*3];
	double tau_lim, gamma_ulim, gamma_llim;

	// Mapping args 
	// Row fill firstly
	Map<Matrix<double,6,7>> J(input_J);
	Map<Matrix<double,7,1>> Gvec(input_Gvec);
	Map<Matrix3d> R_eef(input_R);
	Map<Matrix<double,7,7>> W_tau(input_W_tau);
	Map<Matrix<double,3,3>> W_gamma(input_W_gamma);

	// Output parameters
	double output_tau_opt[7];
	double output_gamma_space_opt[7];
	Matrix<double,7,1> tau_opt;
	Matrix<double,3,1> gamma_space_opt;

	// Initialize Input argumets
	J << 0.5555,         0,   -0.5904,         0,   -0.2150,         0,   0.0000,
         -0.1294,   -0.8103,   -0.2588,   -0.2150,   -0.0000,   -0.2150,         0,
          0.1294,    0.0247,   -0.2588,   -1.0500,    0.1830,         0,         0,
               0,   -1.0000,         0,    1.0000,         0,    1.0000,         0,
         -0.7071,         0,    0.7071,         0,   -1.0000,         0,         0,
         -0.7071,         0,   -0.7071,         0,   -0.0000,         0,    1.0000;
    Gvec << 0.0000, 369.7407, 61.0699, 128.3092, -11.2580, 3.9898, 0;
    R_eef << 1.0000, 0, 0,
              0,   -0.7071,    0.7071,
              0,   -0.7071,   -0.7071;
	tau_lim = 80;
	gamma_llim = -500;
	gamma_ulim = 500;
	W_tau = Matrix<double,7,7>::Identity()/pow(tau_lim,2);
	W_gamma = Matrix<double,3,3>::Identity()/pow(gamma_ulim,2);
	
	// Function(GravOpt_Wrench:(J[6x7],Gvec[7],R_eef[3x3],W_tau[7x7],W_gamma[3x3],tau_lim,gamma_llim,gamma_ulim)->(tau_opt[7],gamma_space_opt[3]) MXFunction)
	memcpy(input_values, input_J, 6*7*sizeof(double));
	memcpy(input_values + 42, input_Gvec, 7*sizeof(double));
	memcpy(input_values + 49, input_R, 3*3*sizeof(double));
	memcpy(input_values + 58, input_W_tau, 7*7*sizeof(double));
	memcpy(input_values + 107, input_W_gamma, 3*3*sizeof(double));
	input_values[116]=tau_lim; input_values[117]=gamma_llim; input_values[118]=gamma_ulim;
	
	// set buffer
    for (casadi_int i = 0; i < sz_arg; ++i) {
        cs_arg[i] = &input_values[i];
    }

	
    for (casadi_int i = 0; i < sz_res; ++i) {
        cs_res[i] = &output_values[i];
    }

	if (F_eval(cs_arg, cs_res, cs_iw, cs_w, cs_mem)) {
        throw std::runtime_error("Function evaluation failed.\n");
    }
	cout<<"debug"<<endl;
    return 0;
}

