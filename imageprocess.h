#ifndef IMAGEPROCESS_H_INCLUDED
#define IMAGEPROCESS_H_INCLUDED

#include <string>
struct rgb
{
    int R;
    int G;
    int B;
    rgb() : R(0), G(0), B(0)
    {
    }
    rgb(int red, int green, int blue) : R(red), G(green), B(blue)
    {
    }
    void operator+(const int &v);
    void operator-(const int &v);
    void operator/(const int &v);
    void operator*(const int &v);

    /*friend rgb operator+(rgb v, const rgb &n);
    friend rgb operator-(rgb v, const rgb &n);*/

    rgb& operator+=(const rgb& v);
    rgb& operator/=(const int& v);
};

struct hsv
{
    float H;
    float S;
    float V;
    hsv() : H(0), S(0), V(0)
    {
    }
    hsv(float hue, float saturation, float value) : H(hue), S(saturation), V(value)
    {
    }
};

/**
 * Give the average color (RGB) of an image
 *
 * @param file Path to the image
 * @returns RGB values
 */
rgb averageColorImg(std::string file);

/**
 * Is image containing only grayscale colors
 *
 * @param file Path to the image
 * @returns if it is only gray
 */
bool isImgGray(std::string file);

/**
 * Convert a RGB color into HSV values
 *
 * @param color RGB values
 * @returns HSV values
 */
hsv rgbToHsv(rgb const color);

/**
 * Convert a HSV color into RGB values
 *
 * @param color HSV values
 * @returns RGB values
 */
rgb hsvToRgb(hsv const color);

/**
 * Is color belonging to a grayscale
 *
 * @param color RGB values
 * @returns belonging to a grayscale
 */
bool isColorGray(rgb const color);

/**
 * Is color belonging to a grayscale
 *
 * @param color HSV values
 * @returns belonging to a grayscale
 */
bool isColorGray(hsv const color);

/**
 * Splits a long value to rgb values.
 *
 * @param combinedColors long
 * @returns rgb values
 */
rgb splitColors(long const &combinedColors);

/**
 * Combines rgb values to a long value
 *
 * @param splitColors rgb values
 * @returns long value
 */
long combineColors(rgb const &splitColors);

/**
 * Combines rgb values to a long value
 *
 * @param red value
 * @param green value
 * @param blue value
 * @returns long value
 */
long combineColors(int const &red, int const &green, int const &blue);

#endif // IMAGEPROCESS_H_INCLUDED