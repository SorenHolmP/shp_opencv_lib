#ifndef FILTERS_SHP_H
#define FILTERS_SHP_H

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

#define BLACK 0
#define WHITE 255
#define MAX_KERNEL_SIZE 11
#define MIN_KERNEL_SIZE 1

void max_filter(cv::Mat input, cv::Mat &output); //Adaptive
void median_filter(cv::Mat input, cv::Mat &output, int minkernel = MIN_KERNEL_SIZE); //Adaptive

struct notch
{
    notch(double u, double v, double cf, int order): _u(u), _v(v), _cf(cf), _order(order)  {}
    double _u,_v;       //Targeted frequency
    double _cf;         //cutoff frequency distance
    int _order;
};

void notch_filter(cv::Mat mag_input, cv::Mat &mag_output, std::vector<notch> notches);

#endif

