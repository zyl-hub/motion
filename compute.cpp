#include "CMmotion.h"
#include <fstream>
#include <iostream>
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
    double traj_accel = 0;
    double traj_time = 0;
    double traj_time_acc = 0;
    double traj_time_dec = 0;
    double traj_time_flat = 0;
    double frame_rate = 75;
    int epoch = 0;
    double none = 0;
    double x_last_all_info_dict[4] = { 0 };
    double y_last_all_info_dict[4] = { 0 };
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
                traj_accel,
                traj_time,
                traj_time_acc,
                traj_time_dec,
                traj_time_flat,
                frame_rate);
            x[epoch] = x_data_list[0];
            v_x[epoch] = x_data_list[2];
            a_x[epoch] = x_last_all_info_dict[0];
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
                    traj_accel,
                    traj_time,
                    traj_time_acc,
                    traj_time_dec,
                    traj_time_flat,
                    frame_rate);
                x[epoch] = y_data_list[0];
                v_x[epoch] = y_data_list[2];
                a_x[epoch] = x_last_all_info_dict[0];
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
            a_x[epoch] = x_last_all_info_dict[0];
            x[epoch] = x_data_list[0];
            v_x[epoch] = x_data_list[2];
            double xinfo = 0;
            double v_xinfo = 0;
            xinfo = computex(
                x_data_list[0], x_data_list[2], x_last_all_info_dict[0], x_data_list[7]);
            v_xinfo = computev(
                x_data_list[0], x_data_list[2], x_last_all_info_dict[0], x_data_list[7]);
            std::cout << epoch << ":";
            compute_motion_1d(x_data_list[1] - x_data_list[0],
                x_data_list[2],
                x_data_list[3],
                x_data_list[4],
                x_data_list[5],
                x_data_list[6],
                none,
                none,
                traj_accel,
                traj_time,
                traj_time_acc,
                traj_time_dec,
                traj_time_flat,
                frame_rate);
            x_data_list[0] = xinfo;
            x_data_list[2] = v_xinfo;
            if (0) {
            } else {
                a_y[epoch] = y_last_all_info_dict[0];
                y[epoch] = y_data_list[0];
                v_y[epoch] = y_data_list[2];
                double yinfo = 0;
                double v_yinfo = 0;
                yinfo = computex(
                    y_data_list[0], y_data_list[2], x_last_all_info_dict[0], y_data_list[7]);
                v_yinfo = computev(
                    y_data_list[0], y_data_list[2], x_last_all_info_dict[0], y_data_list[7]);
                std::cout << epoch << ":";
                compute_motion_1d(y_data_list[1] - y_data_list[0],
                    y_data_list[2],
                    y_data_list[3],
                    y_data_list[4],
                    y_data_list[5],
                    y_data_list[6],
                    none,
                    none,
                    traj_accel,
                    traj_time,
                    traj_time_acc,
                    traj_time_dec,
                    traj_time_flat,
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
