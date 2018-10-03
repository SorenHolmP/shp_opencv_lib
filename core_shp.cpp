//03-10-2018

#include <opencv2/highgui.hpp>
#include "core_shp.h"

void dftshift(cv::Mat& mag)
{
    int cx = mag.cols / 2;
    int cy = mag.rows / 2;

    cv::Mat tmp;
    cv::Mat q0(mag, cv::Rect(0, 0, cx, cy));
    cv::Mat q1(mag, cv::Rect(cx, 0, cx, cy));
    cv::Mat q2(mag, cv::Rect(0, cy, cx, cy));
    cv::Mat q3(mag, cv::Rect(cx, cy, cx, cy));

    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

void compute_DFT(cv::Mat input, cv::Mat& complex_output)
{
    cv::Mat padded;
    int opt_rows = cv::getOptimalDFTSize(input.rows * 2 - 1);
    int opt_cols = cv::getOptimalDFTSize(input.cols * 2 - 1);
    cv::copyMakeBorder(input, padded, 0, opt_rows - input.rows, 0, opt_cols - input.rows, cv::BORDER_CONSTANT, cv::Scalar::all(0));
    //cv::copyMakeBorder(input, padded, 0, 0, 0, opt_cols - input.rows, cv::BORDER_CONSTANT, cv::Scalar::all(0));

    cv::Mat planes[] = {
        cv::Mat_<float>(padded),
        cv::Mat_<float>::zeros(padded.size())
    };
    cv::Mat complex;
    cv::merge(planes, 2, complex);

    // Compute DFT
    cv::dft(complex, complex);
    complex_output = complex;
}

void get_mag_spectrum(cv::Mat complex_input, cv::Mat& mag_output, cv::Mat& phase_output)
{
    cv::Mat planes[] = {
        cv::Mat_<float>::zeros(complex_input.size()),
        cv::Mat_<float>::zeros(complex_input.size())
    };

     cv::split(complex_input,planes);
     cv::Mat mag, phase;
     //cv::magnitude(planes[0], planes[1], mag);
     //cv::phase(planes[0], planes[1], phase);
     cv::cartToPolar(planes[0], planes[1], mag, phase);

     mag_output = mag;
     phase_output = phase;
}

void get_viewable_mag_spectrum(cv::Mat mag_input, cv::Mat &mag_output) //Input raw magnitude from DFT operation
{
    cv::Mat mag_input_copy = mag_input;
    dftshift(mag_input_copy);
    mag_input_copy += cv::Scalar::all(1);
    cv::log(mag_input_copy, mag_input_copy);

    // Transform the matrix with float values into a viewable image form (float) values between 0 and 1) and show the result.
    cv::normalize(mag_input_copy, mag_input_copy, 0, 1, cv::NORM_MINMAX);
    mag_output = mag_input_copy;
}

void restore_image_from_complex_coeffs(cv::Mat complexI, cv::Mat &output)
{
    cv::Mat inverse;
    cv::dft(complexI, inverse, cv::DFT_INVERSE | cv::DFT_SCALE | cv::DFT_REAL_OUTPUT);
    normalize(inverse, inverse, 0,1, cv::NORM_MINMAX);
    output = inverse;
}
