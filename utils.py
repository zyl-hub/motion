import matplotlib.pyplot as plt
import matplotlib.animation as animation
import json
import time


# params:
# showImgDimension : x(0), y(1) or otherwise(2)
showImgDimension = (0, 1, 2)
savePath = "Img/" + \
    time.strftime('%Y-%m-%d-%H-%M-%S', time.localtime(time.time())) + "--"


# params:
# loadDataFromTxt : as you can see.
def loadDataFromTxt(a_max=1, d_max=1, v_max=10, frame_rate=75, dataBase="test_data.txt"):
    dataList_X = []
    dataList_Y = []
    raw_data = [[], [], [], []]
    with open(dataBase, 'r', encoding='utf-8') as fp:
        for i, line in enumerate(fp):
            raw_data[i] = line.split('\t')
        fp.close()
    for i in range(len(raw_data[0])-2):
        dataList_X.append([eval(raw_data[0][i]), eval(raw_data[0][i+1])])
        dataList_X[-1] += [eval(raw_data[2][i]), eval(raw_data[2][i+1])]
        dataList_X[-1] += [a_max, d_max, v_max, frame_rate]
    for i in range(len(raw_data[1])-2):
        dataList_Y.append([eval(raw_data[1][i]), eval(raw_data[1][i+1])])
        dataList_Y[-1] += [eval(raw_data[3][i]), eval(raw_data[3][i+1])]
        dataList_Y[-1] += [a_max, d_max, v_max, frame_rate]
    return dataList_X, dataList_Y


def loadData(dataType, dataOrder, dataBase="test_data.json"):
    dataList = []
    with open(dataBase, 'r', encoding='utf') as fp:
        dataDict = json.load(fp)
        FinalDataDict = dataDict[dataType][dataOrder]
        dataList.append(eval(FinalDataDict["x0"]))
        dataList.append(eval(FinalDataDict["x1"]))
        dataList.append(eval(FinalDataDict["v0"]))
        dataList.append(eval(FinalDataDict["v1"]))
        dataList.append(eval(FinalDataDict["a_max"]))
        dataList.append(eval(FinalDataDict["d_max"]))
        dataList.append(eval(FinalDataDict["v_max"]))
        dataList.append(eval(FinalDataDict["frame_rate"]))
        fp.close()
    return dataList


def loadDataByHand(all_data):
    default_a_max = 5
    default_d_max = 4
    default_v_max = 10
    default_frame_rate = 75
    dataList = []
    dataList.append(eval(input("x0:")))
    dataList.append(eval(input("x1:")))
    dataList.append(eval(input("v0:")))
    dataList.append(eval(input("v0:")))
    if all_data:
        dataList.append(eval(input("a_max:")))
        dataList.append(eval(input("d_max:")))
        dataList.append(eval(input("v_max:")))
        dataList.append(eval(input("frame_rate:")))
    else:
        dataList.append(default_a_max)
        dataList.append(default_d_max)
        dataList.append(default_v_max)
        dataList.append(default_frame_rate)
    return dataList


def loadDataByPath(x, y, v_x, v_y, a_max=1, d_max=1, v_max=10, frame_rate=75):
    dataList_X = []
    dataList_Y = []
    for i in range(len(x)-1):
        dataList_X.append([x[i], x[i+1], v_x[i], v_x[i+1]])
        dataList_X[-1] += [a_max, d_max, v_max, frame_rate]
        dataList_Y.append([y[i], y[i + 1], v_y[i], v_y[i + 1]])
        dataList_Y[-1] += [a_max, d_max, v_max, frame_rate]
    return dataList_X, dataList_Y


