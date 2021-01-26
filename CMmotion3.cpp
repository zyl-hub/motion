void compute_motion_1d(double x, double v0, double v1, double a_max, double d_max, double v_max, double a_factor, double vel_factor, double& traj_accel, double& traj_time, double& traj_time_acc, double& traj_time_dec, double& traj_time_flat, planType pT, nonZeroMode mode) {
    double frame_rate = 75.0;
    double vzero = 0.001;
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
    double true_x = 0;
    double delta_t_change = 0;
    double delta_v = 0;
    double abs_d = 0;
    char adebugmsg[10];

    if (abs(x) > 9) {
        x = copysign(abs(x) - 9, x);
    }
    else {
        x = x;
    }

    if (!finite(x) || !finite(v0) || !finite(v1)) {
        return;
    }

    if (abs(abs(v0) - v_max) < 2 * a_max * delta_t) {
        if (v1 * v0 >= 0) {
            total_x_v0_to_v1 = abs((v0 + v1) / 2 * (v0 - v1) / d_max);
        }
        else {
            total_x_v0_to_v1 = v0 * v0 / (2 * d_max) - v1 * v1 / (2 * a_max);
        }

        true_x = abs(x) - 2 * v0 * delta_t;
        //true_x = abs(x) - 2 * v0 * delta_t - 9;
        //if (true_x >= 0) {
        //    true_x = true_x;
        //}
        //else{
        //    true_x = abs(x) - 2 * v0 * delta_t;
        //}

        if (total_x_v0_to_v1 < abs(true_x)) {
            delta_t_change = (abs(true_x) - total_x_v0_to_v1) / (v_max - v0);
            if (delta_t_change < 2 * delta_t) {
                delta_v = d_max * (2 * delta_t - delta_t_change);
                traj_accel = copysign(abs(delta_v / delta_t), -v0);
                sprintf(adebugmsg, "%%.%dlf", traj_accel);
                GDebugEngine::Instance()->gui_debug_msg(CGeoPoint(-320, 200), adebugmsg, COLOR_YELLOW);
            }
            else {
                traj_accel = 0;
                sprintf(adebugmsg, "%%.%dlf", traj_accel);
                GDebugEngine::Instance()->gui_debug_msg(CGeoPoint(-320, 200), adebugmsg, COLOR_YELLOW);
            }
            std::cout << "x:" << x << "v0:" << v0 << "a:" << traj_accel;
            return;
        }
        else {
            traj_accel = copysign(d_max, -v0);
            sprintf(adebugmsg, "%%.%dlf", traj_accel);
            GDebugEngine::Instance()->gui_debug_msg(CGeoPoint(-320, 200), adebugmsg, COLOR_YELLOW);
            std::cout << "x:" << x << "v0:" << v0 << "a:" << traj_accel;
            return;
        }
    }

    if (abs(v0) < a_max * delta_t && abs(v1) < a_max * delta_t) {
        if (abs(x) < 0.3) {
            traj_accel = 0.5 * (v1 - v0) / delta_t;
            sprintf(adebugmsg, "%%.%dlf", traj_accel);
            GDebugEngine::Instance()->gui_debug_msg(CGeoPoint(-320, 200), adebugmsg, COLOR_YELLOW);
            std::cout << "x:" << x << "v0:" << v0 << "a:" << traj_accel;
            return;
        }
        else {
            traj_accel = copysign(a_max, x);
            sprintf(adebugmsg, "%%.%dlf", traj_accel);
            GDebugEngine::Instance()->gui_debug_msg(CGeoPoint(-320, 200), adebugmsg, COLOR_YELLOW);
            std::cout << "x:" << x << "v0:" << v0 << "a:" << traj_accel;
            return;
        }
    }

    else if (abs(v0) < a_max * delta_t) {
        traj_accel = copysign(a_max, x);
        sprintf(adebugmsg, "%%.%dlf", traj_accel);
        GDebugEngine::Instance()->gui_debug_msg(CGeoPoint(-320, 200), adebugmsg, COLOR_YELLOW);
        std::cout << "x:" << x << "v0:" << v0 << "a:" << traj_accel;
        return;
    }

    else if (abs(v1) < a_max * delta_t) {
        if (v0 * x < 0) {
            traj_accel = copysign(d_max, -v0);
            sprintf(adebugmsg, "%%.%dlf", traj_accel);
            GDebugEngine::Instance()->gui_debug_msg(CGeoPoint(-320, 200), adebugmsg, COLOR_YELLOW);
            std::cout << "x:" << x << "v0:" << v0 << "a:" << traj_accel;
            return;
        }
        else {
            v_m = sqrt((abs(x) + v0 * v0 / (2 * a_max)) / (1 / (2 * a_max) + (1 / (2 * d_max))));
            if (v_m - abs(v0) > 2 * a_max * delta_t) {
                traj_accel = copysign(a_max, v0);
                sprintf(adebugmsg, "%%.%dlf", traj_accel);
                GDebugEngine::Instance()->gui_debug_msg(CGeoPoint(-320, 200), adebugmsg, COLOR_YELLOW);
                std::cout << "x:" << x << "v0:" << v0 << "a:" << traj_accel;
                return;
            }
            else {
                abs_d = std::min((v0 * v0 - v1 * v1) / (2 * abs(x)), d_max);
                //abs_d = 1.2*(abs_d + d_max) / 2;
                traj_accel = copysign(abs_d, -v0);
                sprintf(adebugmsg, "%%.%dlf", traj_accel);
                GDebugEngine::Instance()->gui_debug_msg(CGeoPoint(-320, 200), adebugmsg, COLOR_YELLOW);
                std::cout << "x:" << x << "v0:" << v0 << "a:" << traj_accel;
                return;
            }
        }
    }

    else if (v0 * v1 > 0) {
        if (x * v0 > 0) {
            if (abs(v1) > abs(v0)) {
                // total_t_v0_to_v1 = abs(v0 - v1) / a_max
                // total_x_v0_to_v1 = abs(v0 + v1) / 2 * total_t_v0_to_v1
                traj_accel = copysign(a_max, v0);
                sprintf(adebugmsg, "%%.%dlf", traj_accel);
                GDebugEngine::Instance()->gui_debug_msg(CGeoPoint(-320, 200), adebugmsg, COLOR_YELLOW);
                std::cout << "x:" << x << "v0:" << v0 << "a:" << traj_accel;
                return;
            }
            else if (abs(v1) < abs(v0)) {
                total_t_v0_to_v1 = abs(v0 - v1) / d_max;
                total_x_v0_to_v1 = abs(v0 + v1) / 2 * total_t_v0_to_v1;
                if (abs(x) - total_x_v0_to_v1 > -1) {
                    traj_accel = copysign(a_max, v0);
                    sprintf(adebugmsg, "%%.%dlf", traj_accel);
                    GDebugEngine::Instance()->gui_debug_msg(CGeoPoint(-320, 200), adebugmsg, COLOR_YELLOW);
                }
                else {
                    traj_accel = copysign(d_max, -v0);
                    sprintf(adebugmsg, "%%.%dlf", traj_accel);
                    GDebugEngine::Instance()->gui_debug_msg(CGeoPoint(-320, 200), adebugmsg, COLOR_YELLOW);
                }
                std::cout << "x:" << x << "v0:" << v0 << "a:" << traj_accel;
                return;
            }
        }
        else {
            traj_accel = copysign(d_max, -v0);
            std::cout << "x:" << x << "v0:" << v0 << "a:" << traj_accel;
            return;
        }
    }
    else if (v0 * v1 < 0) {
        if (x * v0 >= 0) {
            // v1 = copysign(min(abs(v1), 5), v1)
            total_x_0_to_v1 = (v1 * v1) / (2 * a_max);
            compute_motion_1d(copysign(abs(x) + total_x_0_to_v1, x), v0, 0, a_max, d_max,
                v_max, a_factor, vel_factor, traj_accel, traj_time, traj_time_acc, traj_time_dec, traj_time_flat, pT, mode);
            return;
        }
        else {
            traj_accel = copysign(d_max, -v0);
            sprintf(adebugmsg, "%%.%dlf", traj_accel);
            GDebugEngine::Instance()->gui_debug_msg(CGeoPoint(-320, 200), adebugmsg, COLOR_YELLOW);
            std::cout << "x:" << x << "v0:" << v0 << "a:" << traj_accel;
            return;
        }
    }
}