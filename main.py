from utils import *
from compute import *


if __name__ == '__main__':
    x_data_list = loadData("state8", "1")
    print(x_data_list)
    # y_data_list = loadData("state2", "1")

    # 0:部分手输
    # 1:全部手输
    # x_data_list = loadDataByHand(0)
    # y_data_list = loadDataByHand(0)

    x, y, vx, vy, ax, ay, t = compute_2d(x_data_list, [], epoch_num=2500)
    print(x_data_list)
    print("epoch:", len(x))
    print("v:", vx[len(vx)-1])
    print("v_m:", max(vx[0:500]))
    print("x:", x[len(x)-1])
    # print(x[60:85])
    # print(vx[60:85])
    # print(x[100:130])
    # print(vx[100:130])
    # print(x[850:900])
    # print(ax[850:900])
    showImg(x_data_list[1], x_data_list[3], x[:], vx[:], t[:], ax[:], None, None, y[:], vy[:],
            ay[:], 5000, 0, render=True, debug=True)
    # showImg(x, vx, t, y, vy, 200, 1)
    # showImg(x, vx, t, y, vy, 200, 2)
