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


def pltSingle(x, v, t, epochNum=None, Dimension=0, render=True):
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
    plt.savefig(savePath + str(Dimension) + ".png")
    if render:
        plt.show()
    return


def showImg(x, vx, t, y=None, vy=None, epochNum=None, Dimension=0, render=True):
    if Dimension == 0:
        pltSingle(x, vx, t, epochNum, Dimension, render)

    if Dimension == 1:
        pltSingle(y, vy, t, epochNum, Dimension, render)

    if Dimension == 2:
        pltSingle(x, vx, t, epochNum, Dimension, render)
        pltSingle(y, vy, t, epochNum, Dimension, render)

# t = [0, 1, 2, 3, 4, 5]
# x = [1 + i for i in t]
# y = [2 + i for i in t]
# vx = [3 + i for i in t]
# vy = [4 + i for i in t]
# showImg(x, t, vx, y, vy, "all", 2)
