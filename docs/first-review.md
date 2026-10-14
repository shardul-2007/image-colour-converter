# First Review — Mini Project (OOP)

## Project Information

- **Project Title:** Image Colour Converter
- **Subtitle:** An Object-Oriented Image Colour Conversion Application
- **Course:** Object-Oriented Programming (OOP) Mini Project
- **Academic Review:** First Review
- **Department:** Department of Information Technology
- **Institute:** N.M.V.P. Mandal's Institute of Engineering and Technology (NMIET)
- **Academic Year:** 2026–2027

### Team Details (Team 13)

| Sr. No. | Student Name | Roll No. / Seat No. | Class & Division | Department |
| :---: | :--- | :---: | :---: | :---: |
| 1 | **PARIHAR SHARDUL RAJENDRA** | SEITB243 | SE IT - B | Information Technology |
| 2 | **WANKHADE ISHAN VIJAY** | SEITB266 | SE IT - B | Information Technology |

---

## 1. Project Introduction

In digital image processing, colour manipulation is a fundamental operation used in medical imaging, computer vision, remote sensing, and graphic design. Digital images are stored as multi-dimensional numerical matrices of pixels, typically arranged across colour channels.

The **Image Colour Converter** is an interactive desktop application engineered in modern C++ (C++17) using OpenCV and CMake. Developed for the Object-Oriented Programming (OOP) mini-project review, this application enables users to load digital images, inspect individual pixel values across channels, apply colour transformations (such as grayscale conversion, colour inversion, and individual channel tuning), preview changes in real time, and export the processed output safely to disk.

---

## 2. Problem Statement

Many beginner-level image editing software programs either bundle complex, heavyweight features with opaque user workflows or implement transformations as procedural, monolithic scripts that lack software structure, encapsulation, and proper memory safety.

