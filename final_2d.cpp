#include <algorithm>
#include <fstream>
#include <iostream>
#include <math.h>
#include <typeinfo>
#include <vector>

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
        std::cout << "0"
                  << "\t";
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

    if (std::abs(v0) < a_max / frame_rate) {
        v0 = copysign(a_max / frame_rate, v0);
    }

    if (std::abs(v1) < a_max / frame_rate) {
        if (std::abs(x) < 0.0015) {
            traj_accel = 0;
            return;
        } else {
            v1 = copysign(vzero, v1);
        }
    }

    if (v0 * v1 > 0) {
        if (x * v0 > 0) {
            if (std::abs(v1) > std::abs(v0)) {
                total_t_v0_to_v1 = std::abs(v0 - v1) / a_max;
                total_x_v0_to_v1 = std::abs(v0 + v1) / 2.0 * total_t_v0_to_v1;
                // state1
                if (std::abs(x) <= total_x_v0_to_v1) {
                    std::cout << "state1:"
                              << "\n";
                    acc_time = (-std::abs(v0) + std::sqrt(v0 * v0 + 2.0 * a_max * abs(x))) / a_max;
                    traj_time_acc += acc_time;
                    traj_time_flat += 0;
                    traj_time_dec += 0;
                    traj_accel = copysign(a_max, v0);
                    std::cout << "state1:"
                              << "x:" << x << "\t"
                              << "v0:" << v0 << "\t"
                              << "v1:" << v1 << "\t"
                              << "a:" << traj_accel << "\n";
                    return;
                } else if (std::abs(x) > total_x_v0_to_v1) {
                    total_x_v0_to_v_max = std::abs(v_max * v_max - v0 * v0) / (2.0 * a_max);
                    total_x_v_max_to_v1 = std::abs(v_max * v_max - v1 * v1) / (2.0 * d_max);
                    total_x = total_x_v0_to_v_max + total_x_v_max_to_v1;
                    // state2
                    if ((std::abs(x) - total_x) >= 0) {
                        std::cout << "state2:"
                                  << "\n";
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
                                  << "a:" << traj_accel << "\n";
                        return;
                    }
                    // state3
                    else if ((std::abs(x) - total_x) < 0) {
                        std::cout << "state3:"
                                  << "\n";
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
                                  << "a:" << traj_accel << "\n";
                        return;
                    }
                }
            }

            else if (std::abs(v1) < std::abs(v0)) {
                total_t_v0_to_v1 = std::abs(v0 - v1) / d_max;
                total_x_v0_to_v1 = std::abs(v0 + v1) / 2.0 * total_t_v0_to_v1;
                // state4
                if (std::abs(x) - total_x_v0_to_v1 < 0.01) {
                    std::cout << "state4:"
                              << "\n";
                    dec_time = (std::abs(v0) - std::sqrt(std::abs(v0 * v0 - 2.0 * d_max * std::abs(x)))) / d_max;
                    traj_time_acc += 0;
                    traj_time_flat += 0;
                    traj_time_dec += dec_time;
                    traj_accel = copysign(d_max, -v0);
                    std::cout << "state4:"
                              << "x:" << x << "\t"
                              << "v0:" << v0 << "\t"
                              << "v1:" << v1 << "\t"
                              << "a:" << traj_accel << "\n";
                    return;
                } else if (std::abs(std::abs(x) - total_x_v0_to_v1) >= 0.01) {
                    total_x_v0_to_v_max = std::abs(v_max * v_max - v0 * v0) / (2.0 * a_max);
                    total_x_v_max_to_v1 = std::abs(v_max * v_max - v1 * v1) / (2.0 * d_max);
                    total_x = total_x_v0_to_v_max + total_x_v_max_to_v1;
                    // state5
                    if (std::abs(x) > total_x) {
                        std::cout << "state5:"
                                  << "\n";
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
                                  << "a:" << traj_accel << "\n";
                        return;
                    }
                    // state6
                    else {
                        std::cout << "state6:"
                                  << "\n";
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
                                  << "v_m:" << v_m << "\n";
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
                std::cout << "state7:"
                          << "\n";
                compute_motion_1d(total_x_0_to_v1, copysign(vzero, v1), v1, a_max, d_max, v_max, a_factor, vel_factor, traj_accel, traj_time, traj_time_acc, traj_time_dec, traj_time_flat, frame_rate);
                compute_motion_1d(total_x_v_m_to_0, v_m, copysign(vzero, -v0), a_max, d_max, v_max, a_factor, vel_factor, traj_accel, traj_time, traj_time_acc, traj_time_dec, traj_time_flat, frame_rate);
                compute_motion_1d(total_x_0_to_v_m, copysign(vzero, -v0), v_m, a_max, d_max, v_max, a_factor, vel_factor, traj_accel, traj_time, traj_time_acc, traj_time_dec, traj_time_flat, frame_rate);
                compute_motion_1d(total_x_v0_to_0, v0, copysign(vzero, v0), a_max, d_max, v_max, a_factor, vel_factor, traj_accel, traj_time, traj_time_acc, traj_time_dec, traj_time_flat, frame_rate);
                if (traj_accel == 0) {
                    traj_accel = copysign(a_max, -v0);
                }
                std::cout << "state7:"
                          << "x:" << x << "\t"
                          << "v0:" << v0 << "\t"
                          << "v1:" << v1 << "\t"
                          << "a:" << traj_accel << "\n";
                return;
            }
            // state8
            else {
                std::cout << "state8:"
                          << "\n";
                compute_motion_1d(total_x_0_to_v1, copysign(vzero, v1), v1, a_max, d_max, v_max, a_factor, vel_factor, traj_accel, traj_time, traj_time_acc, traj_time_dec, traj_time_flat, frame_rate);
                compute_motion_1d(total_x_v_max_to_0, v_max, copysign(vzero, -v0), a_max, d_max, v_max, a_factor, vel_factor, traj_accel, traj_time, traj_time_acc, traj_time_dec, traj_time_flat, frame_rate);
                compute_motion_1d(x - total_x_v0_to_0 - total_x_v_max_to_0 - total_x_0_to_v1, copysign(vzero, -v0), v_m, a_max, d_max, v_max, a_factor, vel_factor, traj_accel, traj_time, traj_time_acc, traj_time_dec, traj_time_flat, frame_rate);
                compute_motion_1d(total_x_v0_to_0, v0, copysign(vzero, v0), a_max, d_max, v_max, a_factor, vel_factor, traj_accel, traj_time, traj_time_acc, traj_time_dec, traj_time_flat, frame_rate);

                std::cout << "state8:"
                          << "x:" << x << "\t"
                          << "v0:" << v0 << "\t"
                          << "v1:" << v1 << "\t"
                          << "a:" << traj_accel << "\n";
                if (traj_accel == 0) {
                    traj_accel = copysign(a_max, -v0);
                }
                return;
            }
        }
    }

    else if (v0 * v1 < 0) {
        // state9
        if (x * v0 >= 0) {
            std::cout << "state9:"
                      << "\n";
            v1 = copysign(std::min(std::abs(v1), 5.0), v1);
            total_x_0_to_v1 = (v1 * v1) / (2.0 * a_max);
            compute_motion_1d(copysign(total_x_0_to_v1, -x),
                copysign(vzero, -x), v1, a_max, d_max, v_max, a_factor, vel_factor, traj_accel, traj_time, traj_time_acc, traj_time_dec, traj_time_flat, frame_rate);

            compute_motion_1d(copysign(abs(x) + total_x_0_to_v1, x), v0, copysign(vzero, x), a_max, d_max, v_max, a_factor, vel_factor, traj_accel, traj_time, traj_time_acc, traj_time_dec, traj_time_flat, frame_rate);

            std::cout << "state9:"
                      << "x:" << x << "\t"
                      << "v0:" << v0 << "\t"
                      << "v1:" << v1 << "\t"
                      << "a:" << traj_accel << "\n";
        }
        // state10
        else if (x * v0 < 0) {
            std::cout << "state10:"
                      << "\n";
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

                std::cout << "**********"
                          << "\n";
            }
            std::cout << "state10:"
                      << "x:" << x << "\t"
                      << "v0:" << v0 << "\t"
                      << "v1:" << v1 << "\t"
                      << "a:" << traj_accel << "\n";
            return;
        }
    }
}
double computex(double x0, double v0, double a, double frame_rate)
{
    double x = 0;
    double v = 0;
    v = v0 + a / frame_rate;
    x = x0 + (v + v0) / frame_rate / 2;
    return x;
}
double computev(double x0, double v0, double a, double frame_rate)
{
    double x = 0;
    double v = 0;
    v = v0 + a / frame_rate;
    x = x0 + (v + v0) / frame_rate / 2;
    return v;
}

