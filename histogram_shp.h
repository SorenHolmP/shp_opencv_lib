#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

void get_histogram_bins(cv::Mat src_gray, int* bins);  //Calculates number of pixel instances in gray scale image.
void get_histogram_gray(cv::Mat src_gray, cv::Mat &output);   //Forward declaration -- see bottom for definition.
void histogram_equalize(cv::Mat src_gray, cv::Mat &output);
