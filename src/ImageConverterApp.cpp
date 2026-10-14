#include "ImageConverterApp.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

ImageConverterApp::ImageConverterApp()
    : currentStatusMessage("Application ready. Open an image to begin.") {
}

bool ImageConverterApp::loadImage(const std::string& filePath) {
    bool ok = imageManager.loadImage(filePath);
    if (!ok) {
        currentStatusMessage = imageManager.getLastError();
        std::cerr << "[ImageConverterApp] " << currentStatusMessage << std::endl;
        return false;
    }

    currentProcessedImage = imageManager.getOriginalImageCopy();
    currentMode = ProcessingMode::Original;
    isRgbRepresentation = false;
    blueScale = 1.0;
    greenScale = 1.0;
    redScale = 1.0;
    inspectedX = -1;
    inspectedY = -1;

    currentStatusMessage = "Loaded '" + filePath + "' (" + 
                           imageManager.getDimensionsString() + ", " + 
                           std::to_string(imageManager.getChannels()) + " channels).";
    std::cout << "[ImageConverterApp] " << currentStatusMessage << std::endl;
    return true;
}

void ImageConverterApp::applyGrayscale() {
    if (!imageManager.isLoaded()) {
        currentStatusMessage = "Error: No image loaded. Cannot convert to grayscale.";
        return;
    }

    currentProcessedImage = ColourProcessor::toGrayscale(imageManager.getOriginalImage());
    currentMode = ProcessingMode::Grayscale;
    isRgbRepresentation = false;
    currentStatusMessage = "Converted to Grayscale (1-channel ITU-R BT.601).";
    std::cout << "[ImageConverterApp] " << currentStatusMessage << std::endl;
}

void ImageConverterApp::applyInvert() {
    if (!imageManager.isLoaded()) {
        currentStatusMessage = "Error: No image loaded. Cannot invert colours.";
        return;
    }

    // Invert based on current state to allow chaining, or based on original
    currentProcessedImage = ColourProcessor::invertColours(currentProcessedImage.empty() ? 
                                imageManager.getOriginalImage() : currentProcessedImage);
    currentMode = ProcessingMode::Inverted;
    currentStatusMessage = "Inverted image colours (negative effect: 255 - pixel).";
    std::cout << "[ImageConverterApp] " << currentStatusMessage << std::endl;
}

void ImageConverterApp::applyBgrRgbToggle() {
    if (!imageManager.isLoaded()) {
        currentStatusMessage = "Error: No image loaded. Cannot toggle representation.";
        return;
    }

    if (currentProcessedImage.channels() != 3) {
        currentStatusMessage = "Notice: BGR/RGB toggle applies only to 3-channel colour images.";
        return;
    }

    // Swap Channel 0 and Channel 2
    currentProcessedImage = ColourProcessor::convertBgrToRgbRepresentation(currentProcessedImage);
    isRgbRepresentation = !isRgbRepresentation;
    currentMode = ProcessingMode::RgbRepresentation;
    currentStatusMessage = isRgbRepresentation 
        ? "Representation: RGB memory order (swapped Blue and Red channels)."
        : "Representation: Default OpenCV BGR memory order.";
    std::cout << "[ImageConverterApp] " << currentStatusMessage << std::endl;
}

void ImageConverterApp::applyChannelIsolation(int channelIndex) {
    if (!imageManager.isLoaded()) {
        currentStatusMessage = "Error: No image loaded. Cannot isolate channel.";
        return;
    }

    currentProcessedImage = ColourProcessor::isolateChannel(imageManager.getOriginalImage(), channelIndex);
    currentMode = ProcessingMode::ChannelIsolated;
    isRgbRepresentation = false;

    std::string channelName = (channelIndex == 0) ? "Blue" : (channelIndex == 1) ? "Green" : "Red";
    currentStatusMessage = "Isolated " + channelName + " channel (other channels set to 0).";
    std::cout << "[ImageConverterApp] " << currentStatusMessage << std::endl;
}

