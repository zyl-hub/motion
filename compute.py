from motionplan import compute_1d
from motionplan import copy_sign


def compute(x0, v0, a, frame_rate, aord, v_max):
    # aord == 1 acc
    # aord == 0 dcc
    if aord == 1:
        # if abs(v0) > v_max:
        # a = 0
        v = v0 + copy_sign(a / frame_rate, v0)
    else:
        v = v0 - copy_sign(a / frame_rate, v0)

    x = x0 + (v + v0) / frame_rate / 2

    return x, v


def compute_2d(x_data_list, y_data_list, y_v=None,epoch_num = 2000):
    # compute x
    x = []
    y = []
    v_x = []
    v_y = []
    a_x = []
    a_y = []
    time = []
    epoch = 0
    x_last_all_info_dict = {"a": 0,
                            "dec_time": 0,
                            "flat_time": 0,
                            "acc_time": 0}
    x_now_all_info_dict = {"a": 0,
                           "dec_time": 0,
                           "flat_time": 0,
                           "acc_time": 0}
    y_last_all_info_dict = {"a": 0,
                            "dec_time": 0,
                            "flat_time": 0,
                            "acc_time": 0}
    y_now_all_info_dict = {"a": 0,
                           "dec_time": 0,
                           "flat_time": 0,
                           "acc_time": 0}
    while(1):
        if(epoch == 0):
            compute_1d(x_data_list[1]-x_data_list[0],
                       x_data_list[2],
                       x_data_list[3],
                       x_data_list[4],
                       x_data_list[5],
                       x_data_list[6],
                       x_data_list[7],
                       x_last_all_info_dict)
            x.append(x_data_list[0])
            v_x.append(x_data_list[2])
            if x_last_all_info_dict["aord"]:
                a_x.append(abs(x_last_all_info_dict["a"]))
            else:
                a_x.append(-abs(x_last_all_info_dict["a"]))
            info = compute(
                x_data_list[0], x_data_list[2], 0, x_data_list[7], 1, x_data_list[6])
            x_data_list[0] = info[0]
            x_data_list[2] = info[1]
            if(len(y_data_list) == 0):
                pass
            else:
                compute_1d(y_data_list[1] - y_data_list[0],
                           y_data_list[2],
                           y_data_list[3],
                           y_data_list[4],
                           y_data_list[5],
                           y_data_list[6],
                           y_data_list[7],
                           y_last_all_info_dict)
                y.append(x_data_list[0])
                v_y.append(x_data_list[2])
                if y_last_all_info_dict["aord"]:
                    a_y.append(abs(y_last_all_info_dict["a"]))
                else:
                    a_y.append(-abs(y_last_all_info_dict["a"]))
                info = compute(
                    y_data_list[0], y_data_list[2], 0, y_data_list[7], 1, y_data_list[6])
                y_data_list[0] = info[0]
                y_data_list[2] = info[1]
        else:
            if x_last_all_info_dict["aord"]:
                a_x.append(abs(x_last_all_info_dict["a"]))
            else:
                a_x.append(-abs(x_last_all_info_dict["a"]))
            x.append(x_data_list[0])
            v_x.append(x_data_list[2])
            info = compute(x_data_list[0], x_data_list[2], x_last_all_info_dict["a"],
                           x_data_list[7], x_last_all_info_dict["aord"], x_data_list[6])
            compute_1d(x_data_list[1] - x_data_list[0],
                       x_data_list[2],
                       x_data_list[3],
                       x_data_list[4],
                       x_data_list[5],
                       x_data_list[6],
                       x_data_list[7],
                       x_last_all_info_dict)
            x_data_list[0] = info[0]
            x_data_list[2] = info[1]
            if (len(y_data_list) == 0):
                pass
            else:
                if y_last_all_info_dict["aord"]:
                    a_y.append(abs(y_last_all_info_dict["a"]))
                else:
                    a_y.append(-abs(y_last_all_info_dict["a"]))
                y.append(x_data_list[0])
                v_y.append(x_data_list[2])
                info = compute(y_data_list[0], y_data_list[2], y_last_all_info_dict["a"], y_data_list[7],
                               y_last_all_info_dict["aord"], y_data_list[6])
                compute_1d(y_data_list[1] - x_data_list[0],
                           y_data_list[2],
                           y_data_list[3],
                           y_data_list[4],
                           y_data_list[5],
                           y_data_list[6],
                           y_data_list[7],
                           y_last_all_info_dict)
                y_data_list[0] = info[0]
                y_data_list[2] = info[1]
        if abs(x_data_list[0] - x_data_list[1]) < 1e-2:
            if(len(y_data_list) == 0):
                break
            else:
                if abs(y_data_list[0] - y_data_list[1]) < 1e-2:
                    break

        if epoch > epoch_num:
            break

        epoch += 1

    return x, y, v_x, v_y, a_x, a_y, range(epoch+1)
