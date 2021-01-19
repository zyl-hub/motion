from motionplan import compute_1d
from motionplan import copy_sign


def compute(x0, v0, a, frame_rate):
    v = v0 + a / frame_rate
    x = x0 + (v + v0) / frame_rate / 2

    return x, v

def compute_2d(x_data_list,y_data_list,epoch_num=2000):
    print("x"*80)
    x, v_x, a_x, x_epoch_list = compute_2d_half(x_data_list, epoch_num=epoch_num)
    print("*"*80)
    y, v_y, a_y, y_epoch_list = compute_2d_half(y_data_list, epoch_num=epoch_num)
    print("x_epoch_list:", x_epoch_list)
    print("y_epoch_list:", y_epoch_list)
    return x, y, v_x, v_y, a_x, a_y, x_epoch_list

def compute_2d_half(x_data_list, epoch_num=2000):
    print("x_data_list:",x_data_list)
    # compute x
    x = []
    v_x = []
    a_x = []
    epoch = 0
    epoch_break = 0
    x_last_all_info_dict = {"a": 0,
                            "dec_time": 0,
                            "flat_time": 0,
                            "acc_time": 0}
    x0 = x_data_list[0][0]
    v0 = x_data_list[0][2]
    for i in range(len(x_data_list)):
        while(1):
            if(epoch == 0):
                print(epoch, ":", end="")
                compute_1d(x_data_list[i][1]-x0,
                           v0,
                           x_data_list[i][3],
                           x_data_list[i][4],
                           x_data_list[i][5],
                           x_data_list[i][6],
                           x_data_list[i][7],
                           x_last_all_info_dict)
                x.append(x0)
                v_x.append(v0)
                a_x.append(x_last_all_info_dict["a"])
                info = compute(x0, v0, 0, x_data_list[i][7])
                x0 = info[0]
                v0 = info[1]
            else:
                print(epoch, ":", end="")
                a_x.append(x_last_all_info_dict["a"])
                x.append(x0)
                v_x.append(v0)
                info = compute(x0, v0, x_last_all_info_dict["a"],
                               x_data_list[i][7])
                compute_1d(x_data_list[i][1] - x0,
                           v0,
                           x_data_list[i][3],
                           x_data_list[i][4],
                           x_data_list[i][5],
                           x_data_list[i][6],
                           x_data_list[i][7],
                           x_last_all_info_dict)
                x0 = info[0]
                v0 = info[1]
            if abs(x0 - x_data_list[i][1]) \
                    < abs(v0 / x_data_list[i][7]) + x_data_list[i][4]/2/x_data_list[i][7]/x_data_list[i][7]:
                print("enough")
                break

            if epoch_break > epoch_num/len(x_data_list):
                epoch_break = 0
                break

            epoch_break += 1
            epoch += 1

    return x, v_x, a_x, range(epoch+1)
