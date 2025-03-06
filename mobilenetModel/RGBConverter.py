from PIL import Image
import os

# Path to your dataset
dataset_path = "C:\\Users\\ferna\\Documents\\vision_artificial\\fotos\\dataset_HSU\\none"

# Process all images
for root, _, files in os.walk("C:\\Users\\ferna\\Documents\\vision_artificial\\fotos\\dataset_HSU\\none"):
    for file in files:
        if file.lower().endswith((".png", ".jpg", ".jpeg")):
            img_path = os.path.join(root, file)
            img = Image.open(img_path)
            
            # Convert to RGB (removing transparency)
            img = img.convert("RGB")
            
            # Save the updated image
            img.save(img_path)

print("Dataset conversion completed. Transparency removed.")