void compute_2d(int len, std::vector<std::vector<double>> x_data_list, std::vector<std::vector<double>> y_data_list, int epoch_num)
{
    std::vector<std::vector<double>> x(len, std::vector<double>(epoch_num, 0));
    std::vector<std::vector<double>> y(len, std::vector<double>(epoch_num, 0));
    std::vector<std::vector<double>> v_x(len, std::vector<double>(epoch_num, 0));
    std::vector<std::vector<double>> v_y(len, std::vector<double>(epoch_num, 0));
    std::vector<std::vector<double>> a_x(len, std::vector<double>(epoch_num, 0));
    std::vector<std::vector<double>> a_y(len, std::vector<double>(epoch_num, 0));
    double x_traj_accel = 0;
    double x_traj_time = 0;
    double x_traj_time_acc = 0;
    double x_traj_time_dec = 0;
    double x_traj_time_flat = 0;
    double y_traj_accel = 0;
    double y_traj_time = 0;
    double y_traj_time_acc = 0;
    double y_traj_time_dec = 0;
    double y_traj_time_flat = 0;
    double frame_rate = 75;
    int epoch = 0;
    double none = 0;
    int epoch_break = 0;
    for (int i = 0; i < len; i++) {
        for (int epoch = 0; epoch < epoch_num; epoch++) {
            if (epoch == 0) {
                std::cout << epoch << ":"
                          << "\n";
                compute_motion_1d(x_data_list[i][1] - x_data_list[i][0],
                    x_data_list[i][2],
                    x_data_list[i][3],
                    x_data_list[i][4],
                    x_data_list[i][5],
                    x_data_list[i][6],
                    none,
                    none,
                    x_traj_accel,
                    x_traj_time,
                    x_traj_time_acc,
                    x_traj_time_dec,
                    x_traj_time_flat,
                    frame_rate);
                x[i][epoch] = x_data_list[i][0];
                v_x[i][epoch] = x_data_list[i][2];
                a_x[i][epoch] = x_traj_accel;
                double xinfo = 0;
                double v_xinfo = 0;
                xinfo = computex(
                    x_data_list[i][0], x_data_list[i][2], 0, x_data_list[i][7]);
                v_xinfo = computev(
                    x_data_list[i][0], x_data_list[i][2], 0, x_data_list[i][7]);
                x_data_list[i][0] = xinfo;
                x_data_list[i][2] = v_xinfo;
                if (0) {
                } else {
                    std::cout << epoch << ":"
                              << "\n";
                    compute_motion_1d(y_data_list[i][1] - y_data_list[i][0],
                        y_data_list[i][2],
                        y_data_list[i][3],
                        y_data_list[i][4],
                        y_data_list[i][5],
                        y_data_list[i][6],
                        none,
                        none,
                        y_traj_accel,
                        y_traj_time,
                        y_traj_time_acc,
                        y_traj_time_dec,
                        y_traj_time_flat,
                        frame_rate);
                    y[i][epoch] = y_data_list[i][0];
                    v_y[i][epoch] = y_data_list[i][2];
                    a_y[i][epoch] = y_traj_accel;
                    double yinfo = 0;
                    double v_yinfo = 0;
                    yinfo = computex(
                        y_data_list[i][0], y_data_list[i][2], 0, y_data_list[i][7]);
                    v_yinfo = computev(
                        y_data_list[i][0], y_data_list[i][2], 0, y_data_list[i][7]);
                    y_data_list[i][0] = yinfo;
                    y_data_list[i][2] = v_yinfo;
                }
            } else {
                a_x[i][epoch] = x_traj_accel;
                x[i][epoch] = x_data_list[i][0];
                v_x[i][epoch] = x_data_list[i][2];
                double xinfo = 0;
                double v_xinfo = 0;
                xinfo = computex(
                    x_data_list[i][0], x_data_list[i][2], x_traj_accel, x_data_list[i][7]);
                v_xinfo = computev(
                    x_data_list[i][0], x_data_list[i][2], x_traj_accel, x_data_list[i][7]);
                std::cout << epoch << ":"
                          << "\n";
                compute_motion_1d(x_data_list[i][1] - x_data_list[i][0],
                    x_data_list[i][2],
                    x_data_list[i][3],
                    x_data_list[i][4],
                    x_data_list[i][5],
                    x_data_list[i][6],
                    none,
                    none,
                    x_traj_accel,
                    x_traj_time,
                    x_traj_time_acc,
                    x_traj_time_dec,
                    x_traj_time_flat,
                    frame_rate);
                x_data_list[i][0] = xinfo;
                x_data_list[i][2] = v_xinfo;
                if (0) {
                } else {
                    a_y[i][epoch] = y_traj_accel;
                    y[i][epoch] = y_data_list[i][0];
                    v_y[i][epoch] = y_data_list[i][2];
                    double yinfo = 0;
                    double v_yinfo = 0;
                    yinfo = computex(
                        y_data_list[i][0], y_data_list[i][2], y_traj_accel, y_data_list[i][7]);
                    v_yinfo = computev(
                        y_data_list[i][0], y_data_list[i][2], y_traj_accel, y_data_list[i][7]);
                    std::cout << epoch << ":"
                              << "\n";
                    compute_motion_1d(y_data_list[i][1] - y_data_list[i][0],
                        y_data_list[i][2],
                        y_data_list[i][3],
                        y_data_list[i][4],
                        y_data_list[i][5],
                        y_data_list[i][6],
                        none,
                        none,
                        y_traj_accel,
                        y_traj_time,
                        y_traj_time_acc,
                        y_traj_time_dec,
                        y_traj_time_flat,
                        frame_rate);
                    y_data_list[i][0] = yinfo;
                    y_data_list[i][2] = v_yinfo;
                }
            }
        }
    }
    std::ofstream xfile;
    std::ofstream yfile;
    std::ofstream v_xfile;
    std::ofstream v_yfile;
    std::ofstream a_xfile;
    std::ofstream a_yfile;
    xfile.open("data/x.txt");
    yfile.open("data/y.txt");
    v_xfile.open("data/v_x.txt");
    v_yfile.open("data/v_y.txt");
    a_xfile.open("data/a_x.txt");
    a_yfile.open("data/a_y.txt");
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < epoch_num; j++) {
            xfile << x[i][j] << " ";
        }
        xfile << "\n";
    }
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < epoch_num; j++) {
            yfile << y[i][j] << " ";
        }
        yfile << "\n";
    }
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < epoch_num; j++) {
            v_xfile << v_x[i][j] << " ";
        }
        v_xfile << "\n";
    }
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < epoch_num; j++) {
            v_yfile << v_y[i][j] << " ";
        }
        v_yfile << "\n";
    }
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < epoch_num; j++) {
            a_xfile << a_x[i][j] << " ";
        }
        a_xfile << "\n";
    }
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < epoch_num; j++) {
            a_yfile << a_y[i][j] << " ";
        }
        a_yfile << "\n";
    }
    xfile.close();
    yfile.close();
    v_xfile.close();
    v_yfile.close();
    a_xfile.close();
    a_yfile.close();
}

