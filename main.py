from utils import *
from compute import *


if __name__ == '__main__':
    x_data_list = loadData("v0v1x", "5")
    y_data_list = loadData("v0v1x", "5")
    x, y, vx, vy, ax, ay, t = compute_2d(x_data_list, y_data_list)
    showImg(x, vx, t, ax, y, vy, ay, 200, 0, render=True, debug=True)
    # showImg(x, vx, t, y, vy, 200, 1)
    # showImg(x, vx, t, y, vy, 200, 2)
