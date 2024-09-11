/*! 
 *  @file casadifunction.cpp
 *  @brief Example code for casadi function
 *  @author Sunhong Kim (tjsghd101@naver.com)
 *  @data Sep. 21. 2023
 *  @Comm
 */


#include <casadi/casadi.hpp>
#include <iostream>
#include <Eigen/Dense>
#include <chrono>

#ifndef CASADI_RESOURCE_PATH
#define CASADI_RESOURCE_PATH ""
#endif


using namespace std;
using namespace Eigen;
using namespace std::chrono;
int main() {

    // CasADi Function Load.
    string casadi_file_path = CASADI_RESOURCE_PATH;
    casadi_file_path += "G.casadi";
    casadi::Function GravOpt_Wrench = casadi::Function::load(casadi_file_path);
    
    // Function information
    // Function(GravOpt_Wrench:(J[6x7],Gvec[7],R_eef[3x3],W_tau[7x7],W_gamma[3x3],tau_lim,gamma_llim,gamma_ulim)->(tau_opt[7],gamma_space_opt[3]) MXFunction)
    
    Matrix<double,6,1> q;

    // Initialize Input argumets
    q << 0,         1,   0,         0,   0,         0;


    casadi::DM q_casadi = casadi::DM::zeros(q.rows(), q.cols());
    
    memcpy(q_casadi.ptr(), q.data(), q.size() * sizeof(double));
    

    vector<casadi::DM> inputs = {q_casadi};
    system_clock::time_point StartTime = system_clock::now();
    vector<casadi::DM> outputs = GravOpt_Wrench(inputs);
    system_clock::time_point EndTime = system_clock::now();
    microseconds micro = duration_cast<microseconds>(EndTime - StartTime);
    
    cout << "function time: " << micro.count()/1000.0 << "ms" << endl;
    // cout << "input: " << inputs << endl;
    cout << "tau_G: " << outputs[0] << endl;

    return 0;
}