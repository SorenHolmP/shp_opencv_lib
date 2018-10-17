#include <opencv2/highgui.hpp>


#define DO_PAD      1

void dftshift(cv::Mat& mag);

void compute_DFT(cv::Mat input, cv::Mat& complex_output, bool pad = true);

void get_mag_spectrum(cv::Mat complex_input, cv::Mat& mag_output, cv::Mat& phase_output);

void get_viewable_mag_spectrum(cv::Mat mag_input, cv::Mat &mag_output); //Input raw magnitude from DFT operation

void restore_image_from_complex_coeffs(cv::Mat complexI, cv::Mat &output);

void restore_image_from_mag_and_phase(cv::Mat mag, cv::Mat phase, cv::Mat &output);

struct notch
{
    notch(double u, double v, double cf, int order): _u(u), _v(v), _cf(cf), _order(order)  {}
    double _u,_v;       //Targeted frequency
    double _cf;         //cutoff frequency distance
    int _order;
};

void notch_filter(cv::Mat mag_input, cv::Mat &mag_output, std::vector<notch> notches);



