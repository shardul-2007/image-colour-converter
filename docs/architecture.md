# Software Architecture & Object-Oriented Design

**Project Title:** Image Colour Converter  
**Subtitle:** An Object-Oriented Image Colour Conversion Application  
**Team:** Team 13  
**Team Members:**  
1. PARIHAR SHARDUL RAJENDRA — SEITB243  
2. WANKHADE ISHAN VIJAY — SEITB266  

**Department:** Department of Information Technology  
**Institute:** N.M.V.P. Mandal's Institute of Engineering and Technology (NMIET)  
**Academic Review:** First Review — Mini Project (OOP)  

---

## 1. Architectural Overview

The **Image Colour Converter** is structured strictly around modern Object-Oriented Programming (OOP) principles in C++17. The design emphasizes:

- **Single Responsibility Principle (SRP):** Each class encapsulates a single, well-defined domain of functionality.
- **Data Encapsulation:** Internal image matrices (`cv::Mat`) and state flags are kept private, accessible solely via safe accessors or const references.
- **Non-Destructive Processing:** The pristine source image is stored immutably once loaded; all color-conversion and channel-filtering operations return newly allocated matrices, preventing accidental mutation of input data.
- **Separation of Concerns:** Disk I/O (`ImageManager`, `OutputManager`), numerical pixel transformations (`ColourProcessor`), and presentation/user orchestration (`ImageConverterApp`) are cleanly separated.

```
                      +-----------------------------+
                      |       main() Entry          |
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

---

## 2. Core Class Responsibilities

### 2.1 ImageManager

- **Header:** `include/ImageManager.h` | **Source:** `src/ImageManager.cpp`
- **Purpose:** Manages the lifecycle of the pristine original image.
- **Key Responsibilities:**
  - Securely decodes image files (JPG, JPEG, PNG, BMP) from filesystem paths using `cv::imread`.
  - Encapsulates the immutable `originalImage` matrix.
  - Exposes zero-copy const references (`const cv::Mat& getOriginalImage() const`) and deep copies (`getOriginalImageCopy()`).
  - Reports dimension metrics (`getWidth()`, `getHeight()`, `getChannels()`).
  - Implements safe pixel inspection at coordinate `(x, y)` with boundary checking.
  - Records and surfaces descriptive error messages on failed I/O.

### 2.2 ColourProcessor

- **Header:** `include/ColourProcessor.h` | **Source:** `src/ColourProcessor.cpp`
- **Purpose:** Pure mathematical and image-processing engine.
- **Key Responsibilities:**
  - **Grayscale Conversion:** Computes perceptual luminance using the ITU-R Recommendation BT.601 standard:
    $$\text{Luminance } (Y) = 0.299 \cdot R + 0.587 \cdot G + 0.114 \cdot B$$
  - **Colour Inversion:** Calculates bitwise/intensity inversion:
    $$\text{Pixel}_{\text{out}} = 255 - \text{Pixel}_{\text{in}}$$
  - **Channel Adjustment:** Multiplies individual colour channels by scaling factors and clamps pixel intensities into $[0, 255]$.
  - **Channel Isolation:** Preserves a chosen colour channel (Blue, Green, or Red) while zeroing out all other channels.
  - **BGR $\leftrightarrow$ RGB Representation Toggle:** Swaps Channel 0 (Blue) and Channel 2 (Red) in memory.
  - **Pixel Inspector Helper:** Extracts numerical channel data, formats hexadecimal color strings (`#RRGGBB`), and computes luminance.

> [!IMPORTANT]
> **Representation vs. Colour-Space Conversion:**  
> The BGR-to-RGB function alters the **memory layout / display ordering** of channels for compatibility with libraries expecting RGB (e.g. Qt, OpenGL, Web viewports). This is **not** a perceptual colour-space conversion (such as converting to HSV, CIELAB, or CIE XYZ). The documentation and application interface strictly maintain this distinction.

### 2.3 OutputManager

- **Header:** `include/OutputManager.h` | **Source:** `src/OutputManager.cpp`
- **Purpose:** Handles validation and export of processed images to disk.
- **Key Responsibilities:**
  - Validates output file extensions (`.png`, `.jpg`, `.jpeg`, `.bmp`).
  - Verifies matrix integrity (ensuring empty images are not written).
  - Automatically creates missing destination directories using `std::filesystem`.
  - Executes `cv::imwrite` and returns structured success/failure messages.

### 2.4 ImageConverterApp

- **Header:** `include/ImageConverterApp.h` | **Source:** `src/ImageConverterApp.cpp`
- **Purpose:** Coordinator and mediator. Connects user interaction with processing routines.
- **Key Responsibilities:**
  - Maintains current application state (`currentProcessedImage`, active `ProcessingMode`, representation flag, channel scaling factors).
  - Handles the desktop GUI windows: Original Preview, Converted Preview, and Dashboard.
  - Registers interactive mouse callbacks for real-time pixel inspection upon clicking or hovering.
  - Provides keyboard shortcuts for one-click processing, resetting, and exporting.
  - Implements the non-interactive automated 14-point test suite (`runSelfTest()`).

---

## 3. Object-Oriented Principles Demonstrated

| OOP Principle | Implementation in Project |
| :--- | :--- |
| **Encapsulation** | Private member variables in `ImageManager`, `OutputManager`, and `ImageConverterApp`. State can only be read or modified through public member functions. |
| **Abstraction** | Callers interact with simple operations like `applyGrayscale()` or `saveConvertedImage()` without needing to manage OpenCV low-level buffer pointers or matrix strides. |
| **Modular Separation** | File decoding, mathematical transformation, file encoding, and presentation logic are distributed across independent compilation units (`.h` / `.cpp`). |
| **Value Semantics & Immutability** | Image manipulation functions take `const cv::Mat&` and return new instances, preserving the pristine original image throughout the program's lifecycle. |
| **Constructors & Resource Management** | Default and parameterized constructors ensure objects are initialized in a valid, predictable state. C++ RAII ensures automatic cleanup of OpenCV matrix headers and buffers. |

---

## 4. Execution Data Flow

1. **User Action:** The user supplies an image path via CLI argument or interactive prompt.
2. **Loading & Validation:** `ImageManager::loadImage()` verifies file existence with `std::filesystem`, decodes the image with `cv::imread`, and caches it in `originalImage`.
3. **State Initialization:** `ImageConverterApp` creates an initial copy for `currentProcessedImage` and opens display windows.
4. **Operation Dispatch:** When the user triggers an action (e.g., Grayscale `[G]` or Invert `[I]`), `ImageConverterApp` passes `originalImage` to the corresponding method in `ColourProcessor`.
5. **Preview Update:** The processed matrix is rendered alongside an on-screen HUD displaying dimensions, active mode, and inspected pixel data.
6. **Export:** On save request `[S]`, `OutputManager` validates the target path and writes the matrix to disk via `cv::imwrite`.
