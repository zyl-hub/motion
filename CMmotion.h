#ifndef CMmotion_H_
#define CMmotion_H_

inline double copysign(double a, double b);

void compute_motion_1d(double x, double v0, double v1, double a_max, double d_max, double v_max, double a_factor, double vel_factor, double& traj_accel, double& traj_time, double& traj_time_acc, double& traj_time_dec, double& traj_time_flat, double frame_rate);

#endif
