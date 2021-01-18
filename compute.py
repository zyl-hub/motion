from motionplan import compute_1d
from motionplan import copy_sign


def compute(x0, v0, a, frame_rate):
    v = v0 + a / frame_rate
    x = x0 + (v + v0) / frame_rate / 2

    return x, v


def compute_2d(x_data_list, y_data_list, epoch_num=2000):
    # compute x
    x = []
    y = []
    v_x = []
    v_y = []
    a_x = []
    a_y = []
    epoch = 0
    epoch_break = 0
    x_last_all_info_dict = {"a": 0,
                            "dec_time": 0,
                            "flat_time": 0,
                            "acc_time": 0}
    y_last_all_info_dict = {"a": 0,
                            "dec_time": 0,
                            "flat_time": 0,
                            "acc_time": 0}
    for i in range(len(x_data_list)):
        while(1):
            if(epoch == 0):
                print(epoch, ":", end="")
                compute_1d(x_data_list[i][1]-x_data_list[i][0],
                           x_data_list[i][2],
                           x_data_list[i][3],
                           x_data_list[i][4],
                           x_data_list[i][5],
                           x_data_list[i][6],
                           x_data_list[i][7],
                           x_last_all_info_dict)
                x.append(x_data_list[i][0])
                v_x.append(x_data_list[i][2])
                a_x.append(x_last_all_info_dict["a"])
                info = compute(
                    x_data_list[i][0], x_data_list[i][2], 0, x_data_list[i][7])
                x_data_list[i][0] = info[0]
                x_data_list[i][2] = info[1]
                if(len(y_data_list[i]) == 0):
                    pass
                else:
                    print(epoch, ":", end="")
                    compute_1d(y_data_list[i][1] - y_data_list[i][0],
                               y_data_list[i][2],
                               y_data_list[i][3],
                               y_data_list[i][4],
                               y_data_list[i][5],
                               y_data_list[i][6],
                               y_data_list[i][7],
                               y_last_all_info_dict)
                    y.append(y_data_list[i][0])
                    v_y.append(y_data_list[i][2])
                    a_y.append(y_last_all_info_dict["a"])
                    info = compute(
                        y_data_list[i][0], y_data_list[i][2], 0, y_data_list[i][7])
                    y_data_list[i][0] = info[0]
                    y_data_list[i][2] = info[1]
            else:
                a_x.append(x_last_all_info_dict["a"])
                x.append(x_data_list[i][0])
                v_x.append(x_data_list[i][2])
                info = compute(x_data_list[i][0], x_data_list[i][2], x_last_all_info_dict["a"],
                               x_data_list[i][7])
                print(epoch, ":", end="")
                compute_1d(x_data_list[i][1] - x_data_list[i][0],
                           x_data_list[i][2],
                           x_data_list[i][3],
                           x_data_list[i][4],
                           x_data_list[i][5],
                           x_data_list[i][6],
                           x_data_list[i][7],
                           x_last_all_info_dict)
                x_data_list[i][0] = info[0]
                x_data_list[i][2] = info[1]
                if (len(y_data_list[i]) == 0):
                    pass
                else:
                    a_y.append(y_last_all_info_dict["a"])
                    y.append(y_data_list[i][0])
                    v_y.append(y_data_list[i][2])
                    info = compute(
                        y_data_list[i][0], y_data_list[i][2], y_last_all_info_dict["a"], y_data_list[i][7])
                    print(epoch, ":", end="")
                    compute_1d(y_data_list[i][1] - y_data_list[i][0],
                               y_data_list[i][2],
                               y_data_list[i][3],
                               y_data_list[i][4],
                               y_data_list[i][5],
                               y_data_list[i][6],
                               y_data_list[i][7],
                               y_last_all_info_dict)
                    y_data_list[i][0] = info[0]
                    y_data_list[i][2] = info[1]
            if abs(x_data_list[i][0] - x_data_list[i][1]) < 3 * x_data_list[i][2] / x_data_list[i][7]:
                if(len(y_data_list[i]) == 0):
                    break
                else:
                    if abs(y_data_list[i][0] - y_data_list[i][1]) < 3 * y_data_list[i][2] / y_data_list[i][7]:
                        break

            if epoch_break > epoch_num/len(x_data_list):
                epoch_break = 0
                break

            epoch_break += 1
            epoch += 1

    return x, y, v_x, v_y, a_x, a_y, range(epoch+1)
