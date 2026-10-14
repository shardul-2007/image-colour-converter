# Assets Directory

This directory contains sample images and asset generation utilities for the **Image Colour Converter** project.

## Included Files

- `sample.png` - A 400x300 lossless 24-bit RGB test pattern image featuring pure Red, Green, Blue, Neutral Gray quadrants, and high-contrast circular geometries. Used as the primary benchmark image for the automated test suite and interactive demos.
- `sample.jpg` - A 400x300 JPEG version of the test pattern (quality 95) for testing JPEG lossy decoding and metadata inspection.
- `generate_assets.py` - Python script used to deterministically reproduce the test pattern assets.

## Test Image Color Specifications

| Region | Position | RGB Color | Expected BGR (OpenCV) | Hex Code |
| :--- | :--- | :--- | :--- | :--- |
| Top-Left Quadrant | `(100, 75)` | `(255, 0, 0)` (Red) | `(0, 0, 255)` | `#FF0000` |
| Top-Right Quadrant | `(300, 75)` | `(0, 255, 0)` (Green) | `(0, 255, 0)` | `#00FF00` |
| Bottom-Left Quadrant | `(100, 225)` | `(0, 0, 255)` (Blue) | `(255, 0, 0)` | `#0000FF` |
| Bottom-Right Quadrant| `(300, 225)` | `(128, 128, 128)` (Gray) | `(128, 128, 128)` | `#808080` |
| Center Outer Disc | `(200, 150)` | `(255, 255, 0)` (Yellow) | `(0, 255, 255)` | `#FFFF00` |
| Center Inner Disc | `(200, 150)` | `(255, 255, 255)` (White) | `(255, 255, 255)` | `#FFFFFF` |
