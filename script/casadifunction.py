#!/usr/bin/env python3

import numpy as np
import casadi as cs
import time

F=cs.Function.load('../lib/GravOpt_Wrench.casadi')
# Function(GravOpt_Wrench:(J[6x7],Gvec[7],R_eef[3x3],W_tau[7x7],W_gamma[3x3],tau_lim,gamma_llim,gamma_ulim)->(tau_opt[7],gamma_space_opt[3]) MXFunction)

J = np.matrix([[0.5555, 0, -0.5904, 0, -0.2150, 0, 0.0000],
               [-0.1294, -0.8103, -0.2588, -0.2150, -0.0000, -0.2150, 0],
               [0.1294, 0.0247, -0.2588, -1.0500, 0.1830, 0, 0],
               [0, -1.0000, 0, 1.0000, 0, 1.0000, 0],
               [-0.7071, 0, 0.7071, 0, -1.0000, 0, 0],
               [-0.7071, 0, -0.7071, 0, -0.0000, 0, 1.0000]])
Gvec = np.matrix([0.0000, 369.7407, 61.0699, 128.3092, -11.2580, 3.9898, 0])
R_eef = np.matrix([[1.0000, 0, 0],
                   [0, -0.7071, 0.7071],
                   [0, -0.7071, -0.7071]])
tau_lim = 80
gamma_ulim = 500
gamma_llim = -500
W_tau = (1/tau_lim**2)*np.eye(7)
W_gamma = (1/gamma_ulim**2)*np.eye(3)

start_time = time.time()
[tau_opt, gamma_space_opt] = F(J, Gvec, R_eef, W_tau, W_gamma, tau_lim, gamma_llim, gamma_ulim)
end_time = time.time()
elapsed_time = (end_time - start_time) * 1000


print(f"function time: {elapsed_time:.4f} ms")
print("tau_opt",tau_opt)
print("gamma_space_opt",gamma_space_opt)