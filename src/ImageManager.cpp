#include "ImageManager.h"
#include <filesystem>
#include <sstream>
#include <iomanip>
#include <cmath>

namespace fs = std::filesystem;

ImageManager::ImageManager()
    : loadedFilePath(""), lastError("No image has been loaded yet.") {
}

ImageManager::ImageManager(const std::string& filePath)
    : loadedFilePath(""), lastError("") {
    loadImage(filePath);
}

bool ImageManager::loadImage(const std::string& filePath) {
    lastError.clear();

    if (filePath.empty()) {
        lastError = "Error: Provided file path is empty.";
        return false;
    }

    // Verify file existence on disk using std::filesystem
    std::error_code ec;
    if (!fs::exists(filePath, ec)) {
        lastError = "Error: File does not exist at '" + filePath + "'. " + ec.message();
        return false;
    }

    if (!fs::is_regular_file(filePath, ec)) {
        lastError = "Error: Path '" + filePath + "' is not a regular file.";
        return false;
    }

    // Attempt to load the image in full BGR colour format using OpenCV
    cv::Mat loaded = cv::imread(filePath, cv::IMREAD_COLOR);

    if (loaded.empty()) {
        lastError = "Error: OpenCV failed to decode image at '" + filePath + 
                    "'. The file may be corrupt or in an unsupported format.";
        return false;
    }

    if (loaded.cols <= 0 || loaded.rows <= 0) {
        lastError = "Error: Image has invalid dimensions (" + 
                    std::to_string(loaded.cols) + "x" + std::to_string(loaded.rows) + ").";
        return false;
    }

    originalImage = loaded;
    loadedFilePath = filePath;
    lastError = "";
    return true;
}

bool ImageManager::isLoaded() const {
    return !originalImage.empty();
}

const cv::Mat& ImageManager::getOriginalImage() const {
    return originalImage;
}

cv::Mat ImageManager::getOriginalImageCopy() const {
    return originalImage.clone();
}

int ImageManager::getWidth() const {
    return originalImage.empty() ? 0 : originalImage.cols;
}

int ImageManager::getHeight() const {
    return originalImage.empty() ? 0 : originalImage.rows;
}

int ImageManager::getChannels() const {
    return originalImage.empty() ? 0 : originalImage.channels();
}

std::string ImageManager::getDimensionsString() const {
    if (originalImage.empty()) {
        return "0 x 0 (No image loaded)";
    }
    return std::to_string(originalImage.cols) + " x " + std::to_string(originalImage.rows);
}

std::string ImageManager::getLoadedFilePath() const {
    return loadedFilePath;
}

std::string ImageManager::getLastError() const {
    return lastError;
}

bool ImageManager::getPixelBGR(int x, int y, uint8_t& b, uint8_t& g, uint8_t& r) const {
    if (originalImage.empty()) {
        return false;
    }

    if (x < 0 || x >= originalImage.cols || y < 0 || y >= originalImage.rows) {
        return false;
    }

    if (originalImage.channels() == 3) {
        // OpenCV stores colour images as BGR (Vec3b: [0]=Blue, [1]=Green, [2]=Red)
        const cv::Vec3b& pixel = originalImage.at<cv::Vec3b>(y, x);
        b = pixel[0];
        g = pixel[1];
        r = pixel[2];
        return true;
    } else if (originalImage.channels() == 1) {
        uint8_t val = originalImage.at<uint8_t>(y, x);
        b = val;
        g = val;
        r = val;
        return true;
    }

    return false;
}

std::string ImageManager::getPixelInfoString(int x, int y) const {
    uint8_t b = 0, g = 0, r = 0;
    if (!getPixelBGR(x, y, b, g, r)) {
        return "Coordinates (" + std::to_string(x) + ", " + std::to_string(y) + ") out of image bounds.";
    }

    // Calculate perceptual luminance using standard weights (Y = 0.299*R + 0.587*G + 0.114*B)
    double luminance = 0.299 * r + 0.587 * g + 0.114 * b;

    std::ostringstream oss;
    oss << "Pixel at (" << x << ", " << y << "): "
        << "B=" << static_cast<int>(b) << ", "
        << "G=" << static_cast<int>(g) << ", "
        << "R=" << static_cast<int>(r) << " | "
        << "HEX: #"
        << std::uppercase << std::hex << std::setfill('0')
        << std::setw(2) << static_cast<int>(r)
        << std::setw(2) << static_cast<int>(g)
        << std::setw(2) << static_cast<int>(b)
        << std::dec << " | Luminance: "
        << std::fixed << std::setprecision(1) << luminance;

    return oss.str();
}
