#pragma once

#include <opencv2/opencv.hpp>
#include <string>

/**
 * @file OutputManager.h
 * @brief Class responsible for exporting and saving processed images to disk.
 * 
 * Project: Image Colour Converter (Team 13)
 * Institute: N.M.V.P. Mandal's Institute of Engineering and Technology (NMIET)
 */

class OutputManager {
private:
    std::string lastMessage;       ///< Description of the most recent save status or error
    bool lastOperationSuccess;     ///< True if the most recent export succeeded

public:
    /**
     * @brief Default constructor.
     */
    OutputManager();

    /**
     * @brief Destructor.
     */
    ~OutputManager() = default;

    /**
     * @brief Validates if the file path has a supported image extension (e.g. .jpg, .jpeg, .png, .bmp).
     * @param filePath Target output path.
     * @return True if extension is valid and supported.
     */
    static bool isValidExtension(const std::string& filePath);

    /**
     * @brief Extracts lowercase file extension from a file path.
     * @param filePath File path string.
     * @return Lowercase extension including dot (e.g., ".png", ".jpg").
     */
    static std::string extractExtension(const std::string& filePath);

    /**
     * @brief Saves an image to disk at the designated destination.
     * 
     * Performs validation on image data, output directory existence/creation, 
     * and file extension support before writing with OpenCV.
     * 
     * @param outputPath Full or relative target file path.
     * @param image Matrix containing the processed image data to write.
     * @return True if image was successfully written to disk; false otherwise.
     */
    bool saveImage(const std::string& outputPath, const cv::Mat& image);

    /**
     * @brief Returns the status/error message from the most recent export operation.
     * @return Human-readable status message string.
     */
    std::string getLastMessage() const;

    /**
     * @brief Checks whether the last save operation was successful.
     * @return True if successful, false otherwise.
     */
    bool wasLastOperationSuccessful() const;
};
