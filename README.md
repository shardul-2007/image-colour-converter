# Image Colour Converter

> An Object-Oriented Image Colour Conversion Application built with C++17, OpenCV 4.x, and CMake.

[![Live Web Demo](https://img.shields.io/badge/Live%20Demo-Deployed%20on%20GitHub%20Pages-brightgreen.svg?style=for-the-badge&logo=github)](https://shardul-2007.github.io/image-colour-converter/)

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![OpenCV](https://img.shields.io/badge/OpenCV-4.x-green.svg)](https://opencv.org/)
[![CMake](https://img.shields.io/badge/CMake-3.16%2B-red.svg)](https://cmake.org/)
[![GitHub Actions CI](https://github.com/shardul-2007/image-colour-converter/actions/workflows/ci-build.yml/badge.svg)](https://github.com/shardul-2007/image-colour-converter/actions)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

🌐 **Live Interactive Deployed Demo:** [https://shardul-2007.github.io/image-colour-converter/](https://shardul-2007.github.io/image-colour-converter/)  
📁 **GitHub Source Repository:** [https://github.com/shardul-2007/image-colour-converter](https://github.com/shardul-2007/image-colour-converter)

---

## 1. Project Information

- **Project Title:** Image Colour Converter
- **Subtitle:** An Object-Oriented Image Colour Conversion Application
- **Course Review:** First Review — Mini Project (Object-Oriented Programming)
- **Department:** Department of Information Technology
- **Institute:** N.M.V.P. Mandal's Institute of Engineering and Technology (NMIET)
- **Academic Year:** 2026–2027
- **Team:** Team 13

### Team Members
1. **PARIHAR SHARDUL RAJENDRA** — Roll No. / Seat No.: `SEITB243`
2. **WANKHADE ISHAN VIJAY** — Roll No. / Seat No.: `SEITB266`

---

## 2. Description & Overview

**Image Colour Converter** is a desktop image-processing application developed in modern C++ (C++17) following Object-Oriented Programming principles. The application allows users to load digital images (JPG, JPEG, PNG, BMP), inspect pixel coordinates and channel values in real time, apply non-destructive colour transformations (such as Grayscale conversion, colour inversion, and individual channel adjustments), preview the results side-by-side, and safely export converted images to disk.

---

## 3. Problem Statement

Many beginner-level image-processing programs are written as monolithic procedural scripts with global state, lacking modular encapsulation, error resilience, and separation of concerns. Additionally, beginner developers frequently confuse **channel byte-order representations** (such as OpenCV's default BGR format versus standard RGB ordering) with **perceptual colour-space conversions** (such as RGB to HSV or CIELAB).

There is an educational and practical need for a structured C++ application that demonstrates clean OOP architecture, safe memory handling, transparent error reporting, and accurate pixel-level operations.

---

## 4. Objectives

- Design a modular, class-based C++ architecture adhering to the Single Responsibility Principle.
- Provide reliable image loading and saving across common raster formats (PNG, JPG, JPEG, BMP).
- Implement mathematical colour transformations without mutating original input image data.
- Correctly distinguish between channel memory representation (BGR vs. RGB) and perceptual colour spaces.
- Offer an interactive desktop interface featuring side-by-side previews, on-screen metadata HUD, and an interactive pixel inspector.
- Provide an automated test suite verifying all 14 project requirements without requiring manual GUI interactions.

---

## 5. Current Implemented Features

1. **Format Compatibility:** Loads JPG, JPEG, PNG, and BMP image formats via OpenCV.
2. **Pristine Image Preservation:** Displays and stores the original image immutably.
3. **Metadata Inspection:** Reads image dimensions (width, height), number of channels, and pixel values.
4. **BGR Channel Ordering:** Accurately accounts for OpenCV's native Blue-Green-Red memory layout.
5. **Grayscale Conversion:** Converts colour images to single-channel grayscale using standard ITU-R Recommendation BT.601 luminance weights ($Y = 0.299R + 0.587G + 0.114B$).
6. **Colour Inversion:** Inverts colour intensities ($\text{pixel}_{\text{out}} = 255 - \text{pixel}_{\text{in}}$) producing a negative effect.
7. **Individual Channel Modification:** Modifies Blue, Green, and Red channel scaling independently with automatic $[0, 255]$ clamping.
8. **Channel Isolation:** Isolates a selected channel (Blue, Green, or Red) while setting the remaining channels to zero.
9. **Representation Ordering Toggle:** Swaps Blue and Red channels in memory to toggle between BGR and RGB representations.
10. **Dual-Window Preview:** Displays the original pristine image and the converted preview side-by-side.
11. **Image Export:** Validates and exports converted images as PNG, JPEG, or BMP.
12. **State Reset:** Resets all processing and restores the pristine original image with a single keypress.
13. **Robust Error Handling:** Detects and reports invalid file paths, unsupported extensions, corrupted files, and write failures.
14. **Interactive Pixel Inspector:** Displays coordinates $(x, y)$, channel intensities, Hex color code (`#RRGGBB`), and perceptual luminance on mouse click or hover.

> [!NOTE]
> **Representation Layout vs. Colour-Space Conversion:**  
> Swapping BGR to RGB changes **channel memory order** for compatibility with RGB-based frameworks (like Qt, OpenGL, or web canvases). It is **not** a perceptual colour-space conversion (such as converting to HSV or CIELAB).

---

## 6. Planned Future Features (Not Yet Implemented)

The following features are planned for future academic milestones:
- Perceptual colour-space conversions (HSV, HSL, and CIELAB).
- Batch image processing across directories.
- Additional spatial filters (Gaussian blur, Sobel edge detection, sharpening).
- Dedicated Qt 6 Widgets UI with native file picker dialogs.
- AI-based automatic colour and white-balance correction.

---

## 7. Technology Stack

- **Language:** C++17
- **Libraries:** OpenCV 4.x (`opencv_core`, `opencv_imgproc`, `opencv_imgcodecs`, `opencv_highgui`)
- **Build System:** CMake 3.16+
- **Toolchain:** MSYS2 UCRT64 (GCC 16.1.0, Ninja) / MSVC compatible
- **Version Control:** Git & GitHub

---

## 8. OOP Concepts Demonstrated

- **Encapsulation:** Internal data members (e.g., `originalImage`, `currentProcessedImage`, `lastError`) are declared `private` and manipulated only via public member functions.
- **Abstraction:** High-level interfaces (`applyGrayscale()`, `saveConvertedImage()`) hide low-level OpenCV buffer pointers and strides.
- **Single Responsibility Principle:** Disk I/O, mathematical image transforms, output file validation, and application orchestration are segregated into separate classes.
- **Immutability & Value Semantics:** Processing functions receive `const cv::Mat&` and return new matrices, ensuring the original image is never modified.
- **Resource Management (RAII):** C++ RAII ensures automatic cleanup of image matrices, avoiding memory leaks.

---

## 9. Architecture & Class Responsibilities

```
                      +-----------------------------+
                      |          main.cpp           |
                      +--------------+--------------+
                                     |
                                     v
                      +-----------------------------+
                      |     ImageConverterApp       |
                      |  (Coordinator & Controller) |
                      +----+---------+---------+----+
                           |         |         |
          +----------------+         |         +----------------+
          |                          |                          |
          v                          v                          v
+-------------------+      +-------------------+      +-------------------+
|   ImageManager    |      |  ColourProcessor  |      |   OutputManager   |
| (Image Input &    |      | (Color Transforms |      | (File Export &    |
|  Pristine Cache)  |      |  & Math Engine)   |      |  Disk Validation) |
+-------------------+      +-------------------+      +-------------------+
```

- **`ImageManager`** (`include/ImageManager.h`, `src/ImageManager.cpp`): Decodes image files from disk, stores the pristine original matrix, reports dimensions and channel counts, validates bounds, and records error messages.
- **`ColourProcessor`** (`include/ColourProcessor.h`, `src/ColourProcessor.cpp`): Pure functional calculation engine. Converts to grayscale, inverts colours, scales channels, isolates channels, swaps BGR/RGB representations, and extracts pixel metadata.
- **`OutputManager`** (`include/OutputManager.h`, `src/OutputManager.cpp`): Validates output extensions (`.png`, `.jpg`, `.jpeg`, `.bmp`), creates target directories if needed, and writes matrices to disk with `cv::imwrite`.
- **`ImageConverterApp`** (`include/ImageConverterApp.h`, `src/ImageConverterApp.cpp`): Manages GUI windows, mouse callbacks, trackbars, keyboard shortcuts, application state, and the automated test suite.

---

## 10. Repository Directory Structure

```
image-colour-converter/
├── include/
│   ├── ImageManager.h
│   ├── ColourProcessor.h
│   ├── OutputManager.h
│   └── ImageConverterApp.h
├── src/
│   ├── main.cpp
│   ├── ImageManager.cpp
│   ├── ColourProcessor.cpp
│   ├── OutputManager.cpp
│   └── ImageConverterApp.cpp
├── assets/
│   ├── sample.png
│   ├── sample.jpg
│   ├── generate_assets.py
│   └── README.md
├── tests/
│   └── README.md
├── docs/
│   ├── architecture.md
│   └── first-review.md
├── .gitignore
├── CMakeLists.txt
├── LICENSE
└── README.md
```

---

## 11. Prerequisites & Dependency Installation

### Windows (MSYS2 UCRT64 — Recommended)

1. Install [MSYS2](https://www.msys2.org/).
2. Open the **MSYS2 UCRT64** terminal and install CMake, Ninja, GCC, and OpenCV:
   ```bash
   pacman -S --needed mingw-w64-ucrt-x86_64-gcc \
                      mingw-w64-ucrt-x86_64-cmake \
                      mingw-w64-ucrt-x86_64-ninja \
                      mingw-w64-ucrt-x86_64-opencv
   ```
3. Ensure `C:\msys64\ucrt64\bin` is added to your Windows `PATH` environment variable.

### Linux (Ubuntu / Debian)

```bash
sudo apt update
sudo apt install -y build-essential cmake libopencv-dev
```

---

## 12. Build and Run Instructions

### Step 1: Configure with CMake

From the repository root directory:

```bash
# Windows (PowerShell)
$env:PATH = "C:\msys64\ucrt64\bin;C:\msys64\usr\bin;$env:PATH"
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release

# Linux
cmake -B build -DCMAKE_BUILD_TYPE=Release
```

### Step 2: Compile the Application

```bash
cmake --build build
```

The compiled binary will be placed at `build/image_colour_converter.exe` (Windows) or `build/image_colour_converter` (Linux).

### Step 3: Run the Application

```bash
# 1. Run automated 14-point test suite (recommended first step):
.\build\image_colour_converter.exe --test assets/sample.png test_output

# 2. Launch interactive desktop GUI with sample image:
.\build\image_colour_converter.exe assets/sample.png

# 3. View command-line help:
.\build\image_colour_converter.exe --help
```

---

## 13. Interactive GUI & Keyboard Controls

When running in GUI mode, the application displays three windows:
1. **Original Image (Pristine BGR):** Displays the untouched source image.
2. **Converted Image Preview:** Displays the live transformed image with an on-screen metadata HUD.
3. **Dashboard & Controls:** Contains interactive sliders for adjusting Blue, Green, and Red channel scaling factors.

### Keyboard Shortcuts

| Key | Action | Description |
| :---: | :--- | :--- |
| `[G]` | **Grayscale** | Converts image to 1-channel luminance via ITU-R BT.601 |
| `[I]` | **Invert Colours** | Computes bitwise negative ($255 - \text{pixel}$) |
| `[T]` | **Toggle BGR/RGB** | Swaps Blue and Red memory representations |
| `[1]` | **Isolate Blue** | Preserves Blue channel, zeroes Green and Red |
| `[2]` | **Isolate Green** | Preserves Green channel, zeroes Blue and Red |
| `[3]` | **Isolate Red** | Preserves Red channel, zeroes Blue and Green |
| `[R]` | **Reset** | Restores pristine original image and resets sliders |
| `[S]` | **Save Image** | Prompts in terminal for output path (`.png`, `.jpg`) |
| `[O]` | **Open File** | Prompts in terminal for a new image path |
| `[P]` | **Print Info** | Prints image dimensions, mode, and pixel data to console |
| `[H]` | **Help** | Prints keyboard control legend |
| `[Q]` / `[ESC]` | **Quit** | Closes windows and exits application safely |

### Mouse Controls

- **Left-Click** anywhere on either preview window to inspect pixel coordinates $(x, y)$, channel intensities (B, G, R), Hex code (`#RRGGBB`), and perceptual luminance.

---

## 14. Screenshots Section

> To add genuine application screenshots:
> 1. Launch the application: `.\build\image_colour_converter.exe assets\sample.png`.
> 2. Capture a screenshot of the dual preview and controls window (e.g. using `Windows + Shift + S`).
> 3. Save the image to `docs/screenshots/gui_preview.png`.
> 4. Link the screenshot in this section: `![GUI Preview](docs/screenshots/gui_preview.png)`.

---

## 15. Limitations

- Only raster formats supported by OpenCV (`.jpg`, `.jpeg`, `.png`, `.bmp`) can be opened or saved.
- Video streams and raw vector images (SVG) are not supported.
- Large images ($> 4K$) will be scaled down proportionally for on-screen preview display while full matrix resolution is retained for file export.
- Perceptual colour spaces (such as HSV and CIELAB) are scheduled for subsequent project reviews.

---

## 16. Contributors

- **Parihar Shardul Rajendra** (`SEITB243`) — Department of Information Technology, NMIET
- **Wankhade Ishan Vijay** (`SEITB266`) — Department of Information Technology, NMIET

---

## 17. License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.
