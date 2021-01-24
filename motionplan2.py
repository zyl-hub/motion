import math
import matplotlib.pyplot as plt


def copy_sign(a, b):
    if b >= 0:
        return abs(a)
    else:
        return -abs(a)

def compute_1d(x, v0, v1, a_max, d_max, v_max, frame_rate, all_info_dict):
    delta_t = 1 / frame_rate

    if math.isinf(x) or math.isinf(v0) or math.isinf(v1):
        return


    if abs(abs(v0) - v_max) < 5*a_max/frame_rate:
        if v1 * v0 > 0:
            total_x_v0_to_v1 = abs((v0 + v1) / 2 * (v0 - v1) / d_max)
        else:
            total_x_v0_to_v1 = v0 * v0 / (2 * d_max) - v1 * v1 / (2 * a_max)

        if total_x_v0_to_v1 < abs(x):
            print("^" * 10)
            all_info_dict["a"] = 0
        else:
            print("&" * 10)
            all_info_dict["a"] = copy_sign(d_max, -v0)
        return

    if abs(v0) < a_max / frame_rate and abs(v1) < a_max / frame_rate:
        if abs(x) < 0.01:
            # v0 = v0 + all_info_dict["a"] / frame_rate
            all_info_dict["a"] = -v0 / frame_rate / 2
            print("00:",all_info_dict["a"])
            return
        else:
            all_info_dict["a"] = copy_sign(a_max, x)
            print("00:",all_info_dict["a"])
            return

    elif abs(v0) < a_max /frame_rate:
        all_info_dict["a"] = copy_sign(a_max, x)
        return

    elif abs(v1) < a_max / frame_rate:
        if v0 * x < 0:
            all_info_dict["a"] = copy_sign(d_max, -v0)
            return
        elif v0 * x >= 0:
            total_x_v0_to_0 = v0*v0/(2*d_max)
            if abs(x) < total_x_v0_to_0:
                all_info_dict["a"] = copy_sign(d_max,-v0)
                return
            else:
                all_info_dict["a"] = copy_sign(a_max, v0)
                return

    elif v0 * v1 > 0:
        if x * v0 > 0:
            if abs(v1) > abs(v0):
                # total_t_v0_to_v1 = abs(v0 - v1) / a_max
                # total_x_v0_to_v1 = abs(v0 + v1) / 2 * total_t_v0_to_v1
                all_info_dict["a"] = copy_sign(a_max, v0)
                return
            elif abs(v1) < abs(v0):
                total_t_v0_to_v1 = abs(v0 - v1) / d_max
                total_x_v0_to_v1 = abs(v0 + v1) / 2 * total_t_v0_to_v1
                if abs(x) > total_x_v0_to_v1:
                    all_info_dict["a"] = copy_sign(a_max, v0)
                    return
                else:
                    all_info_dict["a"] = copy_sign(d_max, -v0)
                    return
        else:
            all_info_dict["a"] = copy_sign(d_max, -v0)
            return
    elif v0 * v1 < 0:
        if x * v0 >= 0:
            # v1 = copy_sign(min(abs(v1), 5), v1)
            total_x_0_to_v1 = (v1 ** 2) / (2 * a_max)
            compute_1d(copy_sign(abs(x) + total_x_0_to_v1, x), v0, 0, a_max, d_max,
                       v_max, frame_rate, all_info_dict)
            return
        else:
            all_info_dict["a"] = copy_sign(d_max,-v0)

















