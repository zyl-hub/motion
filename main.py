from utils import *
from compute import *


if __name__ == '__main__':
    # x_data_list,y_data_list = loadDataFromTxt(a_max=1,d_max=1,v_max=10,frame_rate=75)
    # print(x_data_list)
    # print(y_data_list)
    # y_data_list = loadData("state2", "1")

    # 0:部分手输
    # 1:全部手输
    # x_data_list = loadDataByHand(0)
    # y_data_list = loadDataByHand(0)
    all_x =[]
    all_y =[]
    all_vx = []
    all_vy = []
    all_ax = []
    all_ay = []
    all_t = []
    x_data_list = [
        [0, 1, 0, 0, 1, 1, 10, 75],
        [1, 1, 0, 0, 1, 1, 10, 75]
    ]
    y_data_list = [
        [0, 0, 0, 0, 1, 1, 10, 75],
        [0, 1, 0, 0, 1, 1, 10, 75]
    ]
    for i in range(len(x_data_list)):
        x, y, vx, vy, ax, ay, t = compute_2d(x_data_list[i], y_data_list[i], epoch_num=20)
        all_x += x
        all_y += y
        all_vx += vx
        all_vy += vy
        all_ax += ax
        all_ay += ay
    # print(x_data_list)
    # print("epoch:", len(x))
    # print("v:", vx[len(vx)-1])
    # print("v_m:", max(vx[0:500]))
    # print("x:", x[len(x)-1])
    # print(x[60:85])
    # print(vx[60:85])
    # print(x[100:130])
    # print(vx[100:130])
    # print(x[850:900])
    # print(ax[850:900])
    all_t = range(len(all_x))
    print(len(all_x))
    print(len(all_y))
    print(len(all_vx))
    print(len(all_vy))
    print(len(all_ax))
    print(len(all_ay))
    print(len(all_t))
    showImg(0, 0, all_x[:], all_vx[:], all_t[:], all_ax[:], None, None, all_y[:], all_vy[:],
            all_ay[:], 5000, 2, render=True, debug=True)
    # showImg(x, vx, t, y, vy, 200, 1)
    # showImg(x, vx, t, y, vy, 200, 2)
