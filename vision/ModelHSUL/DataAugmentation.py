import os
import cv2
import random
import numpy as np
import matplotlib.pyplot as plt
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Conv2D, MaxPooling2D, Flatten, Dense, Dropout
from tensorflow.keras.preprocessing.image import ImageDataGenerator
from tensorflow.keras.callbacks import EarlyStopping, ReduceLROnPlateau
import tensorflow as tf

# ------------------------------
# Step 1: Data Augmentation
# ------------------------------

# Function to add random brightness and texture
def augment_image(image, is_white=False):
    """
    Apply geometric transformations, brightness adjustments, and noise to an image.
    """
    if is_white:
        # For white class, optionally add slight noise and brightness adjustments
        noise = np.random.normal(0, 10, image.shape).astype(np.uint8)
        brightened = np.clip(image + noise, 0, 255).astype(np.uint8)
        return brightened

    # Random rotation (-45 to 45 degrees)
    angle = random.uniform(-45, 45)
    h, w = image.shape[:2]
    M = cv2.getRotationMatrix2D((w // 2, h // 2), angle, 1.0)
    rotated = cv2.warpAffine(image, M, (w, h), borderMode=cv2.BORDER_REFLECT)

    # Random brightness adjustment
    brightness = random.uniform(0.5, 1.5)
    brightened = np.clip(rotated * brightness, 0, 255).astype(np.uint8)

    # Add Gaussian noise
    noise = np.random.normal(0, 15, brightened.shape).astype(np.uint8)
    noisy_image = cv2.add(brightened, noise)

    return noisy_image

# Create augmented dataset
def create_augmented_dataset(input_dir, output_dir, total_images_per_class=500):
    """
    Generate augmented images from input letter images.
    """
    os.makedirs(output_dir, exist_ok=True)

    for letter_file in os.listdir(input_dir):
        if letter_file.lower().endswith(('.png', '.jpg', '.jpeg')):
            letter_name = os.path.splitext(letter_file)[0]
            is_white = "white" in letter_name.lower()
            print(f"Processing letter: {letter_name}")

            # Load the original image
            image_path = os.path.join(input_dir, letter_file)
            image = cv2.imread(image_path, cv2.IMREAD_GRAYSCALE)
            if image is None:
                print(f"Error loading {image_path}")
                continue

            # Create letter folder
            letter_dir = os.path.join(output_dir, letter_name)
            os.makedirs(letter_dir, exist_ok=True)

            # Generate augmented images
            for i in range(total_images_per_class):
                augmented_image = augment_image(image, is_white=is_white)
                output_path = os.path.join(letter_dir, f"{letter_name}_{i}.png")
                cv2.imwrite(output_path, augmented_image)

    print("Dataset augmentation completed successfully!")