#include "ImageConverterApp.h"
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

void printBanner() {
    std::cout << "=================================================================\n";
    std::cout << "                     IMAGE COLOUR CONVERTER                      \n";
    std::cout << "         An Object-Oriented Image Colour Conversion Application   \n";
    std::cout << "=================================================================\n";
    std::cout << " Academic Review: First Review - Mini Project (OOP)\n";
    std::cout << " Department:      Department of Information Technology\n";
    std::cout << " Institute:       N.M.V.P. Mandal's Institute of Engineering and\n";
    std::cout << "                  Technology (NMIET)\n";
    std::cout << " Team:            Team 13\n";
    std::cout << " Team Members:\n";
    std::cout << "   1. PARIHAR SHARDUL RAJENDRA - SEITB243\n";
    std::cout << "   2. WANKHADE ISHAN VIJAY     - SEITB266\n";
    std::cout << " Technology Stack: C++17, OpenCV 4.x, CMake\n";
    std::cout << "=================================================================\n\n";
}

void printUsage(const char* progName) {
    std::cout << "Usage Options:\n";
    std::cout << "  " << progName << "                       Launch interactive GUI (will prompt for image)\n";
    std::cout << "  " << progName << " <image_path>          Launch interactive GUI directly with image\n";
    std::cout << "  " << progName << " --test <image_path>   Run automated 14-point test suite\n";
    std::cout << "  " << progName << " --help                Display this help screen\n\n";
}

int main(int argc, char* argv[]) {
    printBanner();

    ImageConverterApp app;

    if (argc >= 2) {
        std::string arg1 = argv[1];

        if (arg1 == "--help" || arg1 == "-h") {
            printUsage(argv[0]);
            return 0;
        }

        if (arg1 == "--test") {
            std::string testImage = (argc >= 3) ? argv[2] : "assets/sample.png";
            std::string outDir = (argc >= 4) ? argv[3] : "test_output";
            return app.runSelfTest(testImage, outDir);
        }

        // Argument is an image path for GUI mode
        app.runGui(arg1);
        return 0;
    }

    // Default mode: check if a default asset exists in assets/
    std::string defaultPath = "assets/sample.png";
    if (fs::exists(defaultPath)) {
        std::cout << "Auto-detected sample image at '" << defaultPath << "'.\n";
        app.runGui(defaultPath);
    } else {
        app.runGui("");
    }

    return 0;
}
