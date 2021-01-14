import math
import matplotlib.pyplot as plt

vzero = 0.001


def copy_sign(a, b):
    if b >= 0:
        return abs(a)
    else:
        return -abs(a)


# all_info_dict = [a,total_time]


# def compute_1d(x, v0, v1, a_max, d_max, v_max, frame_rate, all_info_dict,
def compute_1d(x, v0, v1, a_max, d_max, v_max, frame_rate, all_info_dict):
    delta_t = 1 / frame_rate


    if abs(x) < 1e-5 and abs(v0 - v1) < 1e-5:
        all_info_dict["a"] = 0
        all_info_dict["dec_time"] = 0
        all_info_dict["flat_time"] = 0
        all_info_dict["acc_time"] = 0
        return

    if math.isinf(x) or math.isinf(v0) or math.isinf(v1):
        return

    # if abs(v0) > v_max:
        # all_info_dict["a"] = d_max
        # # all_info_dict["a"] = 0
        # all_info_dict["dec_time"] = 1/frame_rate
        # all_info_dict["flat_time"] = 0
        # all_info_dict["acc_time"] = 0
        # return

    if abs(abs(v0)-v_max) < 0.05*v_max:
        if v1 * v0 > 0:
            total_x_v0_to_v1 = abs((v0+v1)/2*(v0-v1)/d_max)
        else:
            total_x_v0_to_v1 = v0*v0/(2*d_max) - v1*v1/(2*a_max)
        if total_x_v0_to_v1 < abs(x):
            all_info_dict["a"] = 0
        else:
            all_info_dict["a"] = copy_sign(d_max,-v0)
        return

    if abs(v0) < a_max/frame_rate:
        v0 = copy_sign(a_max/frame_rate,v0)

    if abs(v1) < a_max/frame_rate:
        if abs(x) < 0.0015:
            all_info_dict["a"] = 0
            return
        else:
            v1 = copy_sign(vzero,v1)

    if v0 * v1 > 0:
        if x * v0 > 0:
            # state1 & state2 & state3 Done
            if abs(v1) > abs(v0):
                total_t_v0_to_v1 = abs(v0 - v1) / a_max
                total_x_v0_to_v1 = abs(v0 + v1) / 2 * total_t_v0_to_v1
                # state1
                if abs(x) <= total_x_v0_to_v1:
                    print("state1:")
                    acc_time = (-abs(v0) +
                                math.sqrt(v0**2 + 2 * a_max * abs(x))) / a_max
                    all_info_dict["acc_time"] += acc_time
                    all_info_dict["flat_time"] += 0
                    all_info_dict["dec_time"] += 0
                    all_info_dict["a"] = copy_sign(a_max,v0)
                    print("state1:", "x:", x, "\t", "v0:", v0, "\t",
                          "v1:", v1, "\t", "a:", all_info_dict["a"])
                    return
                # state2 & state3
                elif abs(x) > total_x_v0_to_v1:
                    total_x_v0_to_v_max = abs(v_max**2 - v0**2) / (2 * a_max)
                    total_x_v_max_to_v1 = abs(v_max**2 - v1**2) / (2 * d_max)
                    total_x = total_x_v0_to_v_max + total_x_v_max_to_v1
                    # 改成由vm判断
                    # state2 Done
                    if (abs(x) - total_x) >= 0:
                        print("state2:")
                        acc_time = (v_max - abs(v0)) / a_max
                        flat_time = (abs(x) - total_x) / v_max
                        dec_time = (v_max - abs(v1)) / d_max
                        all_info_dict["acc_time"] += acc_time
                        all_info_dict["flat_time"] += flat_time
                        all_info_dict["dec_time"] += dec_time
                        all_info_dict["a"] = copy_sign(a_max, v0)
                        print("state2:", "x:", x, "\t", "v0:", v0, "\t",
                              "v1:", v1, "\t", "a:", all_info_dict["a"])
                        return
                    # state3 Done
                    elif (abs(x) - total_x) < 0:
                        print("state3:")
                        v_m = math.sqrt(
                            (2 * a_max * d_max * abs(x) + d_max * v0 * v0 +
                             a_max * v1 * v1) / (a_max + d_max))
                        acc_time = (v_m - abs(v0)) / a_max
                        flat_time = 0
                        dec_time = (v_m - abs(v1)) / d_max
                        all_info_dict["acc_time"] += acc_time
                        all_info_dict["flat_time"] += flat_time
                        all_info_dict["dec_time"] += dec_time
                        all_info_dict["a"] = copy_sign(a_max,v0)
                        if v_m - v0 < a_max/frame_rate:  # 这个值与加速度和frame_rate有关，要算出来
                            all_info_dict["a"] = 0
                        print("state3:", "x:", x, "\t", "v0:", v0, "\t",
                              "v1:", v1, "\t", "a:", all_info_dict["a"])
                        return

            # state4
            elif abs(v1) < abs(v0):
                total_t_v0_to_v1 = abs(v0 - v1) / d_max
                total_x_v0_to_v1 = abs(v0 + v1) / 2 * total_t_v0_to_v1
                # <++>
                # 小量需要计算
                # state4
                if abs(x) - total_x_v0_to_v1 < 0.01:  # 参数
                    print("state4:")
                    dec_time = (abs(v0) -
                                math.sqrt(abs(v0**2 - 2 * d_max * abs(x)))) / d_max
                    all_info_dict["acc_time"] += 0
                    all_info_dict["flat_time"] += 0
                    all_info_dict["dec_time"] += dec_time
                    all_info_dict["a"] = copy_sign(d_max,-v0)
                    print("state4:", "x:", x, "\t", "v0:", v0, "\t",
                          "v1:", v1, "\t", "a:", all_info_dict["a"])
                    return
                elif abs(abs(x) - total_x_v0_to_v1) >= 0.01:  # 参数
                    total_x_v0_to_v_max = abs(v_max**2 - v0**2) / (2 * a_max)
                    total_x_v_max_to_v1 = abs(v_max**2 - v1**2) / (2 * d_max)
                    total_x = total_x_v0_to_v_max + total_x_v_max_to_v1
                    # state5
                    if(abs(x) > total_x):
                        print("state5:")
                        acc_time = (abs(v_max)-abs(v0))/a_max
                        flat_time = (abs(x)-total_x)/v_max
                        dec_time = (v_max-abs(v1))/d_max
                        all_info_dict["acc_time"] += acc_time
                        all_info_dict["flat_time"] += flat_time
                        all_info_dict["dec_time"] += dec_time
                        all_info_dict["a"] = copy_sign(a_max,v0)
                        print("state5:", "x:", x, "\t", "v0:", v0, "\t",
                              "v1:", v1, "\t", "a:", all_info_dict["a"])
                        return
                    # state6
                    else:
                        print("state6:")
                        v_m = math.sqrt(
                            (2 * a_max * d_max * abs(x) + d_max * v0 * v0 +
                             a_max * v1 * v1) / (a_max + d_max))
                        acc_time = (v_m - abs(v0)) / a_max
                        flat_time = 0
                        dec_time = (v_m - abs(v1)) / d_max
                        all_info_dict["acc_time"] += acc_time
                        all_info_dict["flat_time"] += flat_time
                        all_info_dict["dec_time"] += dec_time
                        all_info_dict["a"] = copy_sign(a_max,v0)
                        if v_m - abs(v0) < a_max/frame_rate:
                            all_info_dict["a"] = 0
                        print("state6:", "x:", x, "\t", "v0:", v0, "\t", "v1:",
                              v1, "\t", "a:", all_info_dict["a"], "\t", "v_m:", v_m)
                        return

        elif x * v0 < 0:
            total_x_v0_to_0 = v0**2 / (2 * d_max)
            # ERROR
            # compute_1d(copy_sign(total_x_v0_to_0, -x), v0, copy_sign(vzero, -x),
            #            a_max, d_max, v_max, frame_rate, all_info_dict)
            v_m = copy_sign(
                math.sqrt((v0*v0/(2*d_max)+v1*v1/(2*a_max)+abs(x))/(1/(2*a_max)+1/(2*d_max))), -v0)
            total_x_v0_to_0 = copy_sign((v0*v0)/(2*d_max), v0)
            total_x_0_to_v_m = copy_sign((v_m*v_m)/(2*a_max), -v0)
            total_x_v_m_to_0 = copy_sign((v_m*v_m)/(2*d_max), -v0)
            total_x_0_to_v1 = copy_sign((v1*v1)/(2*a_max), v1)
            total_x_0_to_v_max = copy_sign((v_max*v_max)/(2*a_max), -v0)
            total_x_v_max_to_0 = copy_sign((v_max*v_max)/(2*d_max), -v0)
            # state7
            if(abs(v_m) < v_max):
                print("state7:")
                compute_1d(total_x_0_to_v1, copy_sign(vzero, v1), v1,
                           a_max, d_max, v_max, frame_rate, all_info_dict)
                compute_1d(total_x_v_m_to_0, v_m, copy_sign(vzero, -v0),
                           a_max, d_max, v_max, frame_rate, all_info_dict)
                compute_1d(total_x_0_to_v_m, copy_sign(vzero, -v0),
                           v_m, a_max, d_max, v_max, frame_rate, all_info_dict)
                compute_1d(total_x_v0_to_0, v0, copy_sign(vzero, v0),
                           a_max, d_max, v_max, frame_rate, all_info_dict)
                # if abs(v0) > 0.015:
                #     # time
                #     compute_1d(total_x_v0_to_0, v0, copy_sign(vzero, v0),
                #                a_max, d_max, v_max, frame_rate, all_info_dict)
                # else:
                #     compute_1d(copy_sign(abs(x)+total_x_v0_to_0, x), copy_sign(vzero, x), v1,
                #                a_max, d_max, v_max, frame_rate, all_info_dict)
                if all_info_dict["a"] == 0:
                    all_info_dict["a"] = copy_sign(a_max,-v0)
                print("state7:", "x:", x, "\t", "v0:", v0, "\t",
                      "v1:", v1, "\t", "a:", all_info_dict["a"])
                return
            # state8
            else:
                print("state8:")
                compute_1d(total_x_0_to_v1, copy_sign(vzero, v1), v1,
                           a_max, d_max, v_max, frame_rate, all_info_dict)
                compute_1d(total_x_v_max_to_0, v_max, copy_sign(
                    vzero, -v0), a_max, d_max, v_max, frame_rate, all_info_dict)
                compute_1d(x-total_x_v0_to_0-total_x_v_max_to_0-total_x_0_to_v1, copy_sign(
                    vzero, -v0), v_m, a_max, d_max, v_max, frame_rate, all_info_dict)
                compute_1d(total_x_v0_to_0, v0, copy_sign(vzero, v0),
                           a_max, d_max, v_max, frame_rate, all_info_dict)
                if all_info_dict["a"] == 0:
                    all_info_dict["a"] = copy_sign(a_max,-v0)
                print("state8:", "x:", x, "\t", "v0:", v0, "\t",
                      "v1:", v1, "\t", "a:", all_info_dict["a"])

            # compute_1d(copy_sign(total_x_v0_to_0+abs(x), x), copy_sign(vzero, x), v1, a_max, d_max, v_max, frame_rate, all_info_dict)
            # compute_1d()

            return

    elif v0 * v1 < 0:
        # state9
        if x * v0 >= 0:
            print("state9:")
            v1 = copy_sign(min(abs(v1), 5), v1)
            total_x_0_to_v1 = (v1**2) / (2 * a_max)
            # total_x_v0_to_0 = (v0**2) / (2 * d_max)
            compute_1d(copy_sign(total_x_0_to_v1, -x),
                       copy_sign(vzero, -x), v1, a_max, d_max, v_max,
                       frame_rate, all_info_dict)

            compute_1d(copy_sign(abs(x)+total_x_0_to_v1, x), v0, copy_sign(vzero, x), a_max, d_max,
                       v_max, frame_rate, all_info_dict)
            print("state9:", "x:", x, "\t", "v0:", v0, "\t",
                  "v1:", v1, "\t", "a:", all_info_dict["a"])

            # if total_x_v0_to_0 <= total_x_0_to_v1 + abs(x):
            # compute_1d(copy_sign(total_x_0_to_v1, -x), copy_sign(vzero, -x),
            # v1, a_max, d_max, v_max, frame_rate, all_info_dict)

            # compute_1d(copy_sign(total_x_0_to_v1 + abs(x), x), v0,
            # copy_sign(vzero, x), a_max, d_max, v_max, frame_rate,
            # all_info_dict)
            # return
            # elif total_x_v0_to_0 > total_x_0_to_v1 + abs(x):
            # compute_1d(copy_sign(total_x_v0_to_0 - abs(x), -x),
            # copy_sign(vzero, -x), v1, a_max, d_max, v_max,
            # frame_rate, all_info_dict)

            # compute_1d(copy_sign(total_x_v0_to_0, x), v0, copy_sign(vzero, x), a_max, d_max,
            # v_max, frame_rate, all_info_dict)
            # return

        # state10
        elif x * v0 < 0:
            print("state10:")
            total_x_v0_to_0 = v0**2 / (2 * d_max)
            if v0 * v1 < 0 and abs(v0) > 0.015:
                compute_1d(copy_sign(total_x_v0_to_0 + abs(x), x),
                           copy_sign(vzero, x),
                           v1,
                           a_max,
                           d_max,
                           v_max,
                           frame_rate,
                           all_info_dict)

                # compute_1d(copy_sign(total_x_v0_to_0, -x), v0, copy_sign(vzero, x),
                # a_max, d_max, v_max, frame_rate, all_info_dict)
                compute_1d(copy_sign(total_x_v0_to_0, -x), v0, copy_sign(vzero, -x),
                           a_max, d_max, v_max, frame_rate, all_info_dict)
            else:
                # compute_1d(copy_sign(total_x_v0_to_0, -x), v0, copy_sign(vzero, x),
                # a_max, d_max, v_max, frame_rate, all_info_dict)
                compute_1d(copy_sign(total_x_v0_to_0, -x), v0, copy_sign(vzero, -x),
                           a_max, d_max, v_max, frame_rate, all_info_dict)
                compute_1d(copy_sign(total_x_v0_to_0 + abs(x), x),
                           copy_sign(vzero, x),
                           v1,
                           a_max,
                           d_max,
                           v_max,
                           frame_rate,
                           all_info_dict)
                print("*"*10)
            print("state10:", "x:", x, "\t", "v0:", v0, "\t",
                  "v1:", v1, "\t", "a:", all_info_dict["a"])
            return
