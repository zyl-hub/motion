# 🐭Important

- [ ] use_fast优先要求达到x

- [ ] accurate达到x并且达到速度(暂时不做留出接口)

- [ ] 波动的bug

- [ ] x v0 v1同向 v1 < v0时贪心不够多

- [ ] 改成1/75s后的预测

- [ ] 加速度可以不是a_max和d_max, 加速减速flag

# 杂活

hxy

- [x] 读取情况，实现情况选择功能

- [x] 返回两个len 8的列表(x0, x1, v0, v1, a_max, d_max, v_max, frame_rate)

- [ ] 测试可用性

csy

- [ ] 返回五个列表(x, y, vx, vy, time)

zyl

- [ ] 画图(x-t, y-t, vx-t, vy-t, x-y)

# 思想

$\delta$ a_max to 0与时间的面积

判断指标

period

v阈值

$\alpha$线性拟合系数

v+$\delta$v = $\alpha$v

安全区
