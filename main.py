from utils import *
from compute import *


if __name__ == '__main__':
    x_data_list = loadData("v0_v1x", "1")
    y_data_list = loadData("v0_v1x", "1")
    x, y, vx, vy, ax, ay, t = compute_2d(x_data_list, y_data_list)
    showImg(x, vx, t, y, vy, 500, 0)