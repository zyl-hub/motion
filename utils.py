import matplotlib.pyplot as plt
import json


# params:
# showImgType : x, v or both
# showImgDimension : x(0), y(1) or both(2)
showImgType = ("all", "x", "v")
showImgDimension = (0, 1, 2)


def loadData(dataType, dataOrder, dataBase="test_data.json"):
    dataList = []
    with open(dataBase, 'r', encoding='utf') as fp:
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


# def showImg(x, y=[], vx, vy=[], time, type='all', Dimension=2, DontShow=True):
#     if Dimension == 0:
#         if type == 'all':
#             plt.figure()
#             plt.xlabel('time')
#             plt.plot(time, x)
#             plt.plot(time, vx)
#             plt.show()
#
#     return 0
