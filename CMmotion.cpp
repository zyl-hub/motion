#include <algorithm>
#include <iostream>
#include <math.h>

double vzero = 0.001;
inline double copysign(double a, double b)
{
    return b < 0 ? -a : a;
}

void compute_motion_1d(double x, double v0, double v1, double a_max, double d_max, double v_max, double a_factor, double vel_factor, double& traj_accel, double& traj_time, double& traj_time_acc, double& traj_time_dec, double& traj_time_flat, double frame_rate)
{
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

    if (std::abs(x) < 1e-5 && std::abs(v0 - v1) < 1e-5) {
        traj_accel = 0;
        traj_time_dec = 0;
        traj_time_flat = 0;
        traj_time_acc = 0;
        return;
    }

    if (!finite(x) || !finite(v0) || !finite(v1)) {
        return;
    }

    if (std::abs(std::abs(v0) - v_max) < 0.05 * v_max) {
        if (v1 * v0 > 0) {
            total_x_v0_to_v1 = std::abs((v0 + v1) / 2.0 * (v0 - v1) / d_max);
        } else {
            total_x_v0_to_v1 = v0 * v0 / (2.0 * d_max) - v1 * v1 / (2.0 * a_max);
        }
        if (total_x_v0_to_v1 < std::abs(x)) {
            traj_accel = 0;
        } else {
            traj_accel = copysign(d_max, -v0);
        }
        return;
    }

    if (v0 * v1 > 0) {
        if (x * v0 > 0) {
            if (std::abs(v1) > std::abs(v0)) {
                total_t_v0_to_v1 = std::abs(v0 - v1) / a_max;
                total_x_v0_to_v1 = std::abs(v0 + v1) / 2.0 * total_t_v0_to_v1;
                // state1
                if (std::abs(x) <= total_x_v0_to_v1) {
                    std::cout << "state1:";
                    acc_time = (-std::abs(v0) + std::sqrt(v0 * v0 + 2.0 * a_max * abs(x))) / a_max;
                    traj_time_acc += acc_time;
                    traj_time_flat += 0;
                    traj_time_dec += 0;
                    traj_accel = copysign(a_max, v0);
                    std::cout << "state1:"
                              << "x:" << x << "\t"
                              << "v0:" << v0 << "\t"
                              << "v1:" << v1 << "\t"
                              << "a:" << traj_accel;
                    return;
                } else if (std::abs(x) > total_x_v0_to_v1) {
                    total_x_v0_to_v_max = std::abs(v_max * v_max - v0 * v0) / (2.0 * a_max);
                    total_x_v_max_to_v1 = std::abs(v_max * v_max - v1 * v1) / (2.0 * d_max);
                    total_x = total_x_v0_to_v_max + total_x_v_max_to_v1;
                    // state2
                    if ((std::abs(x) - total_x) >= 0) {
                        std::cout << "state2:";
                        acc_time = (v_max - abs(v0)) / a_max;
                        flat_time = (abs(x) - total_x) / v_max;
                        dec_time = (v_max - abs(v1)) / d_max;
                        traj_time_acc += acc_time;
                        traj_time_flat += flat_time;
                        traj_time_dec += dec_time;
                        traj_accel = copysign(a_max, v0);
                        std::cout << "state2:"
                                  << "x:" << x << "\t"
                                  << "v0:" << v0 << "\t"
                                  << "v1:" << v1 << "\t"
                                  << "a:" << traj_accel;
                        return;
                    }
                    // state3
                    else if ((std::abs(x) - total_x) < 0) {
                        std::cout << "state3:";
                        v_m = std::sqrt((2.0 * a_max * d_max * std::abs(x) + d_max * v0 * v0 + a_max * v1 * v1) / (a_max + d_max));
                        acc_time = (v_m - std::abs(v0)) / a_max;
                        flat_time = 0;
                        dec_time = (v_m - std::abs(v1)) / d_max;
                        traj_time_acc += acc_time;
                        traj_time_flat += flat_time;
                        traj_time_dec += dec_time;
                        traj_accel = copysign(a_max, v0);
                        if (v_m - v0 < a_max / frame_rate) {
                            traj_accel = 0;
                        }
                        std::cout << "state3:"
                                  << "x:" << x << "\t"
                                  << "v0:" << v0 << "\t"
                                  << "v1:" << v1 << "\t"
                                  << "a:" << traj_accel;
                        return;
                    }
                }
            }

            else if (std::abs(v1) < std::abs(v0)) {
                total_t_v0_to_v1 = std::abs(v0 - v1) / d_max;
                total_x_v0_to_v1 = std::abs(v0 + v1) / 2.0 * total_t_v0_to_v1;
                // state4
                if (std::abs(x) - total_x_v0_to_v1 < 0.01) {
                    std::cout << "state4:";
                    dec_time = (std::abs(v0) - std::sqrt(std::abs(v0 * v0 - 2.0 * d_max * std::abs(x)))) / d_max;
                    traj_time_acc += 0;
                    traj_time_flat += 0;
                    traj_time_dec += dec_time;
                    traj_accel = copysign(d_max, -v0);
                    std::cout << "state4:"
                              << "x:" << x << "\t"
                              << "v0:" << v0 << "\t"
                              << "v1:" << v1 << "\t"
                              << "a:" << traj_accel;
                    return;
                } else if (std::abs(std::abs(x) - total_x_v0_to_v1) >= 0.01) {
                    total_x_v0_to_v_max = std::abs(v_max * v_max - v0 * v0) / (2.0 * a_max);
                    total_x_v_max_to_v1 = std::abs(v_max * v_max - v1 * v1) / (2.0 * d_max);
                    total_x = total_x_v0_to_v_max + total_x_v_max_to_v1;
                    // state5
                    if (std::abs(x) > total_x) {
                        std::cout << "state5:";
                        acc_time = (std::abs(v_max) - std::abs(v0)) / a_max;
                        flat_time = (std::abs(x) - total_x) / v_max;
                        dec_time = (v_max - std::abs(v1)) / d_max;
                        traj_time_acc += acc_time;
                        traj_time_flat += flat_time;
                        traj_time_dec += dec_time;
                        traj_accel = copysign(a_max, v0);
                        std::cout << "state5:"
                                  << "x:" << x << "\t"
                                  << "v0:" << v0 << "\t"
                                  << "v1:" << v1 << "\t"
                                  << "a:" << traj_accel;
                        return;
                    }
                    // state6
                    else {
                        std::cout << "state6:";
                        v_m = std::sqrt((2.0 * a_max * d_max * std::abs(x) + d_max * v0 * v0 + a_max * v1 * v1) / (a_max + d_max));
                        acc_time = (v_m - std::abs(v0)) / a_max;
                        flat_time = 0;
                        dec_time = (v_m - std::abs(v1)) / d_max;
                        traj_time_acc += acc_time;
                        traj_time_flat += flat_time;
                        traj_time_dec += dec_time;
                        traj_accel = copysign(a_max, v0);
                        if (v_m - std::abs(v0) < a_max / frame_rate) {
                            traj_accel = 0;
                        }
                        std::cout << "state6:"
                                  << "x:" << x << "\t"
                                  << "v0:" << v0 << "\t"
                                  << "v1:" << v1 << "\t"
                                  << "a:" << traj_accel << "\t"
                                  << "v_m:" << v_m;
                        return;
                    }
                }
            }
        }

        else if (x * v0 < 0) {
            total_x_v0_to_0 = v0 * v0 / (2.0 * d_max);
            v_m = copysign(std::sqrt((v0 * v0 / (2.0 * d_max) + v1 * v1 / (2.0 * a_max) + std::abs(x)) / (1 / (2.0 * a_max) + 1 / (2.0 * d_max))), -v0);
            total_x_v0_to_0 = copysign((v0 * v0) / (2.0 * d_max), v0);
            total_x_0_to_v_m = copysign((v_m * v_m) / (2.0 * a_max), -v0);
            total_x_v_m_to_0 = copysign((v_m * v_m) / (2.0 * d_max), -v0);
            total_x_0_to_v1 = copysign((v1 * v1) / (2.0 * a_max), v1);
            total_x_0_to_v_max = copysign((v_max * v_max) / (2.0 * a_max), -v0);
            total_x_v_max_to_0 = copysign((v_max * v_max) / (2.0 * d_max), -v0);
            // state7
            if (std::abs(v_m) < v_max) {
                std::cout << "state7:";
                compute_motion_1d(total_x_0_to_v1, copysign(vzero, v1), v1, a_max, d_max, v_max, a_factor, vel_factor, traj_accel, traj_time, traj_time_acc, traj_time_dec, traj_time_flat, frame_rate);
                compute_motion_1d(total_x_v_m_to_0, v_m, copysign(vzero, -v0), a_max, d_max, v_max, a_factor, vel_factor, traj_accel, traj_time, traj_time_acc, traj_time_dec, traj_time_flat, frame_rate);
                compute_motion_1d(total_x_0_to_v_m, copysign(vzero, -v0), v_m, a_max, d_max, v_max, a_factor, vel_factor, traj_accel, traj_time, traj_time_acc, traj_time_dec, traj_time_flat, frame_rate);
                compute_motion_1d(total_x_v0_to_0, v0, copysign(vzero, v0), a_max, d_max, v_max, a_factor, vel_factor, traj_accel, traj_time, traj_time_acc, traj_time_dec, traj_time_flat, frame_rate);
                std::cout << "state7:"
                          << "x:" << x << "\t"
                          << "v0:" << v0 << "\t"
                          << "v1:" << v1 << "\t"
                          << "a:" << traj_accel;
                return;
            }
            // state8
            else {
                std::cout << "state8:";
                compute_motion_1d(total_x_0_to_v1, copysign(vzero, v1), v1, a_max, d_max, v_max, a_factor, vel_factor, traj_accel, traj_time, traj_time_acc, traj_time_dec, traj_time_flat, frame_rate);
                compute_motion_1d(total_x_v_max_to_0, v_max, copysign(vzero, -v0), a_max, d_max, v_max, a_factor, vel_factor, traj_accel, traj_time, traj_time_acc, traj_time_dec, traj_time_flat, frame_rate);
                compute_motion_1d(x - total_x_v0_to_0 - total_x_v_max_to_0 - total_x_0_to_v1, copysign(vzero, -v0), v_m, a_max, d_max, v_max, a_factor, vel_factor, traj_accel, traj_time, traj_time_acc, traj_time_dec, traj_time_flat, frame_rate);
                compute_motion_1d(total_x_v0_to_0, v0, copysign(vzero, v0), a_max, d_max, v_max, a_factor, vel_factor, traj_accel, traj_time, traj_time_acc, traj_time_dec, traj_time_flat, frame_rate);
                std::cout << "state8:"
                          << "x:" << x << "\t"
                          << "v0:" << v0 << "\t"
                          << "v1:" << v1 << "\t"
                          << "a:" << traj_accel;

                return;
            }
        }
    }

    else if (v0 * v1 < 0) {
        // state9
        if (x * v0 >= 0) {
            std::cout << "state9:";
            v1 = copysign(std::min(std::abs(v1), 5.0), v1);
            total_x_0_to_v1 = (v1 * v1) / (2.0 * a_max);
            compute_motion_1d(copysign(total_x_0_to_v1, -x),
                copysign(vzero, -x), v1, a_max, d_max, v_max, a_factor, vel_factor, traj_accel, traj_time, traj_time_acc, traj_time_dec, traj_time_flat, frame_rate);

            compute_motion_1d(copysign(abs(x) + total_x_0_to_v1, x), v0, copysign(vzero, x), a_max, d_max, v_max, a_factor, vel_factor, traj_accel, traj_time, traj_time_acc, traj_time_dec, traj_time_flat, frame_rate);

            std::cout << "state9:"
                      << "x:" << x << "\t"
                      << "v0:" << v0 << "\t"
                      << "v1:" << v1 << "\t"
                      << "a:" << traj_accel;
        }
        // state10
        else if (x * v0 < 0) {
            std::cout << "state10:";
            total_x_v0_to_0 = v0 * v0 / (2.0 * d_max);
            if (v0 * v1 < 0 and abs(v0) > 0.015) {
                compute_motion_1d(copysign(total_x_v0_to_0 + abs(x), x),
                    copysign(vzero, x),
                    v1, a_max, d_max, v_max, a_factor, vel_factor, traj_accel, traj_time, traj_time_acc, traj_time_dec, traj_time_flat, frame_rate);

                compute_motion_1d(copysign(total_x_v0_to_0, -x), v0, copysign(vzero, -x), a_max, d_max, v_max, a_factor, vel_factor, traj_accel, traj_time, traj_time_acc, traj_time_dec, traj_time_flat, frame_rate);
            } else {
                compute_motion_1d(copysign(total_x_v0_to_0, -x), v0, copysign(vzero, -x), a_max, d_max, v_max, a_factor, vel_factor, traj_accel, traj_time, traj_time_acc, traj_time_dec, traj_time_flat, frame_rate);

                compute_motion_1d(copysign(total_x_v0_to_0 + abs(x), x),
                    copysign(vzero, x),
                    v1, a_max, d_max, v_max, a_factor, vel_factor, traj_accel, traj_time, traj_time_acc, traj_time_dec, traj_time_flat, frame_rate);

                std::cout << "**********";
            }
            std::cout << "state10:"
                      << "x:" << x << "\t"
                      << "v0:" << v0 << "\t"
                      << "v1:" << v1 << "\t"
                      << "a:" << traj_accel;
            return;
        }
    }
}
