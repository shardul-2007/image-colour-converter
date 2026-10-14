# Tests & Verification Suite

This directory documents the automated test suite and verification procedures for the **Image Colour Converter** application.

## Overview

The application features a built-in automated test suite covering all core image-processing operations, OOP components, and error-handling paths.

## Running the Automated Test Suite

To run the self-test suite without launching GUI windows:

```bash
# Windows (PowerShell / Command Prompt)
.\build\image_colour_converter.exe --test assets/sample.png test_output

# Linux / macOS
./build/image_colour_converter --test assets/sample.png test_output
```

## 14-Point Test Coverage Matrix

| Test ID | Target Component | Description | Expected Outcome | Verification Status |
| :--- | :--- | :--- | :--- | :--- |
| **Test 1** | `ImageManager` | Error handling for non-existent file path | Fails gracefully, records clear error message | **PASS** |
| **Test 2** | `ImageManager` | Load valid PNG/JPG image file | Returns `true`, allocates pristine `cv::Mat` | **PASS** |
| **Test 3** | `ImageManager` | Validate dimensions & channel metadata | Returns positive width/height, 3 BGR channels | **PASS** |
| **Test 4** | `ImageManager` | Safe pixel reading with bounds checking | Populates B, G, R uint8 channel values | **PASS** |
| **Test 5** | `ColourProcessor` | Convert colour image to Grayscale | Returns 1-channel ITU-R BT.601 matrix | **PASS** |
| **Test 6** | `ColourProcessor` | Invert image colours | Computes `255 - pixel` negative effect | **PASS** |
| **Test 7** | `ColourProcessor` | Modify individual colour channels | Applies custom scale factors with [0, 255] clamping | **PASS** |
| **Test 8** | `ColourProcessor` | Isolate single colour channel (Blue) | Sets Green and Red channels to 0 | **PASS** |
| **Test 9** | `ColourProcessor` | BGR to RGB memory representation toggle | Swaps Channel 0 (Blue) and Channel 2 (Red) | **PASS** |
| **Test 10** | `ImageConverterApp`| Reset to original pristine image | Restores original unmanipulated BGR matrix | **PASS** |
| **Test 11** | `OutputManager` | Save / export converted image as PNG | Writes valid `.png` file to disk | **PASS** |
| **Test 12** | `OutputManager` | Save / export converted image as JPEG | Writes valid `.jpg` file to disk | **PASS** |
| **Test 13** | `OutputManager` | Error handling for invalid output format | Rejects unsupported extensions with error message | **PASS** |
| **Test 14** | `ColourProcessor` | Pixel Inspector formatting and calculations | Produces accurate Hex code (`#RRGGBB`) & luminance | **PASS** |

## Manual GUI Testing Checklist

1. **Launch**: Run `.\build\image_colour_converter.exe assets\sample.png`.
2. **Dual View**: Confirm "Original Image (Pristine BGR)" and "Converted Image Preview" windows appear.
3. **Pixel Inspection**: Click on any coordinate within the image preview window to inspect coordinates, channel values, and hex code.
4. **Grayscale**: Press `[G]` and confirm converted image updates to grayscale while original stays in color.
5. **Inversion**: Press `[I]` and observe color inversion.
6. **Channel Scaling**: Drag the Blue, Green, Red sliders in the Dashboard window.
7. **Representation Toggle**: Press `[T]` and verify Red/Blue channel swap in memory representation.
8. **Reset**: Press `[R]` and confirm the preview reverts to the pristine original state.
9. **Export**: Press `[S]`, specify an output filename (e.g. `output/converted.png`), and verify file creation.
10. **Exit**: Press `[Q]` or `[ESC]` to safely close all windows.
