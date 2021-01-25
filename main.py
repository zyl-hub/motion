from utils import *
# from compute import *
from compute import compute_2d


if __name__ == '__main__':
    # x_data_list, y_data_list = loadDataFromTxt(a_max=5, d_max=5, v_max=3, frame_rate=75, dataBase="test_data4.txt") # 500
    # x_data_list, y_data_list = loadDataFromTxt(a_max=5, d_max=5, v_max=3, frame_rate=75, dataBase="test_data5.txt") # 50
    # x_data_list, y_data_list = loadDataFromTxt(a_max=5, d_max=5, v_max=3, frame_rate=75, dataBase="test_data6.txt") #30
    # x_data_list, y_data_list = loadDataFromTxt(a_max=5, d_max=5, v_max=3, frame_rate=75, dataBase="test_data7.txt") #80
    # x_data_list, y_data_list = loadDataFromTxt(a_max=5, d_max=5, v_max=3, frame_rate=75, dataBase="test_data8.txt") #90
    # x_data_list, y_data_list = loadDataFromTxt(a_max=5, d_max=5, v_max=3, frame_rate=75, dataBase="test_data9.txt") #85
    # x_data_list, y_data_list = loadDataFromTxt(a_max=5, d_max=5, v_max=3, frame_rate=75, dataBase="test_data10.txt") #75
    # x_data_list, y_data_list = loadDataFromTxt(a_max=5, d_max=5, v_max=3, frame_rate=75, dataBase="test_data11.txt") #1000
    # x_data_list = x_data_list[30:60]
    # y_data_list = y_data_list[30:60]
    # x_data_list = []
    #
    # for i in range(len(y_data_list)):
    #     x_data_list.append(y_data_list[i][:])
    # print(x_data_list)
    # print(y_data_list)
    x_data_list = [loadData("state11","5")]
    y_data_list = [loadData("state11","5")]
    # y_data_list = loadData("state2", "1")

    # 0:部分手输
    # 1:全部手输
    # x_data_list = loadDataByHand(1)
    # y_data_list = loadDataByHand(1)
    all_x = []
    all_y = []
    all_vx = []
    all_vy = []
    all_ax = []
    all_ay = []
    # y_data_list = [
    #     # [0, 0, 0, 0, 5, 5, 10, 75],
    #     # [0, 1, 0, 0, 5, 5, 10, 75]
    #     []
    # ]
    # x_data_list = [
    #     # [0, 0, 0, 0, 5, 5, 10, 75],
    #     [0, 1, 0, 0, 5, 5, 10, 75]
    # ]
    x, y, vx, vy, ax, ay, t = compute_2d(
        x_data_list, y_data_list, epoch_num=200)
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
    print(len(x_data_list))
    print(all_vy)
    # showImg(x, vx, t, y, vy, 200, 1)
    # showImg(x, vx, t, y, vy, 200, 2)
