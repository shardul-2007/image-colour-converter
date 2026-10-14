#include "ColourProcessor.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

cv::Mat ColourProcessor::toGrayscale(const cv::Mat& input) {
    if (input.empty()) {
        return cv::Mat();
    }

    // If already 1-channel grayscale, return a safe cloned copy
    if (input.channels() == 1) {
        return input.clone();
    }

    cv::Mat grayOutput;
    // cv::cvtColor with COLOR_BGR2GRAY uses:
    // Y = 0.299 * R + 0.587 * G + 0.114 * B
    cv::cvtColor(input, grayOutput, cv::COLOR_BGR2GRAY);
    return grayOutput;
}

cv::Mat ColourProcessor::invertColours(const cv::Mat& input) {
    if (input.empty()) {
        return cv::Mat();
    }

    cv::Mat invertedOutput;
    // bitwise_not computes out[i] = 255 - in[i] for all channels uniformly
    cv::bitwise_not(input, invertedOutput);
    return invertedOutput;
}

cv::Mat ColourProcessor::adjustChannels(const cv::Mat& input, double blueScale, double greenScale, double redScale) {
    if (input.empty()) {
        return cv::Mat();
    }

    // Ensure image has 3 colour channels
    if (input.channels() != 3) {
        // If grayscale, convert to BGR first so channels can be manipulated
        cv::Mat color;
        cv::cvtColor(input, color, cv::COLOR_GRAY2BGR);
        return adjustChannels(color, blueScale, greenScale, redScale);
    }

    // Split image into individual B, G, R channels
    std::vector<cv::Mat> channels(3);
    cv::split(input, channels);

    // Apply scale factors with automatic saturation clamping to [0, 255]
    channels[0].convertTo(channels[0], CV_8UC1, std::max(0.0, blueScale));
    channels[1].convertTo(channels[1], CV_8UC1, std::max(0.0, greenScale));
    channels[2].convertTo(channels[2], CV_8UC1, std::max(0.0, redScale));

    cv::Mat adjustedOutput;
    cv::merge(channels, adjustedOutput);
    return adjustedOutput;
}

cv::Mat ColourProcessor::isolateChannel(const cv::Mat& input, int channelIndex) {
    if (input.empty()) {
        return cv::Mat();
    }

    cv::Mat source = input;
    if (input.channels() == 1) {
        cv::cvtColor(input, source, cv::COLOR_GRAY2BGR);
    } else if (input.channels() != 3) {
        return input.clone();
    }

    // Clamp channel index to [0, 2]: 0=Blue, 1=Green, 2=Red
    channelIndex = std::clamp(channelIndex, 0, 2);

    std::vector<cv::Mat> channels(3);
    cv::split(source, channels);

    // Zero out the other two channels
    for (int i = 0; i < 3; ++i) {
        if (i != channelIndex) {
            channels[i] = cv::Mat::zeros(source.size(), CV_8UC1);
        }
    }

    cv::Mat isolatedOutput;
    cv::merge(channels, isolatedOutput);
    return isolatedOutput;
}

cv::Mat ColourProcessor::convertBgrToRgbRepresentation(const cv::Mat& input) {
    if (input.empty()) {
        return cv::Mat();
    }

    if (input.channels() != 3) {
        // Non 3-channel images (e.g. grayscale) do not have BGR channel ordering
        return input.clone();
    }

    cv::Mat rgbOutput;
    // Swaps Channel 0 (Blue) with Channel 2 (Red).
    // Note: This changes memory byte ordering from B-G-R to R-G-B.
    cv::cvtColor(input, rgbOutput, cv::COLOR_BGR2RGB);
    return rgbOutput;
}

ColourProcessor::PixelData ColourProcessor::inspectPixel(const cv::Mat& input, int x, int y, bool isRgbRepresentation) {
    PixelData data;
    data.x = x;
    data.y = y;

    if (input.empty() || x < 0 || x >= input.cols || y < 0 || y >= input.rows) {
        return data;
    }

    data.channels = input.channels();

    uint8_t rVal = 0, gVal = 0, bVal = 0;

    if (data.channels == 3) {
        const cv::Vec3b& pixel = input.at<cv::Vec3b>(y, x);
        data.ch0 = pixel[0];
        data.ch1 = pixel[1];
        data.ch2 = pixel[2];

        if (isRgbRepresentation) {
            // Memory layout is RGB: ch0=Red, ch1=Green, ch2=Blue
            rVal = data.ch0;
            gVal = data.ch1;
            bVal = data.ch2;
        } else {
            // Default OpenCV layout is BGR: ch0=Blue, ch1=Green, ch2=Red
            bVal = data.ch0;
            gVal = data.ch1;
            rVal = data.ch2;
        }
    } else if (data.channels == 1) {
        uint8_t gray = input.at<uint8_t>(y, x);
        data.ch0 = gray;
        data.ch1 = gray;
        data.ch2 = gray;
        rVal = gray;
        gVal = gray;
        bVal = gray;
    }

    // Hex code representation
    std::ostringstream oss;
    oss << "#"
        << std::uppercase << std::hex << std::setfill('0')
        << std::setw(2) << static_cast<int>(rVal)
        << std::setw(2) << static_cast<int>(gVal)
        << std::setw(2) << static_cast<int>(bVal);
    data.hexCode = oss.str();

    // Perceptual luminance calculation
    data.luminance = 0.299 * rVal + 0.587 * gVal + 0.114 * bVal;

    return data;
}
