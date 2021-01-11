from utils import *
from compute import *


if __name__ == '__main__':
    x_data_list = loadData("state3", "2")
    # y_data_list = loadData("state2", "1")
    x, y, vx, vy, ax, ay, t = compute_2d(x_data_list, [],epoch_num=200)
    print("epoch:", len(x))
    print("v:", vx[len(vx)-1])
    print("v_m:", max(vx[0:150]))
    print("x:", x[len(x)-1])
    # print(x[60:85])
    # print(vx[60:85])
    # print(x[100:130])
    # print(vx[100:130])
    print(x[850:900])
    print(vx[850:900])
    showImg(x, vx, t, ax, y, vy, ay, 1200, 0, render=True, debug=True)
    # showImg(x, vx, t, y, vy, 200, 1)
    # showImg(x, vx, t, y, vy, 200, 2)
