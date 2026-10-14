#pragma once

#include "ImageManager.h"
#include "ColourProcessor.h"
#include "OutputManager.h"
#include <opencv2/opencv.hpp>
#include <string>

/**
 * @file ImageConverterApp.h
 * @brief Main coordinator application class for Image Colour Converter.
 * 
 * Project: Image Colour Converter (Team 13)
 * Team Members:
 *   1. PARIHAR SHARDUL RAJENDRA — SEITB243
 *   2. WANKHADE ISHAN VIJAY — SEITB266
 * Institute: N.M.V.P. Mandal's Institute of Engineering and Technology (NMIET)
 * Department: Department of Information Technology
 */

class ImageConverterApp {
public:
    enum class ProcessingMode {
        Original,
        Grayscale,
        Inverted,
        RgbRepresentation,
        ChannelIsolated,
        ChannelAdjusted
    };

private:
    ImageManager imageManager;
    OutputManager outputManager;

    cv::Mat currentProcessedImage;
    ProcessingMode currentMode{ProcessingMode::Original};
    bool isRgbRepresentation{false};

    std::string currentStatusMessage;
    int inspectedX{-1};
    int inspectedY{-1};
    ColourProcessor::PixelData inspectedPixel;

    // Active channel scaling factors
    double blueScale{1.0};
    double greenScale{1.0};
    double redScale{1.0};

    // GUI window identifiers
    const std::string WINDOW_ORIGINAL = "Image Colour Converter - Original";
    const std::string WINDOW_PROCESSED = "Image Colour Converter - Converted";
    const std::string WINDOW_DASHBOARD = "Image Colour Converter - Dashboard & Controls";

public:
    /**
     * @brief Constructor.
     */
    ImageConverterApp();

    /**
     * @brief Destructor.
     */
    ~ImageConverterApp() = default;

    /**
     * @brief Loads an image from a file path.
     * @param filePath Path to the image file.
     * @return True if load succeeded.
     */
    bool loadImage(const std::string& filePath);

    /**
     * @brief Converts the current image to Grayscale.
     */
    void applyGrayscale();

    /**
     * @brief Inverts the colours of the current image.
     */
    void applyInvert();

    /**
     * @brief Toggles between OpenCV's BGR and standard RGB memory representations.
     */
    void applyBgrRgbToggle();

    /**
     * @brief Isolates a single colour channel (0=Blue, 1=Green, 2=Red).
     * @param channelIndex Index of channel to isolate.
     */
    void applyChannelIsolation(int channelIndex);

    /**
     * @brief Modifies individual colour channels by custom scaling factors.
     * @param b Blue channel scaling.
     * @param g Green channel scaling.
     * @param r Red channel scaling.
     */
    void applyChannelAdjustment(double b, double g, double r);

    /**
     * @brief Resets all processing and restores the pristine original image.
     */
    void resetToOriginal();

    /**
     * @brief Saves the current converted image to disk.
     * @param outputPath Path to output file (.jpg, .jpeg, .png, .bmp).
     * @return True if save succeeded.
     */
    bool saveConvertedImage(const std::string& outputPath);

    /**
     * @brief Inspects pixel values at coordinates (x, y) on the converted image.
     * @param x Column index.
     * @param y Row index.
     */
    void inspectPixelAt(int x, int y);

    /**
     * @brief Runs the interactive desktop application with GUI windows and keyboard shortcuts.
     * @param initialImagePath Optional path to an image to load at startup.
     */
    void runGui(const std::string& initialImagePath = "");

    /**
     * @brief Non-interactive automated test suite verifying all 14 project requirements.
     * @param testImagePath Path to test input image.
     * @param outputDirectory Directory to write test outputs.
     * @return 0 on full success, non-zero if any test fails.
     */
    int runSelfTest(const std::string& testImagePath, const std::string& outputDirectory);

    // Getters for status and state
    const cv::Mat& getCurrentProcessedImage() const;
    const ImageManager& getImageManager() const;
    const OutputManager& getOutputManager() const;
    ProcessingMode getCurrentMode() const;
    std::string getModeString() const;
    std::string getStatusMessage() const;
    bool getIsRgbRepresentation() const;
    ColourProcessor::PixelData getInspectedPixel() const;

private:
    /**
     * @brief Redraws the GUI preview windows and status overlays.
     */
    void updatePreviews();

    /**
     * @brief Renders an informational HUD banner onto an image matrix for display.
     */
    cv::Mat renderDisplayWithHud(const cv::Mat& image, const std::string& title, bool isOriginal);

    /**
     * @brief Mouse event handler callback for pixel inspection.
     */
    static void onMouseCallback(int event, int x, int y, int flags, void* userdata);

    /**
     * @brief Trackbar callback for Blue channel.
     */
    static void onBlueTrackbar(int val, void* userdata);

    /**
     * @brief Trackbar callback for Green channel.
     */
    static void onGreenTrackbar(int val, void* userdata);

    /**
     * @brief Trackbar callback for Red channel.
     */
    static void onRedTrackbar(int val, void* userdata);
};
