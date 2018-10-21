#include "filters_shp.h"
#include "core_shp.h"


void max_filter(cv::Mat input, cv::Mat &output) //Adaptive
{
    output = input.clone(); //Only to make output proper size
    cv::Mat img_border;
    //Pad the
    int border_width = 5;
    int kernelsize = MIN_KERNEL_SIZE; //Start with the smallest possible kernel
    int top = border_width, bottom = border_width, left = border_width, right = border_width;

    cv::copyMakeBorder(input,img_border,top,bottom,left,right, cv::BORDER_REPLICATE);
    int no = 0;
    for(int i = border_width; i < input.rows; i++)
    {
        for(int j = border_width; j < input.cols; j++)
        {
            int max_val = 0;
            //Find max value in kernel:

            for(int k = i - kernelsize/2; k < i + 1 + kernelsize/2; k++)
            {
                for(int l = j - kernelsize/2; l < j + 1 + kernelsize/2; l++)
                {
                    if(img_border.at<uchar>(k,l) > max_val)
                        max_val = img_border.at<uchar>(k,l);
                    no++;
                }
            }
            no = 0;

            if(max_val == BLACK)
            {
                kernelsize += 2;
                if(kernelsize - 2 == MAX_KERNEL_SIZE)
                {
                    output.at<uchar>(i,j) = max_val;    //If the biggest kernel size still achieves black - then accept.
                    kernelsize = MIN_KERNEL_SIZE; //Reset kernel size.
                }
                else if(j == border_width)              //Leading edge scenario
                {
                    j = input.cols - 1;
                    i--;
                }
                else
                    j--;
            }
            else
            {
            output.at<uchar>(i,j) = max_val;
            kernelsize = MIN_KERNEL_SIZE; //Reset kernel size.
            }
        }
    }
}

void median_filter(cv::Mat input, cv::Mat &output, int minkernel) //Adaptive
{
    output = input.clone(); //Only to make output proper size
    cv::Mat img_border;
    //Pad the
    int border_width = 5;
    int kernelsize = minkernel; //Start with the smallest possible kernel
    int top = border_width, bottom = border_width, left = border_width, right = border_width;

    cv::copyMakeBorder(input,img_border,top,bottom,left,right, cv::BORDER_REPLICATE);
    for(int i = border_width; i < input.rows; i++)
    {
        for(int j = border_width; j < input.cols; j++)
        {
            int median_val = 0;
            std::vector<int> list;
            //Find median value in kernel:

            for(int k = i - kernelsize/2; k < i + 1 + kernelsize/2; k++)
            {
                for(int l = j - kernelsize/2; l < j + 1 + kernelsize/2; l++)
                {
                    list.push_back(img_border.at<uchar>(k,l));
                }
            }

            std::sort(list.begin(), list.end());
            median_val = list[list.size()/2];

            if(median_val == BLACK || median_val == WHITE)
            {
                kernelsize += 2;
                if(kernelsize - 2 == MAX_KERNEL_SIZE)
                {
                    output.at<uchar>(i,j) = median_val;    //If the biggest kernel size still achieves black or white - then accept.
                    kernelsize = minkernel; //Reset kernel size.
                }
                else if(j == border_width)              //Leading edge scenario
                {
                    j = input.cols - 1;
                    i--;
                }
                else
                    j--;
            }
            else
            {
            output.at<uchar>(i,j) = median_val;
            kernelsize = minkernel; //Reset kernel size.
            }
        }
    }
}

void notch_filter(cv::Mat mag_input, cv::Mat &mag_output, std::vector<notch> notches)
{
    int M = mag_input.rows;
    int N = mag_input.cols;

    cv::Mat mag_input_copy = mag_input.clone();


    double Dk, Dmk, D0, Hk, Hmk;
    int uk, vk, n;

    std::vector<cv::Mat> H_NR_list;

    for(int i = 0; i < notches.size(); i++)
    {
        D0 = notches[i]._cf;
        uk = notches[i]._u;
        vk = notches[i]._v;
        n  = notches[i]._order;
        cv::Mat H_NR = mag_input.clone(); //To get proper size and type

        for(int u = 0; u < M; u++)
        {

            for(int v = 0; v < N; v++)
            {
                Dk = sqrt(pow(u - M/2 - uk, 2) + pow(v - N/2 - vk, 2));
                Dmk = sqrt(pow(u - M/2 + uk, 2) + pow(v - N/2 + vk, 2));

                Hk = 1. / (1 + pow(D0 / Dk, 2 * n));
                Hmk = 1. / (1 + pow(D0 / Dmk, 2 * n));

                H_NR.at<float>(u,v) = Hk * Hmk;
            }
        }
        H_NR_list.push_back(H_NR);
    }

    //Multiply together all the individual notch filters:
    cv::Mat new_HNR = H_NR_list[0];
    for(int i = 1; i < H_NR_list.size(); i++)
        new_HNR = new_HNR.mul(H_NR_list[i]);


    //Apply filter
    dftshift(mag_input_copy);
    mag_output = new_HNR.mul(mag_input_copy);
    dftshift(mag_output);
}
