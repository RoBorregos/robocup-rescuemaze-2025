import os
import cv2
import numpy as np
import imgaug.augmenters as iaa
from glob import glob

# Define augmentation sequence
aug = iaa.Sequential([
    iaa.Fliplr(0.5),  # Horizontal flip
    iaa.Flipud(0.5),  # Vertical flip
    iaa.Affine(rotate=(-30, 30)),  # Rotate between -30 to 30 degrees
    iaa.GaussianBlur(sigma=(0, 3.0)),  # Apply Gaussian blur
    iaa.AdditiveGaussianNoise(scale=(10, 50)),  # Add noise
    iaa.Multiply((0.7, 1.3)),  # Change brightness
    iaa.Affine(scale=(0.8, 1.2))  # Scale the image
])

# Define paths
input_dir = "dataset/"  # Change to your dataset folder
output_dir = "augmented_dataset/"
classes = ["H", "S", "U", "white"]
os.makedirs(output_dir, exist_ok=True)

# Process each class
for class_name in classes:
    input_class_dir = os.path.join(input_dir, class_name)
    output_class_dir = os.path.join(output_dir, class_name)
    os.makedirs(output_class_dir, exist_ok=True)
    
    images = glob(os.path.join(input_class_dir, "*.jpg"))  # Adjust extension if needed
    count = 0
    
    while count < 100:
        for img_path in images:
            if count >= 100:
                break
            img = cv2.imread(img_path)
            img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)  # Convert to RGB
            aug_img = aug(image=img)  # Apply augmentation
            aug_img = cv2.cvtColor(aug_img, cv2.COLOR_RGB2BGR)  # Convert back to BGR
            
            output_path = os.path.join(output_class_dir, f"aug_{count}.jpg")
            cv2.imwrite(output_path, aug_img)
            count += 1

print("Data augmentation complete!")