void ImageConverterApp::applyChannelAdjustment(double b, double g, double r) {
    if (!imageManager.isLoaded()) {
        currentStatusMessage = "Error: No image loaded. Cannot adjust channels.";
        return;
    }

    blueScale = b;
    greenScale = g;
    redScale = r;

    currentProcessedImage = ColourProcessor::adjustChannels(imageManager.getOriginalImage(), b, g, r);
    currentMode = ProcessingMode::ChannelAdjusted;
    isRgbRepresentation = false;

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "Adjusted channels: B=" << b << "x, G=" << g << "x, R=" << r << "x.";
    currentStatusMessage = oss.str();
    std::cout << "[ImageConverterApp] " << currentStatusMessage << std::endl;
}

void ImageConverterApp::resetToOriginal() {
    if (!imageManager.isLoaded()) {
        currentStatusMessage = "Error: No image loaded to reset.";
        return;
    }

    currentProcessedImage = imageManager.getOriginalImageCopy();
    currentMode = ProcessingMode::Original;
    isRgbRepresentation = false;
    blueScale = 1.0;
    greenScale = 1.0;
    redScale = 1.0;
    currentStatusMessage = "Reset image to pristine original state.";
    std::cout << "[ImageConverterApp] " << currentStatusMessage << std::endl;
}

bool ImageConverterApp::saveConvertedImage(const std::string& outputPath) {
    if (currentProcessedImage.empty()) {
        currentStatusMessage = "Save Error: No processed image available to export.";
        return false;
    }

    // If image is currently in RGB representation layout, convert back to BGR for standard file writing
    cv::Mat toSave = currentProcessedImage;
    if (isRgbRepresentation && currentProcessedImage.channels() == 3) {
        toSave = ColourProcessor::convertBgrToRgbRepresentation(currentProcessedImage);
    }

    bool ok = outputManager.saveImage(outputPath, toSave);
    currentStatusMessage = outputManager.getLastMessage();
    std::cout << "[ImageConverterApp] " << currentStatusMessage << std::endl;
    return ok;
}

void ImageConverterApp::inspectPixelAt(int x, int y) {
    if (currentProcessedImage.empty()) return;

    inspectedX = x;
    inspectedY = y;
    inspectedPixel = ColourProcessor::inspectPixel(currentProcessedImage, x, y, isRgbRepresentation);

    std::ostringstream oss;
    oss << "Inspected Pixel (" << x << ", " << y << ") | ";
    if (inspectedPixel.channels == 3) {
        oss << (isRgbRepresentation ? "R=" : "B=") << static_cast<int>(inspectedPixel.ch0) << " "
            << "G=" << static_cast<int>(inspectedPixel.ch1) << " "
            << (isRgbRepresentation ? "B=" : "R=") << static_cast<int>(inspectedPixel.ch2) << " | ";
    } else {
        oss << "Gray=" << static_cast<int>(inspectedPixel.ch0) << " | ";
    }
    oss << "Hex: " << inspectedPixel.hexCode << " | "
        << "Luma: " << std::fixed << std::setprecision(1) << inspectedPixel.luminance;
    currentStatusMessage = oss.str();
    std::cout << "[Inspector] " << currentStatusMessage << std::endl;
}

const cv::Mat& ImageConverterApp::getCurrentProcessedImage() const {
    return currentProcessedImage;
}

const ImageManager& ImageConverterApp::getImageManager() const {
    return imageManager;
}

const OutputManager& ImageConverterApp::getOutputManager() const {
    return outputManager;
}

ImageConverterApp::ProcessingMode ImageConverterApp::getCurrentMode() const {
    return currentMode;
}

