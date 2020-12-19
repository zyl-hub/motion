import matplotlib.pyplot as plt
import json


def loadData(dataType, dataOrder):
    dataList = []
    with open('test_data.json', 'r', encoding='utf') as fp:
        dataDict = json.load(fp)
        FinalDataDict = dataDict[dataType][dataOrder]
        dataList.append(FinalDataDict["x0"])
        dataList.append(FinalDataDict["x1"])
        dataList.append(FinalDataDict["v0"])
        dataList.append(FinalDataDict["v1"])
        dataList.append(FinalDataDict["a_max"])
        dataList.append(FinalDataDict["d_max"])
        dataList.append(FinalDataDict["v_max"])
        dataList.append(FinalDataDict["frame_rate"])
    return dataList


def showImg(Dimension=1, x, y=[], vx, vy=[], time, type='all', DontShow=False):
    if Dimension == 1:
        if type == 'all':
            plt.figure()
            plt.xlabel('time')
            plt.plot(time, x)
            plt.plot(time, vx)
            plt.show()

    return 0
