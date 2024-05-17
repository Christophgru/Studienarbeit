/**
 * @file painthelper.h
 * @brief Header file for the paint helper functions.
 * 
 * This file contains the declarations of functions and structures for generating
 * colored images based on distance calculations.
 * 
 * @author Christoph Gruender
 * @date 2024-05-17
 */

#include "calc.h"
#include <iostream>
#include <fstream>
#define DEBUGLEVEL 0
/**
 * @struct Pixel
 * @brief Structure to represent a pixel with RGB values.
 */
struct Pixel {
    unsigned char b; ///< Blue component
    unsigned char g; ///< Green component
    unsigned char r; ///< Red component
};

/**
 * @brief Gets the color corresponding to a value.
 * 
 * @param value The value to map to a color.
 * @param maxValue The maximum value for mapping.
 * @return The color corresponding to the value.
 */
Pixel getColorFromValue(double value, int maxValue = 100);

/**
 * @brief Generates an image based on distance calculations.
 * 
 * @param lines Vector of lines for distance calculations.
 * @param width The width of the image.
 * @param height The height of the image.
 * @param zoomfactor The zoom factor for scaling coordinates.
 * @param maxDistColorGrad The maximum distance for color grading.
 */
void paintpicture(std::vector<calc::line> lines, int width, int height, double zoomfactor = 10, int maxDistColorGrad = 10);