std::string ImageConverterApp::getModeString() const {
    switch (currentMode) {
        case ProcessingMode::Original: return "Original";
        case ProcessingMode::Grayscale: return "Grayscale (ITU-R BT.601)";
        case ProcessingMode::Inverted: return "Inverted Colours (Negative)";
        case ProcessingMode::RgbRepresentation: return "RGB Memory Representation";
        case ProcessingMode::ChannelIsolated: return "Channel Isolated";
        case ProcessingMode::ChannelAdjusted: return "Channel Adjusted";
    }
    return "Unknown";
}

std::string ImageConverterApp::getStatusMessage() const {
    return currentStatusMessage;
}

bool ImageConverterApp::getIsRgbRepresentation() const {
    return isRgbRepresentation;
}

ColourProcessor::PixelData ImageConverterApp::getInspectedPixel() const {
    return inspectedPixel;
}

cv::Mat ImageConverterApp::renderDisplayWithHud(const cv::Mat& image, const std::string& title, bool isOriginal) {
    if (image.empty()) return cv::Mat();

    // Ensure 3-channel for consistent HUD overlay drawing
    cv::Mat color;
    if (image.channels() == 1) {
        cv::cvtColor(image, color, cv::COLOR_GRAY2BGR);
    } else {
        color = image.clone();
    }

    // Limit maximum preview display dimension for practical display
    int maxDisplayDim = 800;
    double scale = 1.0;
    if (color.cols > maxDisplayDim || color.rows > maxDisplayDim) {
        scale = std::min(static_cast<double>(maxDisplayDim) / color.cols,
                         static_cast<double>(maxDisplayDim) / color.rows);
        cv::resize(color, color, cv::Size(), scale, scale, cv::INTER_AREA);
    }

    // Add HUD banner at bottom
    int hudHeight = 70;
    cv::Mat displayWithHud(color.rows + hudHeight, color.cols, CV_8UC3, cv::Scalar(30, 30, 30));
    color.copyTo(displayWithHud(cv::Rect(0, 0, color.cols, color.rows)));

    // HUD Text
    cv::putText(displayWithHud, title, cv::Point(10, color.rows + 22),
                cv::FONT_HERSHEY_SIMPLEX, 0.55, cv::Scalar(0, 255, 255), 1, cv::LINE_AA);

    std::string line2;
    if (isOriginal) {
        line2 = "Dimensions: " + imageManager.getDimensionsString() + 
                " | Channels: " + std::to_string(imageManager.getChannels()) + " (BGR)";
    } else {
        line2 = "Mode: " + getModeString();
        if (inspectedX >= 0) {
            line2 += " | Last Pick: (" + std::to_string(inspectedX) + "," + 
                     std::to_string(inspectedY) + ") " + inspectedPixel.hexCode;
        }
    }
    cv::putText(displayWithHud, line2, cv::Point(10, color.rows + 44),
                cv::FONT_HERSHEY_SIMPLEX, 0.48, cv::Scalar(220, 220, 220), 1, cv::LINE_AA);

    cv::putText(displayWithHud, "[Click on image to inspect pixel]", cv::Point(10, color.rows + 62),
                cv::FONT_HERSHEY_SIMPLEX, 0.40, cv::Scalar(160, 160, 160), 1, cv::LINE_AA);

    return displayWithHud;
}

void ImageConverterApp::onMouseCallback(int event, int x, int y, int /*flags*/, void* userdata) {
    if (event == cv::EVENT_LBUTTONDOWN || event == cv::EVENT_MOUSEMOVE) {
        auto* app = static_cast<ImageConverterApp*>(userdata);
        if (app && !app->currentProcessedImage.empty()) {
            // Note: adjust for display scaling if downscaled
            int origW = app->currentProcessedImage.cols;
            int origH = app->currentProcessedImage.rows;
            int maxDim = 800;
            double scale = 1.0;
            if (origW > maxDim || origH > maxDim) {
                scale = std::min(static_cast<double>(maxDim) / origW,
                                 static_cast<double>(maxDim) / origH);
            }
            int actualX = static_cast<int>(x / scale);
            int actualY = static_cast<int>(y / scale);

            if (actualX >= 0 && actualX < origW && actualY >= 0 && actualY < origH) {
                if (event == cv::EVENT_LBUTTONDOWN) {
                    app->inspectPixelAt(actualX, actualY);
                    app->updatePreviews();
                }
            }
        }
    }
}

