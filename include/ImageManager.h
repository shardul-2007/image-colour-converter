#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include <cstdint>

/**
 * @file ImageManager.h
 * @brief Class responsible for loading, storing, and inspecting input images.
 * 
 * Project: Image Colour Converter (Team 13)
 * Institute: N.M.V.P. Mandal's Institute of Engineering and Technology (NMIET)
 * Department: Department of Information Technology
 */

class ImageManager {
private:
    cv::Mat originalImage;       ///< Pristine original image in OpenCV's default BGR format
    std::string loadedFilePath;  ///< File path of the currently loaded image
    std::string lastError;       ///< Description of the most recent error, if any

public:
    /**
     * @brief Default constructor initializing an empty ImageManager.
     */
    ImageManager();

    /**
     * @brief Parameterized constructor that immediately attempts to load an image.
     * @param filePath Path to the image file (JPG, JPEG, PNG, BMP).
     */
    explicit ImageManager(const std::string& filePath);

    /**
     * @brief Destructor.
     */
    ~ImageManager() = default;

    /**
     * @brief Loads an image from disk into memory.
     * @param filePath Path to the image file.
     * @return True if loading succeeded and image is valid, false otherwise.
     */
    bool loadImage(const std::string& filePath);

    /**
     * @brief Checks whether a valid image is currently loaded.
     * @return True if loaded image is non-empty, false otherwise.
     */
    bool isLoaded() const;

    /**
     * @brief Returns a const reference to the original image (safe, zero-copy access).
     * @return Const reference to original cv::Mat.
     */
    const cv::Mat& getOriginalImage() const;

    /**
     * @brief Returns a deep copy (clone) of the original image.
     * @return Cloned cv::Mat copy.
     */
    cv::Mat getOriginalImageCopy() const;

    /**
     * @brief Gets the width (number of columns) of the loaded image in pixels.
     * @return Image width in pixels, or 0 if no image is loaded.
     */
    int getWidth() const;

    /**
     * @brief Gets the height (number of rows) of the loaded image in pixels.
     * @return Image height in pixels, or 0 if no image is loaded.
     */
    int getHeight() const;

    /**
     * @brief Gets the number of colour channels in the loaded image (typically 3 for BGR).
     * @return Channel count, or 0 if no image is loaded.
     */
    int getChannels() const;

    /**
     * @brief Returns human-readable dimensions (e.g. "1920 x 1080").
     * @return Formatted dimension string.
     */
    std::string getDimensionsString() const;

    /**
     * @brief Gets the file path of the currently loaded image.
     * @return File path string.
     */
    std::string getLoadedFilePath() const;

    /**
     * @brief Gets the last error message recorded by the manager.
     * @return Error message string.
     */
    std::string getLastError() const;

    /**
     * @brief Safely reads the BGR pixel values at a specific (x, y) coordinate.
     * @param x Horizontal column index (0 <= x < width).
     * @param y Vertical row index (0 <= y < height).
     * @param[out] b Blue channel value [0-255].
     * @param[out] g Green channel value [0-255].
     * @param[out] r Red channel value [0-255].
     * @return True if coordinates are valid and pixel was read; false otherwise.
     */
    bool getPixelBGR(int x, int y, uint8_t& b, uint8_t& g, uint8_t& r) const;

    /**
     * @brief Generates a formatted text summary of pixel information at (x, y).
     * @param x Column index.
     * @param y Row index.
     * @return String describing coordinates, channel values, hex code, and luminance.
     */
    std::string getPixelInfoString(int x, int y) const;
};