Furthermore, students and beginner developers frequently confuse **channel byte-order representations** (e.g., OpenCV's default BGR format vs. standard RGB format) with **perceptual colour-space transformations** (e.g., RGB to HSV or CIELAB). There is a distinct educational need for a clean, modular, and beginner-friendly C++ application that demonstrates core OOP design patterns while providing accurate image-processing capabilities and pixel-level inspection.

---

## 3. Objectives

1. Apply Object-Oriented Programming (OOP) principles—including classes, encapsulation, abstraction, and separation of concerns—to solve a real-world image-processing problem.
2. Build an efficient C++ application capable of loading JPG, JPEG, PNG, and BMP image formats.
3. Implement non-destructive colour transformations including BT.601 Grayscale conversion, bitwise colour inversion, channel isolation, and independent channel scaling.
4. Correctly address and explain OpenCV's BGR channel layout versus standard RGB display conventions.
5. Provide an interactive desktop interface with dual image preview and an interactive pixel inspector.
6. Support validated export to PNG and JPEG with comprehensive error handling.
7. Incorporate an automated test suite verifying all 14 project requirements.

---

## 4. Proposed Solution & Architecture

The application adopts a modular, four-class object-oriented architecture where responsibilities are cleanly isolated:

```
[ User Input / CLI / GUI Events ]
               |
               v
     +-------------------+
     | ImageConverterApp | <---+ Maintains active preview & application state
     +---------+---------+
               |
       +-------+-------+
       |               |
       v               v
+--------------+ +-----------------+ +---------------+
| ImageManager | | ColourProcessor | | OutputManager |
+--------------+ +-----------------+ +---------------+
       |                 |                   |
Loads & caches    Performs pure,      Validates paths
pristine image    non-destructive     & writes output
from disk         math transforms     to disk
```

### Class Responsibilities

1. **`ImageManager`**:
   - Decodes images via `cv::imread`.
   - Encapsulates pristine image data (`originalImage`) so it cannot be corrupted.
   - Provides safe accessors for width, height, channel count, and file paths.
   - Performs coordinate bounds checking for pixel inspection.
   - Records detailed I/O error messages.

2. **`ColourProcessor`**:
   - Pure functional processing engine. Never modifies input images.
   - `toGrayscale()`: Converts BGR to 1-channel luminance via standard ITU-R BT.601 ($Y = 0.299R + 0.587G + 0.114B$).
   - `invertColours()`: Generates negative image effect ($\text{out} = 255 - \text{in}$).
   - `adjustChannels()`: Multiplies Blue, Green, Red channels independently by scaling factors with [0, 255] clamping.
   - `isolateChannel()`: Preserves one channel and zeroes out the other two.
   - `convertBgrToRgbRepresentation()`: Swaps Blue and Red channel ordering in memory.
   - `inspectPixel()`: Computes coordinates, channel values, hex string, and luminance.

3. **`OutputManager`**:
   - Encapsulates file-saving operations via `cv::imwrite`.
   - Validates supported extensions (`.png`, `.jpg`, `.jpeg`, `.bmp`).
   - Verifies target directory presence and creates parent folders automatically via `std::filesystem`.
   - Returns structured status and error feedback.

4. **`ImageConverterApp`**:
   - Main coordinator connecting user input to `ImageManager`, `ColourProcessor`, and `OutputManager`.
   - Manages GUI windows (Original Preview, Converted Preview, Dashboard).
   - Handles mouse clicks for interactive pixel inspection.
   - Manages key shortcuts (`[G]`, `[I]`, `[T]`, `[1]`, `[2]`, `[3]`, `[R]`, `[S]`, `[Q]`).
   - Implements automated self-test suite (`runSelfTest()`).

---

## 5. Technologies Used

- **Programming Language:** C++17 (Modern C++ standard with `std::filesystem`, structured bindings, and `<algorithm>`)
- **Computer Vision Library:** OpenCV 4.x (`core`, `imgproc`, `imgcodecs`, `highgui`)
- **Build System:** CMake 3.16+
- **Compiler:** GNU GCC 16.1.0 (MSYS2 UCRT64 toolchain) / MSVC compatible
- **Version Control:** Git & GitHub

---

## 6. Implemented vs. Planned Features

### Current Implemented Features (Review 1)
- [x] Load JPG, JPEG, and PNG images.
- [x] Display original pristine image.
- [x] Read dimensions, channel count, and pixel values.
- [x] Correctly handle OpenCV's BGR channel layout.
- [x] Convert colour image to single-channel Grayscale.
- [x] Invert image colours (negative effect).
- [x] Modify individual colour channels (scaling sliders for Blue, Green, Red).
- [x] Isolate individual colour channels (Blue, Green, Red).
- [x] Correctly toggle between BGR and RGB memory representations.
- [x] Real-time side-by-side preview of converted image.
- [x] Save / export result as PNG or JPEG.
- [x] Reset processing to pristine original image.
- [x] Clear error messages for missing files, invalid extensions, and empty images.
- [x] Interactive pixel inspector displaying coordinates, channel values, Hex `#RRGGBB`, and luminance.
- [x] Automated 14-point test suite for verification.

### Planned Future Scope (Post-Review Enhancements)
- [ ] Perceptual colour-space conversions (HSV, HSL, and CIELAB).
- [ ] Batch processing of multiple images across a directory.
- [ ] Additional image filters (Gaussian blur, Sobel edge detection, sharpening).
- [ ] Dedicated Qt 6 graphical user interface with native file dialogs.
- [ ] AI-based automatic colour balance and exposure correction.

---

## 7. Object-Oriented Concepts Demonstrated

1. **Encapsulation:** Internal data members (`originalImage`, `currentProcessedImage`, `lastError`) are declared `private`. Access is granted strictly through public getters and setters.
2. **Abstraction:** Users and callers interact with high-level conceptual actions (`applyGrayscale()`, `resetToOriginal()`) without needing to understand underlying memory buffer strides or matrix headers.
3. **Modularity & Separation of Responsibilities:** Each class lives in its own `.h` and `.cpp` file and performs one coherent function.
4. **Const-Correctness:** Member functions that do not alter state are marked `const`, and read-only image parameters are passed as `const cv::Mat&`.
5. **Constructors & Resource Management:** Proper constructors initialize default states, while RAII automatically cleans up OpenCV allocations without memory leaks.

---

## 8. Sample Execution Flow

```
1. Application Launch:
   ./image_colour_converter assets/sample.png

2. Image Loaded:
   [ImageConverterApp] Loaded 'assets/sample.png' (400 x 300, 3 channels).

3. User Interaction:
   User clicks on coordinate (100, 75)
   -> [Inspector] Inspected Pixel (100, 75) | B=0 G=0 R=255 | Hex: #FF0000 | Luma: 76.2

4. Transformation:
   User presses [G]
   -> [ImageConverterApp] Converted to Grayscale (1-channel ITU-R BT.601).

5. Export:
   User presses [S] and enters "output/converted.png"
   -> [ImageConverterApp] Success: Image successfully saved to 'output/converted.png' (400x300, 1 channel(s)).
```

---

## 9. Challenges Encountered & Solutions

| Challenge Encountered | Root Cause | Solution Implemented |
| :--- | :--- | :--- |
| **OpenCV BGR Channel Ordering** | OpenCV defaults to Blue-Green-Red byte layout, causing red and blue to appear swapped if interpreted as RGB. | Developed explicit channel mapping and distinguished representation memory swapping from colour-space conversion. |
| **Accidental Image Mutation** | In-place operations on OpenCV matrices alter underlying buffers. | Adopted pure functional processing in `ColourProcessor`, accepting `const cv::Mat&` and returning freshly allocated cloned matrices. |
| **Invalid Output Formats** | User typing unsupported extensions causes runtime exceptions. | Added robust format inspection in `OutputManager::isValidExtension()` supporting `.png`, `.jpg`, `.jpeg`, and `.bmp`. |
| **Automated Testing without GUI** | Continuous integration and headless testing fail if a GUI window is required. | Implemented `--test` flag in `main.cpp` executing a comprehensive 14-point test suite without opening GUI windows. |

---

## 10. Conclusion

The First Review implementation of the **Image Colour Converter** successfully satisfies all academic and technical requirements. The system demonstrates robust Object-Oriented design in C++17, non-destructive image processing, reliable error handling, and beginner-friendly user interaction.
