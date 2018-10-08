#include "histogram_shp.h"


/* Input: Gray scale image
 * Output: Histogram bin values stored in integer array.
 *
 */
void get_histogram_bins(cv::Mat src_gray, int* bins)
{
    for(int i = 0; i < src_gray.rows; i++)
        for(int j = 0; j < src_gray.cols; j++)
        {
            int intensity = src_gray.at<uchar>(i,j);
            bins[intensity]++;
        }
}


/* Input: Gray scale image
 * Output: Image of histogram scaled to dimensions of output
 *
 */
void get_histogram_gray(cv::Mat src_gray, cv::Mat &output)
{
    //Calculate histogram and store in array
    int levels = 256;
    int bins[levels] = {0};
    get_histogram_bins(src_gray, &bins[0]);

    //Find most common intensity: (Used for scaling purposes)
    double xmax = 0;
    for(int i = 0; i < 255; i++)
    {
        if(bins[i] > xmax)
            xmax = bins[i];
    }

    //Draw histogram
    cv::Mat temp(256,256,CV_8U);
    int hist_height = temp.rows, hist_width = levels;
    temp = 255;
    output = 255;  //Set all pixels to white

    for(int j = 0; j < temp.cols; j++)
    {
        int bin_val = (bins[j] / xmax) * hist_height;
        for(int i = hist_height-1; i > (hist_height - bin_val); i--)
        {
            //output.at<uchar>(i,j) = 0;
            temp.at<uchar>(i,j) = 0;
        }
    }

    //Rescale to output size:
    cv::resize(temp, output, output.size(), 0,0, cv::INTER_LINEAR);
}

/* Input: grayscale image
 * Output: Histogram equalized image
 * Note: Dimensions of output must match input
 *
 */
void histogram_equalize(cv::Mat src_gray, cv::Mat &output)
{
    int bins[256] = {0};
    get_histogram_bins(src_gray, &bins[0]);

    //cv::Mat img_equalized(img.rows, img.cols, CV_8U);
    int intensity = 0;
    int n_intensity = 0;
    double num_pixels = src_gray.rows * src_gray.cols;
    double scale = 256 - 1;                 //scale onto range 0-255
    double factor = scale / num_pixels;

    for(int i = 0; i < output.rows; i++)
    {
        for(int j = 0; j < output.cols; j++)
        {
            intensity = src_gray.at<uchar>(i,j);
            double sum = 0;

            for(int k = 0; k < intensity; k++)
            {
                sum = sum + bins[k];
            }
            n_intensity = factor * sum;
            output.at<uchar>(i,j) = n_intensity;

        }
    }
}
