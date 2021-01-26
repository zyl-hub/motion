#include <algorithm>
#include <iostream>
#include <math.h>

double vzero = 0.001;
inline double copysign(double a, double b)
{
    return b < 0 ? -a : a;
}

void compute_motion_1d(double x, double v0, double v1, double a_max, double d_max, double v_max, double a_factor, double vel_factor, double& traj_accel, double& traj_time, double& traj_time_acc, double& traj_time_dec, double& traj_time_flat)
{
    double frame_rate = 75;
    double delta_t = 1 / frame_rate;
    double total_x_v0_to_v1 = 0;
    double total_t_v0_to_v1 = 0;
    double total_x_v0_to_v_max = 0;
    double total_x_v_max_to_v1 = 0;
    double total_x = 0;
    double acc_time = 0;
    double flat_time = 0;
    double dec_time = 0;
    double total_x_v0_to_0 = 0;
    double v_m = 0;
    double total_x_0_to_v_m = 0;
    double total_x_v_m_to_0 = 0;
    double total_x_0_to_v1 = 0;
    double total_x_0_to_v_max = 0;
    double total_x_v_max_to_0 = 0;

    if (!finite(x) || !finite(v0) || !finite(v1)) {
        return;
    }

    if (std::abs(std::abs(v0) - v_max) < 5 * a_max / frame_rate) {
        if (v1 * v0 > 0) {
            total_x_v0_to_v1 = std::abs((v0 + v1) / 2 * (v0 - v1) / d_max);
        } else {
            total_x_v0_to_v1 = v0 * v0 / (2 * d_max) - v1 * v1 / (2 * a_max);
        }
        if (total_x_v0_to_v1 < abs(x)) {
            traj_accel = 0;
        } else {
            traj_accel = copysign(d_max, -v0);
        }
        return;
    }

    if (std::abs(v0) < a_max / frame_rate and std::abs(v1) < a_max / frame_rate) {
        if (std::abs(x) < 0.01) {
            traj_accel = -v0 / frame_rate / 2;
            return;
        } else {
            traj_accel = copysign(a_max, x);
            return;
        }
    }

    else if (std::abs(v0) < a_max / frame_rate) {
        traj_accel = copysign(a_max, x);
        return;
    }

    else if (std::abs(v1) < a_max / frame_rate) {
        if (v0 * x < 0) {
            traj_accel = copysign(d_max, -v0);
            return;
        } else if (v0 * x >= 0) {
            total_x_v0_to_0 = v0 * v0 / (2 * d_max);
            if (abs(x) < total_x_v0_to_0) {
                traj_accel = copysign(d_max, -v0);
                return;
            } else {
                traj_accel = copysign(a_max, v0);
                return;
            }
        }
    } else if (v0 * v1 > 0) {
        if (x * v0 > 0) {
            if (std::abs(v1) > std::abs(v0)) {
                traj_accel = copysign(a_max, v0);
                return;
            } else if (abs(v1) < abs(v0)) {
                total_t_v0_to_v1 = abs(v0 - v1) / d_max;
                total_x_v0_to_v1 = abs(v0 + v1) / 2 * total_t_v0_to_v1;
                if (std::abs(x) > total_x_v0_to_v1) {
                    traj_accel = copysign(a_max, v0);
                    return;
                } else {
                    traj_accel = copysign(d_max, -v0);
                    return;
                }
            }
        }

        else {
            traj_accel = copysign(d_max, -v0);
            return;
        }
    }

    else if (v0 * v1 < 0) {
        if (x * v0 >= 0) {
            total_x_0_to_v1 = (v1 * v1) / (2 * a_max);
            compute_motion_1d(copysign(abs(x) + total_x_0_to_v1, x), v0, 0, a_max, d_max,
                v_max, a_factor, vel_factor, traj_accel, traj_time, traj_time_acc, traj_time_dec, traj_time_flat);
            return;
        } else {
            traj_accel = copysign(d_max, -v0);
        }
    }
}
