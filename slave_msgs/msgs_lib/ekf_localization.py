#!/usr/bin/env python3
import numpy as np
from math import sin, cos, pi
# from nav_lib.utils.wrapto2pi import wrapto2pi


# mut_1: t-1 state: x, y, theta  3*1 matrix    np.array([[x], [y], [theta]])
# sigmat_1: t-1 variance: 3*3 matrix
# ut: control parameter: linear, angular  2*1 matrix 
# zt: gps obseravation: position x and y  2*1 matrix     np.array([[x], [y]])
# alpha: control noise
# sigmaQ: observation noise (variance): 2*2
# sampletime: sample time


def wraptopi(radian):
    # -pi to pi

    if radian > pi:
        radian2 = radian - 2 * pi
    elif radian < -pi:
        radian2 = radian + 2 * pi
    else:
        radian2 = radian

    # diff = radian1 - radian2
    # print(diff)
    return radian2


def ekf_odom_uwb(mut_1, sigmat_1, ut, zt, alpha, sigmaQ, sampletime):

    C = np.array([[1, 0, 0], [0, 1, 0]])

    theta = float(wraptopi(mut_1[2, 0]))
    vt = float(ut[0, 0])
    wt = float(ut[1, 0])

    # prediction step
    if wt != 0:

        Gt = np.array([[1, 0, -(vt/wt) * cos(theta) + (vt/wt) * cos(theta + wt * sampletime)], 
                        [0, 1, -(vt/wt) * sin(theta) + (vt/wt) * sin(theta + wt * sampletime)], 
                        [0, 0, 1]])
        
        Vt = np.array([ [(-sin(theta) + sin(theta + wt * sampletime))/wt, (vt * (sin(theta) - sin(theta + wt * sampletime)))/(wt ** 2) +                     (vt * cos(theta + wt * sampletime) * sampletime)/wt], 
                        [((cos(theta) - cos(theta + wt * sampletime))/wt), (-vt * (cos(theta) - cos(theta + wt * sampletime)))/(wt ** 2) + (vt * sin(theta + wt * sampletime) * sampletime)/wt ], 
                        [0, sampletime] ])

        Mt = np.array([ [alpha[0] * (vt ** 2) + alpha[1] * (wt ** 2) , 0], 
                        [0, alpha[2] * (vt ** 2) + alpha[3] * (wt ** 2)] ])

        mu_t = mut_1 + np.array([ [-(vt/wt) * sin(theta) + (vt/wt) * sin(theta + wt * sampletime)],
                                  [(vt/wt) * cos(theta) - (vt/wt) * cos(theta + wt * sampletime)],
                                  [wt * sampletime] ])

        temp1 = Gt @ sigmat_1 @ np.transpose(Gt)
        temp2 = Vt @ Mt @ np.transpose(Vt)

        sigma_t = temp1 + temp2

    else:
        Gt = np.array([ [1, 0, -sampletime * vt * sin(theta)],
                        [0, 1, sampletime * vt * cos(theta)],
                        [0, 0, 1]  ])
        
        Vt = np.array([ [sampletime * cos(theta), 0], 
                        [sampletime * sin(theta), 0],
                        [0, 0] ])
        
        Mt = np.array([ [alpha[0] * (vt ** 2) + alpha[1] * (wt ** 2) , 0], 
                        [0, alpha[2] * (vt ** 2) + alpha[3] * (wt ** 2)] ])

        mu_t = mut_1 + np.array([  [vt * sampletime * cos(theta)],
                                    [vt * sampletime * sin(theta)],
                                    [0] ])

        sigma_t = Gt @ sigmat_1 @ np.transpose(Gt) + Vt @ Mt @ np.transpose(Vt)

    mu_t[2, 0] = wraptopi(mu_t[2, 0])

    # correction step
    if zt.size != 0:
     
        Kt = sigma_t @ np.transpose(C) @ np.linalg.inv(C @ sigma_t @ np.transpose(C) + sigmaQ)
        mu_t = mu_t + Kt @ (zt - C @ mu_t)
        mu_t[2, 0] = wraptopi(mu_t[2, 0])
        sigma_t = (np.identity(3) - Kt @ C) @ sigma_t

    return mu_t, sigma_t