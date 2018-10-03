#include <opencv2/highgui.hpp>

void dftshift(cv::Mat& mag);

void compute_DFT(cv::Mat input, cv::Mat& complex_output);

void get_mag_spectrum(cv::Mat complex_input, cv::Mat& mag_output, cv::Mat& phase_output);

void get_viewable_mag_spectrum(cv::Mat mag_input, cv::Mat &mag_output); //Input raw magnitude from DFT operation

void restore_image_from_complex_coeffs(cv::Mat complexI, cv::Mat &output);



