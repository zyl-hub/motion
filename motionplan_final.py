import math


def compute(x0, v0, a, frame_rate):
    v = v0 + a / frame_rate
    x = x0 + (v + v0) / frame_rate / 2
    return x, v


def copy_sign(a, b):
    if b >= 0:
        return abs(a)
    else:
        return -abs(a)


def compute_1d(x, v0, v1, a_max, d_max, v_max, frame_rate, all_info_dict, debug=1):
    delta_t = 1 / frame_rate

    x, v0 = compute(x, v0, all_info_dict['a'], frame_rate)

    if math.isinf(x) or math.isinf(v0) or math.isinf(v1):
        return

    if abs(v0) + a_max * delta_t > v_max:
        if v1 * v0 >= 0:
            total_x_v0_to_v1 = abs((v0 + v1) / 2 * (v0 - v1) / d_max)
        else:
            total_x_v0_to_v1 = v0 * v0 / (2 * d_max) - v1 * v1 / (2 * a_max)

        if total_x_v0_to_v1 < abs(x):
            delta_t_change = (abs(x) - total_x_v0_to_v1) / abs(v0)
            if delta_t_change < 2*delta_t:
            #     delta_v = d_max * (2*delta_t - delta_t_change)
            #     all_info_dict["a"] = copy_sign(abs(delta_v / delta_t), -v0)
                all_info_dict["a"] = copy_sign(d_max, -v0)
            else:
                all_info_dict["a"] = 0
            # print("x:", x, "v0:", v0, "a:", all_info_dict["a"])
            return
        else:
            all_info_dict["a"] = copy_sign(d_max, -v0)
            # print("x:", x, "v0:", v0, "a:", all_info_dict["a"])
            return

    elif abs(v0) < a_max * delta_t and abs(v1) < a_max * delta_t:
        if abs(x) < 0.5:
            all_info_dict["a"] = (v1 - v0) / delta_t
            if debug == 1:
                print("^-^~~" * 5)
                print('*' * 20)
                print("special_a:", all_info_dict["a"])
                print('*' * 20)
            return
        else:
            all_info_dict["a"] = copy_sign(a_max, x)
            return

    elif abs(v0) < a_max * delta_t:
        all_info_dict["a"] = copy_sign(a_max, x)
        return

    elif abs(v1) < a_max * delta_t:
        if v0 * x < 0:
            all_info_dict["a"] = copy_sign(d_max, -v0)
            return
        else:
            v_m = math.sqrt((abs(x) + v0 * v0 / (2 * a_max)) / (1 / (2 * a_max) + (1 / (2 * d_max))))
            if abs(v0) + a_max * delta_t <= v_m:
                all_info_dict["a"] = copy_sign(a_max, v0)
                print("x:", x, "v0:", v0, "a:", all_info_dict["a"])
                return
            else:
                abs_d = min((v0 * v0 - v1 * v1) / (2 * abs(x)), d_max)
                # abs_d = (abs_d + d_max) / 2
                all_info_dict["a"] = copy_sign(abs_d, -v0)
                print("x:", x, "v0:", v0, "a:", all_info_dict["a"])
                return

    elif v0 * v1 > 0:
        if x * v0 > 0:
            if abs(v1) > abs(v0):
                all_info_dict["a"] = copy_sign(a_max, v0)
                return
            elif abs(v1) < abs(v0):
                total_t_v0_to_v1 = abs(v0 - v1) / d_max
                total_x_v0_to_v1 = abs(v0 + v1) / 2 * total_t_v0_to_v1
                if abs(x) < total_x_v0_to_v1:
                    all_info_dict["a"] = copy_sign(d_max, -v0)
                    return
                else:
                    v_m = math.sqrt((abs(x) + v0 * v0 / (2 * a_max)) / (1 / (2 * a_max) + (1 / (2 * d_max))))
                    if abs(v0) + a_max * delta_t > v_m:
                        abs_d = min((v0 * v0 - v1 * v1) / (2 * abs(x)), d_max)
                        # abs_d = (abs_d + d_max) / 2
                        all_info_dict["a"] = copy_sign(abs_d, -v0)
                        return
                    else:
                        all_info_dict["a"] = copy_sign(a_max, v0)
                        return
        else:
            all_info_dict["a"] = copy_sign(d_max, -v0)
            return

    elif v0 * v1 < 0:
        if x * v0 >= 0:
            total_x_0_to_v1 = (v1 ** 2) / (2 * a_max)
            compute_1d(copy_sign(abs(x) + total_x_0_to_v1, x), v0, 0, a_max, d_max, v_max, frame_rate, all_info_dict)
            return
        else:
            all_info_dict["a"] = copy_sign(d_max, -v0)