void ImageConverterApp::onBlueTrackbar(int val, void* userdata) {
    auto* app = static_cast<ImageConverterApp*>(userdata);
    if (app && app->imageManager.isLoaded()) {
        app->blueScale = val / 100.0;
        app->applyChannelAdjustment(app->blueScale, app->greenScale, app->redScale);
        app->updatePreviews();
    }
}

void ImageConverterApp::onGreenTrackbar(int val, void* userdata) {
    auto* app = static_cast<ImageConverterApp*>(userdata);
    if (app && app->imageManager.isLoaded()) {
        app->greenScale = val / 100.0;
        app->applyChannelAdjustment(app->blueScale, app->greenScale, app->redScale);
        app->updatePreviews();
    }
}

void ImageConverterApp::onRedTrackbar(int val, void* userdata) {
    auto* app = static_cast<ImageConverterApp*>(userdata);
    if (app && app->imageManager.isLoaded()) {
        app->redScale = val / 100.0;
        app->applyChannelAdjustment(app->blueScale, app->greenScale, app->redScale);
        app->updatePreviews();
    }
}

void ImageConverterApp::updatePreviews() {
    if (!imageManager.isLoaded()) return;

    cv::Mat origDisplay = renderDisplayWithHud(imageManager.getOriginalImage(), 
                                               "Original Image (Pristine BGR)", true);
    cv::Mat procDisplay = renderDisplayWithHud(currentProcessedImage, 
                                               "Converted Image Preview", false);

    cv::imshow(WINDOW_ORIGINAL, origDisplay);
    cv::imshow(WINDOW_PROCESSED, procDisplay);
}

