#include "CMmotion.h"
#include "compute.h"

int main()
{
    int epoch_num=2000;
    double* x_data_list =new double[8];
    double* y_data_list =new double[8];
    x_data_list[0]=1.0;
    x_data_list[1]=2.4;
    x_data_list[2]=1.0;
    x_data_list[3]=2.0;
    x_data_list[4]=1.0;
    x_data_list[5]=1.0;
    x_data_list[6]=10.0;
    x_data_list[7]=75.0;
    y_data_list[0]=1.0;
    y_data_list[1]=2.4;
    y_data_list[2]=1.0;
    y_data_list[3]=2.0;
    y_data_list[4]=1.0;
    y_data_list[5]=1.0;
    y_data_list[6]=10.0;
    y_data_list[7]=75.0;
    compute_2d(x_data_list, y_data_list, epoch_num);
}