def pltSingle(x_end, v_end, x, v, t, a=None, epochNum=None, Dimension=0, render=True, debug=False):
    if not debug:
        if epochNum is not None:
            x = x[0:epochNum]
            v = v[0:epochNum]
            t = t[0:epochNum]

        fig = plt.figure()

        ax1 = fig.add_subplot(111)
        ax1.plot(t, v, 'r', label="v")
        ax1.legend(loc=1)

        ax2 = ax1.twinx()
        ax2.plot(t, x, 'g', label="x")
        ax2.legend(loc=2)

        # ax4 = ax1.twiny()
        # v_endlist = []
        # for i in range(len(t)):
        #     v_endlist.append(v_end)
        # ax4.plot(t, v_endlist, 'r')
        #
        # ax5 = ax2.twiny()
        # x_endlist = []
        # for i in range(len(t)):
        #     x_endlist.append(x_end)
        # ax5.plot(t, x_endlist, 'g')

        ax1.set_title("Dimension = " + str(Dimension))
        ax1.set_xlabel('Time/(epoch)')
        ax2.set_ylabel('x/(m)')
        ax1.set_ylabel('v/(m/s)')
        # plt.savefig(savePath + str(Dimension) + ".png")
        if render:
            plt.show()
        return
    else:
        if epochNum is not None:
            x = x[0:epochNum]
            v = v[0:epochNum]
            a = a[0:epochNum]
            t = t[0:epochNum]

        fig = plt.figure()

        ax1 = fig.add_subplot(211)
        ax1.plot(t, v, 'r', label="v")
        ax1.legend(loc=1)

        ax2 = ax1.twinx()
        ax2.plot(t, x, 'g', label="x")
        ax2.legend(loc=2)

        # ax4 = ax1.twiny()
        # v_endlist = []
        # for i in range(len(t)):
        #     v_endlist.append(v_end)
        # ax4.plot(t, v_endlist, 'b', label="v_target")
        # ax4.legend(loc=4)

        # ax5 = ax2.twiny()
        # x_endlist = []
        # for i in range(len(t)):
        #     x_endlist.append(x_end)
        # ax5.plot(t, x_endlist, 'y', label="x_target")
        # ax5.legend(loc=3)

        ax1.set_title("Dimension = " + str(Dimension))
        ax2.set_ylabel('x/(m)')
        ax1.set_ylabel('v/(m/s)')

        ax3 = fig.add_subplot(212)
        ax3.plot(t, a, 'r', label="a")
        ax3.legend(loc=1)

        ax3.set_xlabel('Time/(epoch)')
        ax3.set_ylabel('a/(m/s2)')

        # plt.savefig(savePath + str(Dimension) + ".png")
        if render:
            plt.show()
        return


def pltPath(x, y, epochNum=None, render=True):
    if epochNum is not None:
        x = x[0:epochNum]
        y = y[0:epochNum]

    fig = plt.figure()

    ax1 = fig.add_subplot(111)
    ax1.plot(x, y, 'r')
    ax1.set_title("X-Y/(m)")
    if render:
        plt.show()
    return


def pltDynamicPath(x, y, epochNum=None):
    if epochNum is not None:
        x = x[0:epochNum]
        y = y[0:epochNum]

    fig, ax = plt.subplots()
    line = ax.plot(x, y)
    dot, = ax.plot([], [], 'ro')

    def init():
        ax.set_xlim(min(x), max(x))
        ax.set_ylim(min(y), max(y))
        return line

    def gen_dot():
        for i in range(len(x)):
            newdot = [x[i], y[i]]
            yield newdot

    def update_dot(newdot):
        dot.set_data(newdot[0], newdot[1])
        return dot,

    ani = animation.FuncAnimation(fig, update_dot, frames=gen_dot, interval=100, init_func=init)
    ani.save("Gif/dynamicImg.gif", writer='pillow', fps=1)
    plt.show()


def showImg(x_end, v_end, x, vx, t, ax, y_end=None, vy_end=None, y=None, vy=None, ay=None, epochNum=None, Dimension=0, render=True, debug=False):
    x_len = len(x)
    y_len = len(y)
    if(abs(x_len - y_len)) > 0.01 * x_len:
        return

    min_len=min(x_len,y_len)
    x = x[0:min_len]
    y = y[0:min_len]
    vx = vx[0:min_len]
    vy = vy[0:min_len]
    ax = ax[0:min_len]
    ay = ay[0:min_len]
    t = range(min_len)

    if Dimension == 0:
        pltSingle(x_end, v_end, x, vx, t, ax,
                  epochNum, Dimension, render, debug)

    if Dimension == 1:
        pltSingle(y_end, vy_end, y, vy, t, ay,
                  epochNum, Dimension, render, debug)

    if Dimension == 2:
        pltSingle(x_end, v_end, x, vx, t, ax,
                  epochNum, Dimension, render, debug)
        pltSingle(y_end, vy_end, y, vy, t, ay,
                  epochNum, Dimension, render, debug)
        pltPath(x, y, epochNum, render)


def showImgFromTxt(target_x="data/x.txt", target_v="data/v_x.txt", target_a="data/a_x.txt"):
    with open(target_x, 'r', encoding='utf-8') as fp:
        x = fp.read().split(' ')[0:-1]
        for i in range(len(x)):
            x[i] = eval(x[i])
        fp.close()
    with open(target_v, 'r', encoding='utf-8') as fp:
        v = fp.read().split(' ')[0:-1]
        for i in range(len(v)):
            v[i] = eval(v[i])
        fp.close()
    with open(target_a, 'r', encoding='utf-8') as fp:
        a = fp.read().split(' ')[0:-1]
        for i in range(len(a)):
            a[i] = eval(a[i])
        fp.close()
    t = range(len(x))
    pltSingle(0, 0, x, v, t, a)


# x = [1, 2, 3, 4, 5]
# y = [2, 4, 6, 8, 10]
# v_x = [1+i for i in x]
# v_y = [10 + i for i in x]
# X,Y = loadDataByPath(x, y, v_x, v_y)
# print(X)
# print(Y)
# X = showImgFromTxt()

# X = [-1, 2, 3, 4, 2]
# Y = [1, 6, 8, 2, 4]
# pltDynamicPath(X, Y)
