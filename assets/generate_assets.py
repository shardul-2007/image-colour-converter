import os
import math
from PIL import Image, ImageDraw

def create_test_images():
    os.makedirs("assets", exist_ok=True)
    width, height = 400, 300
    img = Image.new("RGB", (width, height), color=(255, 255, 255))
    draw = ImageDraw.Draw(img)

    # 4 Quadrants:
    # Top-Left: Pure Red (255, 0, 0)
    draw.rectangle([0, 0, width // 2, height // 2], fill=(255, 0, 0))
    # Top-Right: Pure Green (0, 255, 0)
    draw.rectangle([width // 2, 0, width, height // 2], fill=(0, 255, 0))
    # Bottom-Left: Pure Blue (0, 0, 255)
    draw.rectangle([0, height // 2, width // 2, height], fill=(0, 0, 255))
    # Bottom-Right: Gradient / Neutral Gray (128, 128, 128)
    draw.rectangle([width // 2, height // 2, width, height], fill=(128, 128, 128))

    # Center circle: Bright Yellow (255, 255, 0)
    cx, cy, r = width // 2, height // 2, 45
    draw.ellipse([cx - r, cy - r, cx + r, cy + r], fill=(255, 255, 0), outline=(0, 0, 0), width=2)

    # Inner circle: White (255, 255, 255)
    ir = 20
    draw.ellipse([cx - ir, cy - ir, cx + ir, cy + ir], fill=(255, 255, 255), outline=(0, 0, 0), width=1)

    # Save as PNG (lossless)
    png_path = os.path.join("assets", "sample.png")
    img.save(png_path, "PNG")
    print(f"Generated {png_path} ({width}x{height})")

    # Save as JPEG
    jpg_path = os.path.join("assets", "sample.jpg")
    img.save(jpg_path, "JPEG", quality=95)
    print(f"Generated {jpg_path} ({width}x{height})")

if __name__ == "__main__":
    create_test_images()