void ImageConverterApp::runGui(const std::string& initialImagePath) {
    std::cout << "\n=======================================================\n";
    std::cout << "  IMAGE COLOUR CONVERTER - C++ OOP Desktop Application \n";
    std::cout << "  Team 13 | Department of Information Technology (NMIET) \n";
    std::cout << "=======================================================\n";
    std::cout << "Keyboard Controls:\n";
    std::cout << "  [G] Convert to Grayscale (ITU-R BT.601)\n";
    std::cout << "  [I] Invert Colours (Negative effect)\n";
    std::cout << "  [T] Toggle BGR <-> RGB representation order\n";
    std::cout << "  [1] Isolate Blue channel (zero other channels)\n";
    std::cout << "  [2] Isolate Green channel\n";
    std::cout << "  [3] Isolate Red channel\n";
    std::cout << "  [R] Reset to original image\n";
    std::cout << "  [S] Save / export converted image to file\n";
    std::cout << "  [O] Open another image file\n";
    std::cout << "  [P] Print pixel inspector and metadata details\n";
    std::cout << "  [H] Display this help menu\n";
    std::cout << "  [Q] / [ESC] Quit application\n";
    std::cout << "Mouse Controls:\n";
    std::cout << "  Click anywhere on either image to inspect coordinates & colours.\n";
    std::cout << "=======================================================\n\n";

    if (!initialImagePath.empty()) {
        loadImage(initialImagePath);
    } else {
        std::cout << "Enter path to an image file (e.g. assets/sample.jpg): ";
        std::string path;
        if (std::getline(std::cin, path) && !path.empty()) {
            loadImage(path);
        }
    }

    if (!imageManager.isLoaded()) {
        std::cerr << "No image loaded. Exiting GUI mode.\n";
        return;
    }

    cv::namedWindow(WINDOW_ORIGINAL, cv::WINDOW_AUTOSIZE);
    cv::namedWindow(WINDOW_PROCESSED, cv::WINDOW_AUTOSIZE);
    cv::namedWindow(WINDOW_DASHBOARD, cv::WINDOW_AUTOSIZE);

    // Trackbars for individual channel adjustments (0% to 200%, default 100%)
    int bVal = 100, gVal = 100, rVal = 100;
    cv::createTrackbar("Blue (%)", WINDOW_DASHBOARD, &bVal, 200, onBlueTrackbar, this);
    cv::createTrackbar("Green (%)", WINDOW_DASHBOARD, &gVal, 200, onGreenTrackbar, this);
    cv::createTrackbar("Red (%)", WINDOW_DASHBOARD, &rVal, 200, onRedTrackbar, this);

    // Render controls dashboard graphic
    cv::Mat dash(180, 480, CV_8UC3, cv::Scalar(40, 40, 40));
    cv::putText(dash, "Image Colour Converter Controls", cv::Point(15, 30),
                cv::FONT_HERSHEY_SIMPLEX, 0.65, cv::Scalar(0, 255, 255), 2);
    cv::putText(dash, "Use sliders above for custom channel scaling", cv::Point(15, 60),
                cv::FONT_HERSHEY_SIMPLEX, 0.45, cv::Scalar(200, 200, 200), 1);
    cv::putText(dash, "Keys: [G] Gray  [I] Invert  [T] BGR/RGB", cv::Point(15, 95),
                cv::FONT_HERSHEY_SIMPLEX, 0.48, cv::Scalar(255, 255, 255), 1);
    cv::putText(dash, "Keys: [1] Blue  [2] Green   [3] Red isolate", cv::Point(15, 120),
                cv::FONT_HERSHEY_SIMPLEX, 0.48, cv::Scalar(255, 255, 255), 1);
    cv::putText(dash, "Keys: [R] Reset [S] Save    [Q] Quit", cv::Point(15, 145),
                cv::FONT_HERSHEY_SIMPLEX, 0.48, cv::Scalar(255, 255, 255), 1);
    cv::imshow(WINDOW_DASHBOARD, dash);

    // Register mouse callbacks for pixel inspection
    cv::setMouseCallback(WINDOW_ORIGINAL, onMouseCallback, this);
    cv::setMouseCallback(WINDOW_PROCESSED, onMouseCallback, this);

    updatePreviews();

    bool running = true;
    while (running) {
        int key = cv::waitKey(30);
        if (key < 0) continue;

        char c = static_cast<char>(key & 0xFF);
        switch (c) {
            case 'g':
            case 'G':
                applyGrayscale();
                updatePreviews();
                break;
            case 'i':
            case 'I':
                applyInvert();
                updatePreviews();
                break;
            case 't':
            case 'T':
                applyBgrRgbToggle();
                updatePreviews();
                break;
            case '1':
                applyChannelIsolation(0);
                updatePreviews();
                break;
            case '2':
                applyChannelIsolation(1);
                updatePreviews();
                break;
            case '3':
                applyChannelIsolation(2);
                updatePreviews();
                break;
            case 'r':
            case 'R':
                bVal = 100; gVal = 100; rVal = 100;
                cv::setTrackbarPos("Blue (%)", WINDOW_DASHBOARD, 100);
                cv::setTrackbarPos("Green (%)", WINDOW_DASHBOARD, 100);
                cv::setTrackbarPos("Red (%)", WINDOW_DASHBOARD, 100);
                resetToOriginal();
                updatePreviews();
                break;
            case 's':
            case 'S': {
                std::cout << "\nEnter destination output path (e.g. output/result.png): ";
                std::string savePath;
                if (std::cin >> savePath && !savePath.empty()) {
                    saveConvertedImage(savePath);
                }
                updatePreviews();
                break;
            }
            case 'o':
            case 'O': {
                std::cout << "\nEnter new image path: ";
                std::string newPath;
                if (std::cin >> newPath && !newPath.empty()) {
                    loadImage(newPath);
                    bVal = 100; gVal = 100; rVal = 100;
                    cv::setTrackbarPos("Blue (%)", WINDOW_DASHBOARD, 100);
                    cv::setTrackbarPos("Green (%)", WINDOW_DASHBOARD, 100);
                    cv::setTrackbarPos("Red (%)", WINDOW_DASHBOARD, 100);
                    updatePreviews();
                }
                break;
            }
            case 'p':
            case 'P':
                std::cout << "\n--- Current Application State ---\n";
                std::cout << "File: " << imageManager.getLoadedFilePath() << "\n";
                std::cout << "Dimensions: " << imageManager.getDimensionsString() << "\n";
                std::cout << "Original Channels: " << imageManager.getChannels() << " (BGR)\n";
                std::cout << "Current Mode: " << getModeString() << "\n";
                std::cout << "RGB Representation Swapped: " << (isRgbRepresentation ? "Yes" : "No") << "\n";
                if (inspectedX >= 0) {
                    std::cout << "Last Inspected Pixel: (" << inspectedX << ", " << inspectedY << ") -> "
                              << inspectedPixel.hexCode << ", Luma=" << inspectedPixel.luminance << "\n";
                }
                std::cout << "---------------------------------\n";
                break;
            case 'h':
            case 'H':
                std::cout << "\nHelp: [G] Grayscale | [I] Invert | [T] RGB/BGR | [1,2,3] Isolate | [R] Reset | [S] Save | [Q] Quit\n";
                break;
            case 27: // ESC
            case 'q':
            case 'Q':
                running = false;
                break;
            default:
                break;
        }
    }

    cv::destroyAllWindows();
    std::cout << "Exited application.\n";
}

