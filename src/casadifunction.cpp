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
    casadi_file_path += "GravOpt_Wrench.casadi";
    casadi::Function GravOpt_Wrench = casadi::Function::load(casadi_file_path);
    
    // Function information
    // Function(GravOpt_Wrench:(J[6x7],Gvec[7],R_eef[3x3],W_tau[7x7],W_gamma[3x3],tau_lim,gamma_llim,gamma_ulim)->(tau_opt[7],gamma_space_opt[3]) MXFunction)
    
    Matrix<double,6,7> J;
    Matrix<double,7,1> Gvec;
    Matrix3d R_eef;
    double tau_lim = 80, gamma_llim = -500, gamma_ulim = 500;
    Matrix<double,7,7> W_tau;
    Matrix<double,3,3> W_gamma;

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

    W_tau = Matrix<double,7,7>::Identity()/pow(tau_lim,2);
    W_gamma = Matrix<double,3,3>::Identity()/pow(gamma_ulim,2);

    casadi::DM J_casadi = casadi::DM::zeros(J.rows(), J.cols());
    casadi::DM Gvec_casadi = casadi::DM::zeros(Gvec.rows(), Gvec.cols());
    casadi::DM R_eef_casadi = casadi::DM::zeros(R_eef.rows(), R_eef.cols());
    casadi::DM W_tau_casadi = casadi::DM::zeros(W_tau.rows(), W_tau.cols());
    casadi::DM W_gamma_casadi = casadi::DM::zeros(W_gamma.rows(), W_gamma.cols());

    memcpy(J_casadi.ptr(), J.data(), J.size() * sizeof(double));
    memcpy(Gvec_casadi.ptr(), Gvec.data(), Gvec.size() * sizeof(double));
    memcpy(R_eef_casadi.ptr(), R_eef.data(), R_eef.size() * sizeof(double));
    memcpy(W_tau_casadi.ptr(), W_tau.data(), W_tau.size() * sizeof(double));
    memcpy(W_gamma_casadi.ptr(), W_gamma.data(), W_gamma.size() * sizeof(double));

    vector<casadi::DM> inputs = {J_casadi, Gvec_casadi, R_eef_casadi, W_tau_casadi, W_gamma_casadi, tau_lim, gamma_llim, gamma_ulim};
    system_clock::time_point StartTime = system_clock::now();
    vector<casadi::DM> outputs = GravOpt_Wrench(inputs);
    system_clock::time_point EndTime = system_clock::now();
    microseconds micro = duration_cast<microseconds>(EndTime - StartTime);
    
    cout << "function time: " << micro.count()/1000.0 << "ms" << endl;
//     cout << "input: " << inputs << endl;
    cout << "tau_opt: " << outputs[0] << endl;
    cout << "gamma_space_opt: " << outputs[1] << endl;

    return 0;
}