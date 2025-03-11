import math
import numpy as np


def get_U1(X, Z_des, Lamda, m, g):  
    z = X[5]
    z_dot=X[6]
    Phi=X[7]
    Theta=X[9]

    z_des = Z_des[1]
    z_dot_des = Z_des(2)
    z_ddot_des = Z_des(3)

    s=(-z_dot_des+z_dot) + Lamda*(-z_des+z)
    #s_dot=-k1*sign(s)-k2*s

    f = -g
    U1 = (m/(math.cos(Phi)*math.cos(Theta)))*(-f+z_ddot_des+Lamda*(z_dot_des-z_dot))
    return U1

def get_U2(X, PHI_des, w_d, I, Lamda, J_r , b1):
    Ix, Iy, Iz = I[0:2]

    a1=(Iy-Iz)/Ix
    Phi=X[7]
    Phi_dot = X[8]

    Theta_dot=X[10]
    Psi_dot=X[12]

    Phi_des = PHI_des[1]
    Phi_dot_des= PHI_des[2] 
    Phi_ddot_des= PHI_des[3]

    s=(-Phi_dot_des+Phi_dot)+Lamda*(-Phi_des+Phi)
    # s_dot=-k1*sign(s)-k2*s

    f=Psi_dot*Theta_dot*a1-(J_r/Ix)*Theta_dot*w_d
    # U2 = (1/b1)*(-s_dot-a1*Theta_dot*Psi_dot+Phi_ddot_des+lambda1*(Phi_dot_des-Phi_dot))
    U2 = (1/b1)*(-f+ Phi_ddot_des+Lamda*(Phi_dot_des-Phi_dot))
    #clip u2

    return U2

def get_U3(X, THETA_des, w_d, I, Lamda, J_r, b1):
    Ix, Iy, Iz = I[0:2]

    a2 = (Iz - Ix) / Iy

    Phi, Phi_dot, Theta, Theta_dot, Psi, Psi_dot = X[6:12]

    Theta_des, Theta_dot_des, Theta_ddot_des = THETA_des

    s = (Theta_dot_des - Theta_dot) + Lamda * (Theta_des - Theta)
    # s_dot=-k1*sign(s)-k2*s;
    f = Psi_dot * Phi_dot * a2 + (J_r / Iy) * Phi_dot * w_d
    
    # U2 = (1/b1)*(-s_dot-a1*Theta_dot*Psi_dot+Phi_ddot_des+Lamda*(Phi_dot_des-Phi_dot))
    U3 = (1 / b1) * (-f + Theta_ddot_des + Lamda * (Theta_dot_des - Theta_dot))
    return(U3)


def get_U4(X, PSI_des, I, length, Lamda):
    Ix, Iy, Iz = I[0:2]
    b1 = length / Iy
    a3 = (Ix - Iy) / Iz
    
    Phi, Phi_dot, Theta, Theta_dot, Psi, Psi_dot = X[6:12]

    Psi_des, Psi_dot_des, Psi_ddot_des = PSI_des

    s = (Psi_dot_des - Psi_dot) + Lamda * (Psi_des - Psi)
    f = Theta_dot * Phi_dot * a3

    U4 = (1 / b1) * (-f + Psi_ddot_des + Lamda * (Psi_dot_des - Psi_dot))

    return U4

def get_U(X, Z_des, PHI_des, THETA_des, PSI_des, w_d, m, Lamda ):
    U = []
    U.append(get_U1(X, Z_des, m, Lamda))
    U.append(get_U2(X, PHI_des, w_d))
    U.append(get_U3(X, THETA_des, w_d))
    U.append(get_U4(X, PSI_des))

    return U

def get_W(U, W_old, b, d):
    u1, u2, u3, u4 = U
    
    if (d*u1 - 2*d*u3 - b*u4) / (4*d*b) > 0:
        w1 = np.sqrt((d*u1 - 2*d*u3 - b*u4) / (4*d*b))
    else:
        w1 = W_old[0]

    if (d*u1 - 2*d*u2 + b*u4) / (4*d*b) > 0:
        w2 = np.sqrt((d*u1 - 2*d*u2 + b*u4) / (4*d*b))
    else:
        w2 = W_old[1]

    if (d*u1 + 2*d*u3 - b*u4) / (4*d*b) > 0:
        w3 = np.sqrt((d*u1 + 2*d*u3 - b*u4) / (4*d*b))
    else:
        w3 = W_old[2]

    if (d*u1 + 2*d*u2 + b*u4) / (4*d*b) > 0:
        w4 = np.sqrt((d*u1 + 2*d*u2 + b*u4) / (4*d*b))
    else:
        w4 = W_old[3]

    W = np.array([w1, w2, w3, w4])
    return W



def get_wd(W):
    W1=W[1]
    W2=W[2]
    W3=W[3]
    W4=W[4]
    w_d =W4+W2-W3-W1
    return w_d
