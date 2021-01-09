import matplotlib.pyplot as plt
import json
import time


# params:
# showImgType : x, v or both
# showImgDimension : x(0), y(1) or otherwise(2)
showImgType = ('x', 'v', 'xy', 'all')
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


def pltSingle(x, y, type, epochNum, render=True):
    if epochNum is not None:
        x = x[0:epochNum]
        y = y[0:epochNum]
    plt.figure()
    plt.title(type)
    plt.xlabel(type[-1])
    plt.ylabel(type[0:1])
    plt.plot(x, y)
    plt.savefig(savePath + type + ".png")
    if render:
        plt.show()
    return


def showImg(x, t, vx=None, y=None, vy=None, epochNum=None, type='all', Dimension=0, render=True):
    if Dimension == 0:
        if type == 'x':
            pltSingle(t, x, "xx-t", epochNum, render)
            return
        if type == 'v':
            pltSingle(t, vx, "vx-t", epochNum, render)
            return
        if type == 'all':
            pltSingle(t, x, "xx-t", epochNum, render)
            pltSingle(t, vx, "vx-t", epochNum, render)
            return

    if Dimension == 1:
        if type == 'x':
            pltSingle(t, y, "yy-t", epochNum, render)
            return
        if type == 'v':
            pltSingle(t, vy, "vy-t", epochNum, render)
            return
        if type == 'all':
            pltSingle(t, y, "yy-t", epochNum, render)
            pltSingle(t, vy, "vy-t", epochNum, render)
            return

    if Dimension == 2:
        if type == 'x':
            pltSingle(t, x, "xx-t", epochNum, render)
            pltSingle(t, y, "yy-t", epochNum, render)
            pltSingle(x, y, "yy-x", epochNum, render)
            return
        if type == 'v':
            pltSingle(t, vx, "vx-t", epochNum, render)
            pltSingle(t, vy, "vy-t", epochNum, render)
            return
        if type == 'all':
            pltSingle(t, x, "xx-t", epochNum, render)
            pltSingle(t, y, "yy-t", epochNum, render)
            pltSingle(x, y, "yy-x", epochNum, render)
            pltSingle(t, vx, "vx-t", epochNum, render)
            pltSingle(t, vy, "vy-t", epochNum, render)
            return


# t = [0, 1, 2, 3, 4, 5]
# x = [1 + i for i in t]
# y = [2 + i for i in t]
# vx = [3 + i for i in t]
# vy = [4 + i for i in t]
# showImg(x, t, vx, y, vy, "all", 2)
