import math
import matplotlib.pyplot as plt


def copy_sign(a, b):
    if b >= 0:
        return abs(a)
    else:
        return -abs(a)


# all_info_dict = [a,total_time]


# def compute_1d(x, v0, v1, a_max, d_max, v_max, frame_rate, all_info_dict,
def compute_1d(x, v0, v1, a_max, d_max, v_max, frame_rate, all_info_dict):
    delta_t = 1 / frame_rate

    if x < 1e-5 and abs(v0 - v1) < 1e-5:
        all_info_dict["a"] = 0
        all_info_dict["dec_time"] = 0
        all_info_dict["flat_time"] = 0
        all_info_dict["acc_time"] = 0
        all_info_dict["aord"] = 0
        return

    if math.isinf(x) or math.isinf(v0) or math.isinf(v1):
        return

    if abs(v0) > v_max:
        all_info_dict["a"] = d_max
        all_info_dict["dec_time"] = 1/frame_rate
        all_info_dict["flat_time"] = 0
        all_info_dict["acc_time"] = 0
        all_info_dict["aord"] = 0
        return

    if v0 * v1 > 0:
        if x * v0 > 0:
            if abs(v1) > abs(v0):
                total_t_v0_to_v1 = abs(v0 - v1) / a_max
                total_x_v0_to_v1 = abs(v0 + v1) / 2 * total_t_v0_to_v1
                if abs(x) <= total_x_v0_to_v1:
                    acc_time = (-abs(v0) +
                                math.sqrt(v0**2 + 2 * a_max * abs(x))) / a_max
                    all_info_dict["acc_time"] += acc_time
                    all_info_dict["flat_time"] += 0
                    all_info_dict["dec_time"] += 0
                    all_info_dict["a"] = a_max
                    all_info_dict["aord"] = 1
                    return
                elif abs(x) > total_x_v0_to_v1:
                    total_x_v0_to_v_max = abs(v_max**2 - v0**2) / (2 * a_max)
                    total_x_v_max_to_v1 = abs(v_max**2 - v1**2) / (2 * d_max)
                    total_x = total_x_v0_to_v_max + total_x_v_max_to_v1
                    if abs(x) >= total_x:
                        acc_time = (v_max - abs(v0)) / a_max
                        flat_time = (abs(x) - total_x) / v_max
                        dec_time = (v_max - abs(v1)) / d_max
                        all_info_dict["acc_time"] += acc_time
                        all_info_dict["flat_time"] += flat_time
                        all_info_dict["dec_time"] += dec_time
                        all_info_dict["a"] = a_max
                        all_info_dict["aord"] = 1
                        return
                    elif abs(x) < total_x:
                        v_m = math.sqrt(
                            (2 * a_max * d_max * abs(x) + d_max * v0 * v0 +
                             a_max * v1 * v1) / (a_max + d_max))
                        acc_time = (v_m - abs(v0)) / a_max
                        flat_time = 0
                        dec_time = (v_m - abs(v1)) / d_max
                        all_info_dict["acc_time"] += acc_time
                        all_info_dict["flat_time"] += flat_time
                        all_info_dict["dec_time"] += dec_time
                        all_info_dict["a"] = a_max
                        all_info_dict["aord"] = 1
                        return
            elif abs(v1) < abs(v0):
                total_t_v0_to_v1 = abs(v0 - v1) / d_max
                total_x_v0_to_v1 = abs(v0 + v1) / 2 * total_t_v0_to_v1
                if abs(x) <= total_x_v0_to_v1:
                    dec_time = (abs(v0) -
                                math.sqrt(v0**2 - 2 * d_max * abs(x))) / d_max
                    all_info_dict["acc_time"] += 0
                    all_info_dict["flat_time"] += 0
                    all_info_dict["dec_time"] += dec_time
                    all_info_dict["a"] = d_max
                    all_info_dict["aord"] = 0
                    return
                elif abs(x) > total_x_v0_to_v1:
                    total_x_v0_to_v_max = abs(v_max**2 - v0**2) / (2 * a_max)
                    total_x_v_max_to_v1 = abs(v_max**2 - v1**2) / (2 * d_max)
                    total_x = total_x_v0_to_v_max + total_x_v_max_to_v1
                    if(abs(x) > total_x):
                        acc_time = (abs(v_max)-abs(v0))/a_max
                        flat_time = (abs(x)-total_x)/v_max
                        dec_time = (v_max-abs(v1))/d_max
                        all_info_dict["acc_time"] += acc_time
                        all_info_dict["flat_time"] += flat_time
                        all_info_dict["dec_time"] += dec_time
                        all_info_dict["a"] = a_max
                        all_info_dict["aord"] = 1
                        return
                    else:
                        v_m = math.sqrt(
                            (2 * a_max * d_max * abs(x) + d_max * v0 * v0 +
                             a_max * v1 * v1) / (a_max + d_max))
                        acc_time = (v_m - abs(v0)) / a_max
                        flat_time = 0
                        dec_time = (v_m - abs(v1)) / d_max
                        all_info_dict["acc_time"] += acc_time
                        all_info_dict["flat_time"] += flat_time
                        all_info_dict["dec_time"] += dec_time
                        all_info_dict["a"] = a_max
                        all_info_dict["aord"] = 1
                        return

        elif x * v0 < 0:
            total_x_v0_to_0 = v0**2 / (2 * d_max)
            # ERROR
            compute_1d(copy_sign(total_x_v0_to_0, -x), v0, copy_sign(1e-8, -x),
                       a_max, d_max, v_max, frame_rate, all_info_dict)
            v_m = copy_sign(
                math.sqrt((v0*v0/(2*d_max)+v1*v1/(2*a_max)+abs(x))/(1/(2*a_max)+1/(2*d_max))), -v0)
            total_x_v0_to_0 = copy_sign((v0*v0)/(2*d_max), v0)
            total_x_0_to_v_m = copy_sign((v_m*v_m)/(2*a_max), -v0)
            total_x_v_m_to_0 = copy_sign((v_m*v_m)/(2*d_max), -v0)
            total_x_0_to_v1 = copy_sign((v1*v1)/(2*a_max), v1)
            total_x_0_to_v_max = copy_sign((v_max*v_max)/(2*a_max), -v0)
            total_x_v_max_to_0 = copy_sign((v_max*v_max)/(2*d_max), -v0)
            if(abs(v_m) < v_max):
                compute_1d(total_x_0_to_v1, copy_sign(1e-8, v1), v1,
                           a_max, d_max, v_max, frame_rate, all_info_dict)
                compute_1d(total_x_v_m_to_0, v_m, copy_sign(1e-8, -v0),
                           a_max, d_max, v_max, frame_rate, all_info_dict)
                compute_1d(total_x_0_to_v_m, copy_sign(1e-8, -v0),
                           v_m, a_max, d_max, v_max, frame_rate, all_info_dict)
                compute_1d(total_x_v0_to_0, v0, copy_sign(1e-8, v0),
                           a_max, d_max, v_max, frame_rate, all_info_dict)
            else:
                compute_1d(total_x_0_to_v1, copy_sign(1e-8, v1), v1,
                           a_max, d_max, v_max, frame_rate, all_info_dict)
                compute_1d(total_x_v_max_to_0, v_max, copy_sign(
                    1e-8, -v0), a_max, d_max, v_max, frame_rate, all_info_dict)
                compute_1d(x-total_x_v0_to_0-total_x_v_max_to_0-total_x_0_to_v1, copy_sign(
                    1e-8, -v0), v_m, a_max, d_max, v_max, frame_rate, all_info_dict)
                compute_1d(total_x_v0_to_0, v0, copy_sign(1e-8, v0),
                           a_max, d_max, v_max, frame_rate, all_info_dict)

            # compute_1d(copy_sign(total_x_v0_to_0+abs(x), x), copy_sign(1e-8, x), v1, a_max, d_max, v_max, frame_rate, all_info_dict)
            # compute_1d()

            return

    elif v0 * v1 < 0:
        if x * v0 >= 0:
            v1 = copy_sign(min(abs(v1),5),v1)
            total_x_0_to_v1=(v1**2) / (2 * a_max)
            total_x_v0_to_0=(v0**2) / (2 * d_max)
            compute_1d(copy_sign(total_x_0_to_v1, -x),
                           copy_sign(1e-8, -x), v1, a_max, d_max, v_max,
                           frame_rate, all_info_dict)

            compute_1d(copy_sign(abs(x)+total_x_0_to_v1, x), v0, copy_sign(1e-8, x), a_max, d_max,
                           v_max, frame_rate, all_info_dict)

            # if total_x_v0_to_0 <= total_x_0_to_v1 + abs(x):
                # compute_1d(copy_sign(total_x_0_to_v1, -x), copy_sign(1e-8, -x),
                           # v1, a_max, d_max, v_max, frame_rate, all_info_dict)

                # compute_1d(copy_sign(total_x_0_to_v1 + abs(x), x), v0,
                           # copy_sign(1e-8, x), a_max, d_max, v_max, frame_rate,
                           # all_info_dict)
                # return
            # elif total_x_v0_to_0 > total_x_0_to_v1 + abs(x):
                # compute_1d(copy_sign(total_x_v0_to_0 - abs(x), -x),
                           # copy_sign(1e-8, -x), v1, a_max, d_max, v_max,
                           # frame_rate, all_info_dict)

                # compute_1d(copy_sign(total_x_v0_to_0, x), v0, copy_sign(1e-8, x), a_max, d_max,
                           # v_max, frame_rate, all_info_dict)
                # return

        elif x * v0 < 0:
            total_x_v0_to_0=v0**2 / (2 * d_max)
            compute_1d(copy_sign(total_x_v0_to_0 + abs(x), x),
                       copy_sign(1e-8, x), v1, a_max, d_max, v_max, frame_rate,
                       all_info_dict)

            compute_1d(copy_sign(total_x_v0_to_0, -x), v0, copy_sign(1e-8, x),
                       a_max, d_max, v_max, frame_rate, all_info_dict)
            return


