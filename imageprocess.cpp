#include "imageprocess.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>

void rgb::operator+(const int &v)
{
    R += v;
    G += v;
    B += v;
}

void rgb::operator-(const int &v)
{
    R -= v;
    G -= v;
    B -= v;
}

/**
 * Give the average color (RGB) of an image
 *
 * @param file Path to the image
 * @returns RGB values
 */
rgb averageColorImg(std::string file)
{
    cv::Mat image = cv::imread(file);
    cv::Vec3b buf;

    int64 R(0), G(0), B(0);

    for (int i = 0; i < image.rows; i++)
        for (int j = 0; j < image.cols; j++)
        {
            buf = image.at<cv::Vec3b>(i, j);
            B += buf[0];
            G += buf[1];
            R += buf[2];
        }

    B /= (image.cols * image.rows);
    G /= (image.cols * image.rows);
    R /= (image.cols * image.rows);

    rgb val(R, G, B);
    return val;
}

/**
 * Is image containing only grayscale colors
 *
 * @param file Path to the image
 * @returns if it is only gray
 */
bool isImgGray(std::string file)
{
    cv::Mat image = cv::imread(file);
    cv::Vec3b buf;

    for (int i = 0; i < image.rows; i++)
        for (int j = 0; j < image.cols; j++)
        {
            buf = image.at<cv::Vec3b>(i, j);
            rgb col(buf[2], buf[1], buf[0]);
            if (!isColorGray(col))
                return false;
        }

    return true;
}

/**
 * Convert a RGB color into HSV values
 *
 * @param color RGB values
 * @returns HSV values
 */
hsv rgbToHsv(rgb const color)
{
    hsv val;
    float f6(6.);
    float R(color.R / 255.f), G(color.G / 255.f), B(color.B / 255.f);
    //std::cout << R << " " << G << " " << B << std::endl;
    float xmax = std::max(R, std::max(G, B)),
          xmin = std::min(R, std::min(G, B));
    float chroma = xmax - xmin;

    // hue
    if (chroma == 0)
        val.H = 0;
    else if (xmax == R)
        val.H = 60. * std::fmod((G - B) / chroma, 6);
    else if (xmax == G)
        val.H = 60. * ((B - R) / chroma + 2);
    else if (xmax == B)
        val.H = 60. * ((R - G) / chroma + 4);

    //  saturation
    if (xmax == 0)
        val.S = 0;
    else
        val.S = chroma / xmax;

    // value
    val.V = xmax;
    return val;
}

rgb hsvToRgb(hsv const color)
{
    int chroma = color.V * color.S * 255;
    float f8(8.), f2(2.);
    float hue = std::fmod(color.H / 60, 8);
    int interm = chroma * (1 - abs(std::fmod(hue, 2) - 1));

    rgb val(255, 255, 255);
    if (hue <= 1.0)
        val = {chroma, interm, 0};
    else if (hue <= 2.0)
        val = {interm, chroma, 0};
    else if (hue <= 3.0)
        val = {0, chroma, interm};
    else if (hue <= 4.0)
        val = {0, interm, chroma};
    else if (hue <= 5.0)
        val = {interm, 0, chroma};
    else if (hue <= 6.0)
        val = {chroma, 0, interm};
    else if (hue <= 7.0)
        val = {interm, interm, interm};
    val.operator+(255 * color.V);
    val.operator-(chroma);

    return val;
}

/**
 * Is color belonging to a grayscale
 *
 * @param color RGB values
 * @returns belonging to a grayscale
 */
bool isColorGray(rgb const color)
{
    if (color.R == color.G && color.G == color.B)
        return true;
    else
        return false;
}

/**
 * Is color belonging to a grayscale
 *
 * @param color HSV values
 * @returns belonging to a grayscale
 */
bool isColorGray(hsv const color)
{
    return color.S == 0;
}