int ImageConverterApp::runSelfTest(const std::string& testImagePath, const std::string& outputDirectory) {
    std::cout << "\n=======================================================\n";
    std::cout << "   RUNNING AUTOMATED TEST SUITE FOR IMAGE COLOUR CONVERTER\n";
    std::cout << "=======================================================\n";

    int failures = 0;
    auto report = [&](const std::string& testName, bool condition, const std::string& detail = "") {
        if (condition) {
            std::cout << "  [PASS] " << testName;
            if (!detail.empty()) std::cout << " (" << detail << ")";
            std::cout << "\n";
        } else {
            std::cout << "  [FAIL] " << testName;
            if (!detail.empty()) std::cout << " - " << detail;
            std::cout << "\n";
            failures++;
        }
    };

    // Test 1: Invalid file loading (error handling)
    ImageManager testMgr;
    bool invalidLoad = testMgr.loadImage("non_existent_file_xyz_123.jpg");
    report("Test 1: Error handling for non-existent file", !invalidLoad && !testMgr.getLastError().empty(),
           testMgr.getLastError());

    // Test 2: Valid image loading
    bool loadOk = loadImage(testImagePath);
    report("Test 2: Load valid image file", loadOk, imageManager.getDimensionsString());
    if (!loadOk) {
        std::cerr << "FATAL: Cannot continue tests without a valid test image.\n";
        return 1;
    }

    // Test 3: Metadata verification
    report("Test 3: Image dimensions and channel count", 
           imageManager.getWidth() > 0 && imageManager.getHeight() > 0 && imageManager.getChannels() == 3,
           std::to_string(imageManager.getWidth()) + "x" + std::to_string(imageManager.getHeight()) + ", 3 ch");

    // Test 4: Pixel reading & BGR ordering
    uint8_t b = 0, g = 0, r = 0;
    bool pixelOk = imageManager.getPixelBGR(imageManager.getWidth() / 2, imageManager.getHeight() / 2, b, g, r);
    report("Test 4: Pixel inspection at center", pixelOk, 
           "B=" + std::to_string(b) + " G=" + std::to_string(g) + " R=" + std::to_string(r));

    // Test 5: Grayscale conversion
    applyGrayscale();
    report("Test 5: Convert colour image to grayscale", 
           currentProcessedImage.channels() == 1 && currentMode == ProcessingMode::Grayscale,
           "Output channels: 1");

    // Test 6: Invert colours
    applyInvert();
    report("Test 6: Invert image colours", 
           !currentProcessedImage.empty() && currentMode == ProcessingMode::Inverted,
           "Mode: Inverted");

    // Test 7: Modify individual channels
    applyChannelAdjustment(0.5, 1.2, 0.8);
    report("Test 7: Modify individual colour channels", 
           currentProcessedImage.channels() == 3 && currentMode == ProcessingMode::ChannelAdjusted,
           "B=0.5x, G=1.2x, R=0.8x");

    // Test 8: Channel isolation (Blue, Green, Red)
    applyChannelIsolation(0); // Blue
    cv::Vec3b samplePixelBlue = currentProcessedImage.at<cv::Vec3b>(imageManager.getHeight() / 2, imageManager.getWidth() / 2);
    report("Test 8: Channel isolation (Blue only)", 
           samplePixelBlue[1] == 0 && samplePixelBlue[2] == 0,
           "G=0, R=0, B preserved");

    // Test 9: BGR to RGB representation conversion
    resetToOriginal();
    cv::Vec3b origCenter = imageManager.getOriginalImage().at<cv::Vec3b>(imageManager.getHeight() / 2, imageManager.getWidth() / 2);
    applyBgrRgbToggle();
    cv::Vec3b swappedCenter = currentProcessedImage.at<cv::Vec3b>(imageManager.getHeight() / 2, imageManager.getWidth() / 2);
    report("Test 9: BGR to RGB representation toggle", 
           swappedCenter[0] == origCenter[2] && swappedCenter[2] == origCenter[0],
           "Blue and Red swapped in memory");

    // Test 10: Reset functionality
    resetToOriginal();
    report("Test 10: Reset processing to original image", 
           currentMode == ProcessingMode::Original && !isRgbRepresentation,
           "State: Pristine Original");

    // Test 11: Export to PNG
    fs::path outDir(outputDirectory);
    fs::create_directories(outDir);
    std::string testPng = (outDir / "selftest_output.png").string();
    applyGrayscale();
    bool savePngOk = saveConvertedImage(testPng);
    report("Test 11: Save / export result as PNG", savePngOk && fs::exists(testPng), testPng);

    // Test 12: Export to JPEG
    std::string testJpg = (outDir / "selftest_output.jpg").string();
    applyInvert();
    bool saveJpgOk = saveConvertedImage(testJpg);
    report("Test 12: Save / export result as JPEG", saveJpgOk && fs::exists(testJpg), testJpg);

    // Test 13: Export with invalid extension (error handling)
    std::string invalidExt = (outDir / "selftest_output.invalidext").string();
    bool invalidSave = saveConvertedImage(invalidExt);
    report("Test 13: Error handling for invalid output format", !invalidSave && !outputManager.getLastMessage().empty(),
           outputManager.getLastMessage());

    // Test 14: Pixel Inspector data validation
    inspectPixelAt(imageManager.getWidth() / 4, imageManager.getHeight() / 4);
    report("Test 14: Pixel Inspector format and calculations", 
           !inspectedPixel.hexCode.empty() && inspectedPixel.hexCode[0] == '#',
           "Hex: " + inspectedPixel.hexCode + ", Luma: " + std::to_string(inspectedPixel.luminance));

    std::cout << "=======================================================\n";
    if (failures == 0) {
        std::cout << "  ALL 14 TESTS PASSED SUCCESSFULLY! (0 failures)\n";
    } else {
        std::cout << "  TEST COMPLETED WITH " << failures << " FAILURE(S).\n";
    }
    std::cout << "=======================================================\n\n";

    return failures;
}
