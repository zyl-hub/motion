from utils import *
from compute import *


if __name__ == '__main__':
    x_data_list = loadData("acc_all_time", "1")
    y_data_list = loadData("acc_all_time", "1")
    x, y, vx, vy, t = compute_2d(x_data_list, y_data_list)
    showImg(x, t, vx, y, vy, "all", 2)
