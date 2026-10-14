#include "OutputManager.h"
#include <filesystem>
#include <algorithm>
#include <cctype>

namespace fs = std::filesystem;

OutputManager::OutputManager()
    : lastMessage(""), lastOperationSuccess(false) {
}

std::string OutputManager::extractExtension(const std::string& filePath) {
    fs::path p(filePath);
    std::string ext = p.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return ext;
}

bool OutputManager::isValidExtension(const std::string& filePath) {
    std::string ext = extractExtension(filePath);
    return (ext == ".jpg" || ext == ".jpeg" || ext == ".png" || ext == ".bmp");
}

bool OutputManager::saveImage(const std::string& outputPath, const cv::Mat& image) {
    lastOperationSuccess = false;
    lastMessage.clear();

    if (outputPath.empty()) {
        lastMessage = "Save Error: Output file path is empty.";
        return false;
    }

    if (image.empty()) {
        lastMessage = "Save Error: Provided image is empty; cannot export.";
        return false;
    }

    if (!isValidExtension(outputPath)) {
        lastMessage = "Save Error: Unsupported file format in '" + outputPath + 
                      "'. Supported extensions are: .png, .jpg, .jpeg, .bmp";
        return false;
    }

    try {
        fs::path outPath(outputPath);
        fs::path parentDir = outPath.parent_path();

        // Create destination directory if it does not already exist
        if (!parentDir.empty() && !fs::exists(parentDir)) {
            std::error_code ec;
            if (!fs::create_directories(parentDir, ec)) {
                lastMessage = "Save Error: Failed to create target directory '" + 
                              parentDir.string() + "': " + ec.message();
                return false;
            }
        }

        // Attempt writing the image with OpenCV imwrite
        bool success = cv::imwrite(outputPath, image);

        if (!success) {
            lastMessage = "Save Error: OpenCV failed to write image to '" + outputPath + 
                          "'. Check disk permissions or file path validity.";
            return false;
        }

        lastOperationSuccess = true;
        lastMessage = "Success: Image successfully saved to '" + outputPath + 
                      "' (" + std::to_string(image.cols) + "x" + std::to_string(image.rows) + 
                      ", " + std::to_string(image.channels()) + " channel(s)).";
        return true;

    } catch (const std::exception& ex) {
        lastMessage = "Save Error Exception: " + std::string(ex.what());
        return false;
    }
}

std::string OutputManager::getLastMessage() const {
    return lastMessage;
}

bool OutputManager::wasLastOperationSuccessful() const {
    return lastOperationSuccess;
}