def confirm_v1(v0, acc_time, dec_time):
    v1=v0 + 10 * acc_time - 9 * dec_time
    # print("v1:", v1)
    return v1


def confirm_x(v0, acc_time, dec_time, flat_time=0):
    v1=confirm_v1(v0, acc_time, dec_time)
    v_max=v0 + 10 * acc_time
    x=acc_time * (v_max + v0) / 2 + dec_time * \
        (v_max + v1) / 2 + flat_time * v_max
    print("v_max:", v_max)
    print("x:", x)
    return x


if __name__ == "__main__":
    # a_max: 10
    # d_max: 9
    # frame_rate: 75
    # x, v0, v1, a_max, d_max, v_max, frame_rate
    # data_test = {"a": 0, "acc_time": 0, "flat_time": 0, "dec_time": 0}
    # compute_1d(x=-2,
    #            v0=1,
    #            v1=3,
    #            a_max=10,
    #            d_max=8,
    #            v_max=40,
    #            frame_rate=75,
    #            all_info_dict=data_test)
    # print(data_test)
    # confirm_x(1, data_test["acc_time"], data_test["dec_time"], data_test["flat_time"])
    x0=1
    x1=4
    v0=2
    v1=3
    epoch=0
    frame_rate=75
    delta_t=1 / frame_rate
    data_test={"a": 0, "acc_time": 0, "flat_time": 0, "dec_time": 0}
    epoch_list=[]
    x_list=[]
    v_list=[]
    x_standard=[]
    while not (epoch > 50):
        compute_1d(x=x1 - x0,
                   v0=v0,
                   v1=v1,
                   a_max=10,
                   d_max=8,
                   v_max=40,
                   frame_rate=75,
                   all_info_dict=data_test)
        if data_test["a"] == 10:
            delta_v=data_test["a"] * delta_t
            delta_x=(2 * v0 + delta_t * data_test["a"]) / 2 * delta_t
            v0 += delta_v
            x0 += delta_x

        else:
            delta_v=-data_test["a"] * delta_t
            delta_x=(2 * v0 + delta_t * data_test["a"]) / 2 * delta_t
            v0 += delta_v
            x0 += delta_x
        epoch += 1
        epoch_list.append(epoch)
        x_list.append(x0)
        # x_standard.append(3)
        # flag = False
        # if x0 > 3 and not flag:
        # v1 = 1
        # x1 = 6
        # flag = True

        v_list.append(v0)
        # print(x0)
        if x0 > 4:
            break
        # break
    # print(epoch_list)
    # plt.plot(epoch_list[0:], x_list[0:], 'bo-', markersize=2)
    # plt.plot(epoch_list[0:], v_list[0:], 'ro-', markersize=2)
    # print(x_list)
    # print(v_list)
    plt.scatter(epoch_list, v_list)
    plt.show()
