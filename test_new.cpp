#include <algorithm>
#include <fstream>
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
                if (traj_accel == 0) {
                    traj_accel = copysign(a_max, -v0);
                }
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

void compute_2d(double* x_data_list, double* y_data_list, int epoch_num)
{
    double* x = new double[epoch_num];
    double* y = new double[epoch_num];
    double* v_x = new double[epoch_num];
    double* v_y = new double[epoch_num];
    double* a_x = new double[epoch_num];
    double* a_y = new double[epoch_num];
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
    while (1) {
        if (epoch == 0) {
            std::cout << epoch << ":";
            compute_motion_1d(x_data_list[1] - x_data_list[0],
                x_data_list[2],
                x_data_list[3],
                x_data_list[4],
                x_data_list[5],
                x_data_list[6],
                none,
                none,
                x_traj_accel,
                x_traj_time,
                x_traj_time_acc,
                x_traj_time_dec,
                x_traj_time_flat,
                frame_rate);
            x[epoch] = x_data_list[0];
            v_x[epoch] = x_data_list[2];
            a_x[epoch] =x_traj_accel;
            double xinfo = 0;
            double v_xinfo = 0;
            xinfo = computex(
                x_data_list[0], x_data_list[2], 0, x_data_list[7]);
            v_xinfo = computev(
                x_data_list[0], x_data_list[2], 0, x_data_list[7]);
            x_data_list[0] = xinfo;
            x_data_list[2] = v_xinfo;
            if (0) {
            } else {
                std::cout << epoch << ":";
                compute_motion_1d(y_data_list[1] - y_data_list[0],
                    y_data_list[2],
                    y_data_list[3],
                    y_data_list[4],
                    y_data_list[5],
                    y_data_list[6],
                    none,
                    none,
                    y_traj_accel,
                    y_traj_time,
                    y_traj_time_acc,
                    y_traj_time_dec,
                    y_traj_time_flat,
                    frame_rate);
                y[epoch] = y_data_list[0];
                v_y[epoch] = y_data_list[2];
                a_y[epoch] = y_traj_accel;
                double yinfo = 0;
                double v_yinfo = 0;
                yinfo = computex(
                    y_data_list[0], y_data_list[2], 0, y_data_list[7]);
                v_yinfo = computev(
                    y_data_list[0], y_data_list[2], 0, y_data_list[7]);
                y_data_list[0] = xinfo;
                y_data_list[2] = v_yinfo;
            }
        } else {
            a_x[epoch] =x_traj_accel;
            x[epoch] = x_data_list[0];
            v_x[epoch] = x_data_list[2];
            double xinfo = 0;
            double v_xinfo = 0;
            xinfo = computex(
                x_data_list[0], x_data_list[2],x_traj_accel, x_data_list[7]);
            v_xinfo = computev(
                x_data_list[0], x_data_list[2],x_traj_accel, x_data_list[7]);
            std::cout << epoch << ":";
            compute_motion_1d(x_data_list[1] - x_data_list[0],
                x_data_list[2],
                x_data_list[3],
                x_data_list[4],
                x_data_list[5],
                x_data_list[6],
                none,
                none,
                x_traj_accel,
                x_traj_time,
                x_traj_time_acc,
                x_traj_time_dec,
                x_traj_time_flat,
                frame_rate);
            x_data_list[0] = xinfo;
            x_data_list[2] = v_xinfo;
            if (0) {
            } else {
                a_y[epoch] = y_traj_accel;
                y[epoch] = y_data_list[0];
                v_y[epoch] = y_data_list[2];
                double yinfo = 0;
                double v_yinfo = 0;
                yinfo = computex(
                    y_data_list[0], y_data_list[2],y_traj_accel, y_data_list[7]);
                v_yinfo = computev(
                    y_data_list[0], y_data_list[2],y_traj_accel, y_data_list[7]);
                std::cout << epoch << ":";
                compute_motion_1d(y_data_list[1] - y_data_list[0],
                    y_data_list[2],
                    y_data_list[3],
                    y_data_list[4],
                    y_data_list[5],
                    y_data_list[6],
                    none,
                    none,
                    y_traj_accel,
                    y_traj_time,
                    y_traj_time_acc,
                    y_traj_time_dec,
                    y_traj_time_flat,
                    frame_rate);
            }
        }
        if (epoch > epoch_num) {
            break;
        }

        epoch += 1;
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
    for (int i = 0; i < epoch_num; i++) {
        xfile << x[i] << " ";
    }
    for (int i = 0; i < epoch_num; i++) {
        yfile << y[i] << " ";
    }
    for (int i = 0; i < epoch_num; i++) {
        v_xfile << v_x[i] << " ";
    }
    for (int i = 0; i < epoch_num; i++) {
        v_yfile << v_y[i] << " ";
    }
    for (int i = 0; i < epoch_num; i++) {
        a_xfile << a_x[i] << " ";
    }
    for (int i = 0; i < epoch_num; i++) {
        a_yfile << a_y[i] << " ";
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
    int epoch_num = 2000;
    double* x_data_list = new double[8];
    double* y_data_list = new double[8];
    x_data_list[0] = 1.0;
    x_data_list[1] = 2.4;
    x_data_list[2] = 1.0;
    x_data_list[3] = 2.0;
    x_data_list[4] = 1.0;
    x_data_list[5] = 1.0;
    x_data_list[6] = 10.0;
    x_data_list[7] = 75.0;
    y_data_list[0] = 1.0;
    y_data_list[1] = 2.4;
    y_data_list[2] = 1.0;
    y_data_list[3] = 2.0;
    y_data_list[4] = 1.0;
    y_data_list[5] = 1.0;
    y_data_list[6] = 10.0;
    y_data_list[7] = 75.0;
    compute_2d(x_data_list, y_data_list, epoch_num);
}
