# Import libraries
import numpy as np
from scipy.ndimage import convolve
import os
import cv2
import numpy as np
import matplotlib.pyplot as plt
import zipfile
from scipy.ndimage import maximum_filter
import time
from scipy.spatial.distance import cdist
from scipy.ndimage import generate_binary_structure, binary_erosion


# this was common for all fast implementations, (execpt rgb split) the difference was weighting method so that is taken
# as an argument
def harris_corner_detector(image, window_size=3, k=0.04, threshold=0.01, weighting_method="Uniform"):
    # Convert to grayscale and apply Gaussian blur
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    img_gaussian = cv2.GaussianBlur(gray, (3, 3), 0)

    # Calculate image gradients using the Sobel operators
    sobel_x = np.array([[-1, 0, 1], [-2, 0, 2], [-1, 0, 1]])
    sobel_y = np.array([[1, 2, 1], [0, 0, 0], [-1, -2, -1]])

    Ix = cv2.filter2D(img_gaussian, cv2.CV_64F, sobel_x)
    Iy = cv2.filter2D(img_gaussian, cv2.CV_64F, sobel_y)

    # Squared intensity differences
    Ix2 = Ix ** 2
    Iy2 = Iy ** 2
    Ixy = Ix * Iy

    # Select the appropriate weight window
    window = get_weight_window(window_size, weighting_method)

    # Calculate the structure tensor components
    M11 = cv2.filter2D(Ix2, cv2.CV_64F, window)
    M22 = cv2.filter2D(Iy2, cv2.CV_64F, window)
    M12 = cv2.filter2D(Ixy, cv2.CV_64F, window)

    # Calculate the corner response R
    det_M = M11 * M22 - M12 ** 2
    trace_M = M11 + M22
    R = det_M - k * trace_M ** 2

    # Thresholding to find corners
    corners = np.zeros_like(R)
    corners[R > threshold * R.max()] = 1

    # Apply non-maximum suppression
    corners_after_nms = non_maximum_suppression(corners)

    return corners_after_nms



# method to get the correct window based on weighting method choice
def get_weight_window(window_size, weighting_method):
    if weighting_method == "Uniform":
        return np.ones((window_size, window_size))

    elif weighting_method == "Circular":
        window_radius = window_size // 2
        y, x = np.ogrid[-window_radius:window_radius + 1, -window_radius:window_radius + 1]
        return (x ** 2 + y ** 2 <= window_radius ** 2).astype(np.float64)

    elif weighting_method == "Gaussian":
        sigma = window_size / 6  # Adjust sigma based on window size
        return gaussian_window(window_size, sigma)


def gaussian_window(size, sigma):
    x = np.arange(0, size, 1, float)
    y = x[:, np.newaxis]
    x0, y0 = size // 2, size // 2
    return np.exp(-((x - x0) ** 2 + (y - y0) ** 2) / (2 * sigma ** 2)) / (2 * np.pi * sigma ** 2)


# this is also same for whenever it is used
def non_maximum_suppression(corners):
    # Use binary dilation to find local maxima
    structure = generate_binary_structure(2, 2)
    local_maxima = maximum_filter(corners, footprint=structure) == corners

    # Apply non-maximum suppression
    suppressed_corners = corners * local_maxima

    return suppressed_corners


def select_top_corners(corners, top_k=10):
    # Get indices of the top-k corners
    indices_flat = np.argpartition(corners.ravel(), -top_k)[-top_k:]

    # Convert flat indices to 2D indices
    indices = np.unravel_index(indices_flat, corners.shape)

    # Create a binary mask for the top-k corners
    top_corners_mask = np.zeros_like(corners, dtype=np.uint8)
    top_corners_mask[indices] = 1
    return top_corners_mask


def display_top_corners(image, corners, top_k=10):
    # Get indices of the top-k corners
    indices = np.unravel_index(np.argsort(corners.ravel())[-top_k:], corners.shape)

    # Plot the original image
    plt.imshow(image, cmap='gray')
    plt.title('Original Image')

    # Plot the top-k corners on the original image
    plt.scatter(indices[1], indices[0], marker='x', color='red')
    plt.legend([])
    plt.show()


def squared_intensity_difference(image, u, v, x, y):
    intensity_difference = image[x + u, y + v].astype(np.float64) - image[x, y].astype(np.float64)
    squared_difference = intensity_difference * intensity_difference
    return squared_difference


def sum_squared_intensity_differences(image, weights, x, y):
    window_size = weights.shape[0]
    E_sum = 0

    for u in range(-window_size // 2, window_size // 2 + 1):
        for v in range(-window_size // 2, window_size // 2 + 1):
            E_sum += weights[u + window_size // 2, v + window_size // 2] * squared_intensity_difference(image, u, v, x, y)

    return E_sum


def taylor_harris_corner_detector(image, window_size=3, k=0.04):
    # Convert RGB image to grayscale
    image = cv2.cvtColor(image, cv2.COLOR_RGB2GRAY)
    height, width = image.shape

    # Compute image gradients
    Ix = cv2.Sobel(image, cv2.CV_64F, 1, 0, ksize=3)
    Iy = cv2.Sobel(image, cv2.CV_64F, 0, 1, ksize=3)

    # Compute products of gradients
    Ix2 = Ix**2
    Iy2 = Iy**2
    Ixy = Ix * Iy

    # Initialize Harris response matrix
    corners = np.zeros_like(image, dtype=np.float64)

    # Calculate windowed sums
    Sxx = cv2.boxFilter(Ix2, -1, (window_size, window_size))
    Syy = cv2.boxFilter(Iy2, -1, (window_size, window_size))
    Sxy = cv2.boxFilter(Ixy, -1, (window_size, window_size))

    # Iterate over all possible (x, y) values
    for y in range(height):
        for x in range(width):
            # Extract the windowed sums
            sum_Ix2 = Sxx[y, x]
            sum_Iy2 = Syy[y, x]
            sum_Ixy = Sxy[y, x]

            # Construct the structure tensor H
            H = np.array([[sum_Ix2, sum_Ixy],
                          [sum_Ixy, sum_Iy2]])

            # Compute the determinant and trace using matrix multiplication
            det_H = np.linalg.det(H)
            trace_H = np.trace(H)

            # Compute the Harris response for the current pixel
            corners[y, x] = det_H - k * (trace_H**2)

    return corners


image_files = os.listdir("images/")

"""# Loop through each image file
for filename in image_files:
    # Load the image using OpenCV
    image = cv2.imread(os.path.join("images/", filename))

    start_time = time.time()

    # Apply the Taylor Harris Corner Detector
    corners = taylor_harris_corner_detector(image)

    end_time = time.time()

    processing_time = end_time - start_time
    print(processing_time)

    # Plot the original image and the detected corners
    plt.figure(figsize=(8, 4))
    plt.subplot(121), plt.imshow(image, cmap='gray'), plt.title('Original Image')
    plt.subplot(122), plt.imshow(corners, cmap='jet'), plt.title('Harris Corners')
    plt.show()"""

# Loop through each image file
for filename in image_files:
    # Load the image using OpenCV
    image = cv2.imread(os.path.join("images/", filename))

    start_time = time.time()

    # Apply the Taylor Harris Corner Detector
    corners = taylor_harris_corner_detector(image)

    end_time = time.time()

    processing_time = end_time - start_time
    print(processing_time)

    # Select the top 10 corners from the corner response map
    selected_top_corners = select_top_corners(corners)

    # Display the top 10 corners on the original image
    display_top_corners(image, selected_top_corners)
# Write your code here