int main()
{
    int epoch_num = 20;
    int len = 30;
    std::vector<std::vector<double>> x_data_list(len, std::vector<double>(8));
    std::vector<std::vector<double>> y_data_list(len, std::vector<double>(8));
    x_data_list = {
        //{ 0, 1, 0, 0, 5, 5, 10, 75 },
        //{ 1, 0, 0, 0, 5, 5, 10, 75 }
        { -0.5593004, -0.7067148, -1.396623, -1.514813, 2, 2, 2, 75 },
        { -0.7067148, -0.8638186, -1.514813, -1.588326, 2, 2, 2, 75 },
        { -0.8638186, -1.026053, -1.588326, -1.616257, 2, 2, 2, 75 },
        { -1.026053, -1.188802, -1.616257, -1.598524, 2, 2, 2, 75 },
        { -1.188802, -1.347447, -1.598524, -1.535044, 2, 2, 2, 75 },
        { -1.347447, -1.497373, -1.535044, -1.425882, 2, 2, 2, 75 },
        { -1.497373, -1.633974, -1.425882, -1.271283, 2, 2, 2, 75 },
        { -1.633974, -1.752733, -1.271283, -1.074105, 2, 2, 2, 75 },
        { -1.752733, -1.850087, -1.074105, -0.8503662, 2, 2, 2, 75 },
        { -1.850087, -1.924048, -0.8503662, -0.6122551, 2, 2, 2, 75 },
        { -1.924048, -1.973482, -0.6122551, -0.3653864, 2, 2, 2, 75 },
        { -1.973482, -1.997684, -0.3653864, -0.1131792, 2, 2, 2, 75 },
        { -1.997684, -1.996253, -0.1131792, 0.1416853, 2, 2, 2, 75 },
        { -1.996253, -1.96915, 0.1416853, 0.3937327, 2, 2, 2, 75 },
        { -1.96915, -1.917175, 0.3937327, 0.6317842, 2, 2, 2, 75 },
        { -1.917175, -1.842573, 0.6317842, 0.8382132, 2, 2, 2, 75 },
        { -1.842573, -1.749296, 0.8382132, 1.001539, 2, 2, 2, 75 },
        { -1.749296, -1.64165, 1.001539, 1.12304, 2, 2, 2, 75 },
        { -1.64165, -1.523764, 1.12304, 1.204617, 2, 2, 2, 75 },
        { -1.523764, -1.399566, 1.204617, 1.248376, 2, 2, 2, 75 },
        { -1.399566, -1.272803, 1.248376, 1.255426, 2, 2, 2, 75 },
        { -1.272803, -1.147177, 1.255426, 1.225893, 2, 2, 2, 75 },
        { -1.147177, -1.026378, 1.225893, 1.159849, 2, 2, 2, 75 },
        { -1.026378, -0.9140646, 1.159849, 1.057968, 2, 2, 2, 75 },
        { -0.9140646, -0.8138712, 1.057968, 0.9198763, 2, 2, 2, 75 },
        { -0.8138712, -0.7290246, 0.9198763, 0.7589595, 2, 2, 2, 75 },
        { -0.7290246, -0.6606693, 0.7589595, 0.5944827, 2, 2, 2, 75 },
        { -0.6606693, -0.6086587, 0.5944827, 0.4395013, 2, 2, 2, 75 },
        { -0.6086587, -0.5707421, 0.4395013, 0.3164212, 2, 2, 2, 75 },
        { -0.5707421, -0.5439812, 0.3164212, 0.2169981, 2, 2, 2, 75 }
    };
    y_data_list = {
        //{ 0, 1, 0, 0, 5, 5, 10, 75 },
        //{ 1, 0, 0, 0, 5, 5, 10, 75 }
        { 0.8973975, 0.9551162, 0.6788739, 0.4597472, 2, 2, 2, 75 },
        { 0.9551162, 0.9895447, 0.4597472, 0.2188767, 2, 2, 2, 75 },
        { 0.9895447, 0.9988242, 0.2188767, -0.03711079, 2, 2, 2, 75 },
        { 0.9988242, 0.9817651, -0.03711079, -0.3014678, 2, 2, 2, 75 },
        { 0.9817651, 0.937889, -0.3014678, -0.5667361, 2, 2, 2, 75 },
        { 0.937889, 0.8675485, -0.5667361, -0.8237159, 2, 2, 2, 75 },
        { 0.8675485, 0.7721788, -0.8237159, -1.059779, 2, 2, 2, 75 },
        { 0.7721788, 0.654757, -1.059779, -1.257747, 2, 2, 2, 75 },
        { 0.654757, 0.5195642, -1.257747, -1.41188, 2, 2, 2, 75 },
        { 0.5195642, 0.3709606, -1.41188, -1.523403, 2, 2, 2, 75 },
        { 0.3709606, 0.2132534, -1.523403, -1.591918, 2, 2, 2, 75 },
        { 0.2132534, 0.05086101, -1.591918, -1.615988, 2, 2, 2, 75 },
        { 0.05086101, -0.1116732, -1.615988, -1.594743, 2, 2, 2, 75 },
        { -0.1116732, -0.2697831, -1.594743, -1.528308, 2, 2, 2, 75 },
        { -0.2697831, -0.4189067, -1.528308, -1.416867, 2, 2, 2, 75 },
        { -0.4189067, -0.554528, -1.416867, -1.261271, 2, 2, 2, 75 },
        { -0.554528, -0.6728393, -1.261271, -1.078989, 2, 2, 2, 75 },
        { -0.6728393, -0.7722734, -1.078989, -0.889514, 2, 2, 2, 75 },
        { -0.7722734, -0.8526001, -0.889514, -0.7017476, 2, 2, 2, 75 },
        { -0.8526001, -0.9142769, -0.7017476, -0.5209429, 2, 2, 2, 75 },
        { -0.9142769, -0.9581893, -0.5209429, -0.3504991, 2, 2, 2, 75 },
        { -0.9581893, -0.9855407, -0.3504991, -0.193627, 2, 2, 2, 75 },
        { -0.9855407, -0.9978983, -0.193627, -0.05439415, 2, 2, 2, 75 },
        { -0.9978983, -0.9973025, -0.05439415, 0.06182712, 2, 2, 2, 75 },
        { -0.9973025, -0.9864266, 0.06182712, 0.1477116, 2, 2, 2, 75 },
        { -0.9864266, -0.9687073, 0.1477116, 0.1971192, 2, 2, 2, 75 },
        { -0.9687073, -0.9478081, 0.1971192, 0.2108644, 2, 2, 2, 75 },
        { -0.9478081, -0.9271623, 0.2108644, 0.1942402, 2, 2, 2, 75 },
        { -0.9271623, -0.9089998, 0.1942402, 0.1639914, 2, 2, 2, 75 },
        { -0.9089998, -0.8943051, 0.1639914, 0.1261828, 2, 2, 2, 75 }
    };

    compute_2d(len, x_data_list, y_data_list, epoch_num);
    std::ofstream endfile;
    endfile.open("data/end.txt");
    for (int i = 0; i < len; i++) {
        endfile << x_data_list[i][1] << " ";
        endfile << x_data_list[i][3] << " ";
    }
    endfile << std::endl;
    for (int i = 0; i < len; i++) {
        endfile << y_data_list[i][1] << " ";
        endfile << y_data_list[i][3] << " ";
    }
    endfile.close();
}
