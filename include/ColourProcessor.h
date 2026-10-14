#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <cstdint>

/**
 * @file ColourProcessor.h
 * @brief Class responsible for performing non-destructive image colour transformations.
 * 
 * IMPORTANT ARCHITECTURAL NOTE:
 * Channel ordering conversion (e.g. BGR to RGB) changes internal memory representation 
 * and byte layout for external systems (such as Qt, OpenGL, or Web viewports).
 * It is NOT an actual perceptual colour-space conversion (such as RGB to HSV or CIELAB).
 * 
 * Project: Image Colour Converter (Team 13)
 * Institute: N.M.V.P. Mandal's Institute of Engineering and Technology (NMIET)
 */

class ColourProcessor {
public:
    /**
     * @brief Structure representing detailed pixel information.
     */
    struct PixelData {
        int x{0};
        int y{0};
        int channels{0};
        uint8_t ch0{0};         ///< Blue in BGR, Red in RGB, Gray in 1-channel
        uint8_t ch1{0};         ///< Green in BGR/RGB
        uint8_t ch2{0};         ///< Red in BGR, Blue in RGB
        std::string hexCode;    ///< Hex string representation (#RRGGBB)
        double luminance{0.0};  ///< Perceptual brightness value [0-255]
    };

    /**
     * @brief Default constructor.
     */
    ColourProcessor() = default;

    /**
     * @brief Destructor.
     */
    ~ColourProcessor() = default;

    /**
     * @brief Converts a colour image (BGR) to single-channel Grayscale.
     * 
     * Uses standard ITU-R Recommendation BT.601 luminance weighting:
     * Gray = 0.299 * Red + 0.587 * Green + 0.114 * Blue
     * 
     * @param input Pristine source image (const reference, never modified).
     * @return New grayscale cv::Mat with 1 channel (CV_8UC1).
     */
    static cv::Mat toGrayscale(const cv::Mat& input);

    /**
     * @brief Inverts the colours of the image (negative effect).
     * 
     * Computes pixel_out = 255 - pixel_in for each channel.
     * 
     * @param input Source image (const reference, never modified).
     * @return New cv::Mat with inverted colours.
     */
    static cv::Mat invertColours(const cv::Mat& input);

    /**
     * @brief Modifies individual colour channels by applying independent scaling factors.
     * 
     * Values are clamped to the valid uint8 range [0, 255].
     * 
     * @param input Source image (must be 3-channel).
     * @param blueScale Scaling factor for Blue channel (e.g., 1.0 = unchanged, 0.0 = removed, 1.5 = amplified).
     * @param greenScale Scaling factor for Green channel.
     * @param redScale Scaling factor for Red channel.
     * @return New cv::Mat with adjusted channel intensities.
     */
    static cv::Mat adjustChannels(const cv::Mat& input, double blueScale, double greenScale, double redScale);

    /**
     * @brief Isolates a single channel, setting all other colour channels to 0.
     * 
     * @param input Source 3-channel image.
     * @param channelIndex 0 for Blue, 1 for Green, 2 for Red.
     * @return New cv::Mat with only the selected channel preserved.
     */
    static cv::Mat isolateChannel(const cv::Mat& input, int channelIndex);

    /**
     * @brief Converts between BGR and RGB memory representations.
     * 
     * OpenCV loads and stores images in BGR byte ordering by default.
     * Many external GUI toolkits, OpenGL textures, and web frameworks expect RGB order.
     * This function swaps the 1st (Blue) and 3rd (Red) channels in memory.
     * 
     * Note: This is a representation layout swap, not a perceptual colour-space transform.
     * 
     * @param input Source 3-channel image.
     * @return New cv::Mat with swapped channel representation.
     */
    static cv::Mat convertBgrToRgbRepresentation(const cv::Mat& input);

    /**
     * @brief Inspects pixel values at coordinates (x, y) in any given image.
     * @param input Image to inspect.
     * @param x Column index.
     * @param y Row index.
     * @param isRgbRepresentation Set to true if image is currently in RGB ordering.
     * @return PixelData struct containing channel values, hex code, and luminance.
     */
    static PixelData inspectPixel(const cv::Mat& input, int x, int y, bool isRgbRepresentation = false);
};
