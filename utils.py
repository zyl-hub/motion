import matplotlib.pyplot as plt
import json
import time


# params:
# showImgDimension : x(0), y(1) or otherwise(2)
showImgDimension = (0, 1, 2)
savePath = "Img/" + \
    time.strftime('%Y-%m-%d-%H-%M-%S', time.localtime(time.time())) + "--"


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

def pltSingle(x, v, t, a=None, epochNum=None, Dimension=0, render=True, debug=False):
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


def showImg(x, vx, t, ax, y=None, vy=None, ay=None, epochNum=None, Dimension=0, render=True, debug=False):
    if Dimension == 0:
        pltSingle(x, vx, t, ax, epochNum, Dimension, render, debug)

    if Dimension == 1:
        pltSingle(y, vy, t, ay, epochNum, Dimension, render, debug)

    if Dimension == 2:
        pltSingle(x, vx, t, ax, epochNum, Dimension, render, debug)
        pltSingle(y, vy, t, ay, epochNum, Dimension, render, debug)

# t = [0, 1, 2, 3, 4, 5]
# x = [1 + i for i in t]
# y = [2 + i for i in t]
# vx = [3 + i for i in t]
# vy = [4 + i for i in t]
# showImg(x, t, vx, y, vy, "all", 2)
