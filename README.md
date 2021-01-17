# 🐭Important

- [ ] use_fast优先要求达到x

- [ ] accurate达到x并且达到速度(暂时不做留出接口)

- [ ] 波动的bug

- [ ] x v0 v1同向 v1 < v0时贪心不够多

- [ ] 改成1/75s后的预测

- [ ] 加速度可以不是a_max和d_max, 加速减速flag

- [ ] 把重要的图片放在ImportantImg中

- [ ] 使用第一次compute_1d算出的时间画一条竖线就可以检测时间计算是否正确

- [ ] 在测试时发现会出现v_m的精确度问题，会导致20%的误差，state6

# 进度

- [x] state1

- [x] state2

- [x] state3

- [x] state4

- [x] state5

- [x] state6

- [x] state7

- [x] state8

- [x] state9

- [x] state10

- [x] state9和state10需要多加亿点点情况

- [x] 移植到C++ok，CMmotion是最终要上的代码，final和final_2d是测试程序

# 杂活

hxy

- [x] 读取情况，实现情况选择功能

- [x] 返回两个len 8的列表(x0, x1, v0, v1, a_max, d_max, v_max, frame_rate)

- [x] 测试可用性

csy

- [x] 返回五个列表(x, y, vx, vy, time)

a_or_d = 1是加速0是减速

zyl

- [x] 画图(x-t, y-t, vx-t, vy-t, x-y)

- [x] 完成了显示目标值横线

# 思想

![](http://latex.codecogs.com/svg.latex?\Delta{v})是a_max to 0与时间的面积

一些判断用的指标：

period

v阈值

![](http://latex.codecogs.com/svg.latex?\alpha)线性拟合系数用来表示在不同的速度情况下因为电机影响的速度值

![](http://latex.codecogs.com/svg.latex?v+\Delta{v}=\alpha{v})

安全区
