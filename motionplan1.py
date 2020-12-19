import math


def copy_sign(a, b):
    if b >= 0:
        return abs(a)
    else:
        return -abs(a)

# all_info_dict = [a,total_time]

def compute_1d(x, v0, v1, a_max, d_max, v_max, frame_rate, a_in, t_in, all_info_dict):
    a = a_in
    t = t_in
    delta_t = 1 / frame_rate

    if x == 0 and v0 == v1:
        return [0, 0]

    if math.isinf(x) or math.isinf(v0) or math.isinf(v1):
        return None

    if v0 * v1 > 0:
        if x * v0 > 0:
            if abs(v1) > abs(v0):
                total_t_v0_to_v1 = abs(v0 - v1) / a_max
                total_x_v0_to_v1 = abs(v0 + v1) / 2 * total_t_v0_to_v1
                if abs(x) <= total_x_v0_to_v1:
                    t += delta_t
                    all_info_dict["total_time"] = t
                    all_info_dict["a"] = a_max
                    return
                elif abs(x) > total_x_v0_to_v1:
                    total_x_v0_to_v_max = abs(v_max ** 2 - v0 ** 2) / (2 * a_max)
                    total_x_v_max_to_v1 = abs(v_max ** 2 - v1 ** 2) / (2 * d_max)
                    total_x = total_x_v0_to_v_max + total_x_v_max_to_v1
                    if abs(x) >= total_x:
                        acc_time = (v_max - abs(v0)) / a_max
                        flat_time = (abs(x)-total_x)/v_max
                        dec_time = (v_max - abs(v1)) / d_max
                        all_info_dict["total_time"] = acc_time + flat_time + dec_time
                        all_info_dict["a"] = a_max
                        return
                    elif abs(x) < total_x:
                        v_m = math.sqrt((2*a_max*d_max*abs(x)+d_max*v0*v0+a_max*v1*v1)/(a_max+d_max))
                        acc_time = (v_m - abs(v0)) / a_max
                        flat_time = 0
                        dec_time = (v_m - abs(v1)) / d_max
                        total_time = acc_time + flat_time + dec_time
                        all_info_dict["total_time"] = acc_time + flat_time + dec_time
                        all_info_dict["a"] = a_max
                        return

            elif abs(v1) < abs(v0):
                total_t_v0_to_v1 = abs(v0 - v1) / d_max
                total_x_v0_to_v1 = abs(v0 + v1) / 2 * total_t_v0_to_v1
                if abs(x) <= total_x_v0_to_v1:
                    all_info_dict["total_time"] = t
                    all_info_dict["a"] = d_max
                    return
                elif abs(x) > total_x_v0_to_v1:
                    flat_time = (abs(x) - total_x_v0_to_v1) / abs(v0)
                    dec_time = total_t_v0_to_v1
                    total_time = flat_time + dec_time
                    all_info_dict["total_time"] = total_time
                    all_info_dict["a"] = d_max
                    return

        elif x * v0 < 0:
            pass

    elif v0 * v1 < 0:
        if x * v0 >= 0:
            total_x_0_to_v1 = v1**2 / (2*a_max)
            total_x_v0_to_0 = v0**2 / (2*d_max)
            if total_x_v0_to_0 <= total_x_0_to_v1 + abs(x):
                compute_1d(copy_sign(total_x_0_to_v1 + abs(x), x), v0, 0, a_max, d_max, v_max, frame_rate, a_in, t_in, all_info_dict)
                compute_1d(copy_sign(total_x_0_to_v1, -x), 0, v1, a_max, d_max, v_max, frame_rate, a_in, t_in, all_info_dict)
            elif total_x_v0_to_0 > total_x_0_to_v1 + abs(x):
                compute_1d(copy_sign(total_x_v0_to_0, x), v0, 0, a_max, d_max, v_max, frame_rate, a_in, t_in, all_info_dict)
                compute_1d(copy_sign(total_x_v0_to_0-abs(x), -x), 0, v1, a_max, d_max, v_max, frame_rate, a_in, t_in, all_info_dict)
