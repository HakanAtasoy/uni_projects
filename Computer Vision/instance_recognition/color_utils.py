import numpy as np

def rgb_to_hsv(rgb_array):
    # Normalize RGB values to [0, 1]
    normalized_rgb = rgb_array / 255.0

    # Separate RGB channels
    r, g, b = normalized_rgb[:, :, 0], normalized_rgb[:, :, 1], normalized_rgb[:, :, 2]

    # Compute value (V)
    v = np.max(normalized_rgb, axis=2)

    # Compute saturation (S)
    s = np.divide((v - np.min(normalized_rgb, axis=2)), v, out=np.zeros_like(v), where=(v != 0))

    cmax = np.max(normalized_rgb, axis=2)
    cmin = np.min(normalized_rgb, axis=2)

    delta = cmax - cmin

    # Calculate H based on the value of cmax
    h = np.select(
        [delta == 0, cmax == r, cmax == g, cmax == b],
        [0, ((g - b) / delta % 6)/6, ((b - r) / delta + 2)/6, ((r - g) / delta + 4)/6]
    )

    h = (h * 255).astype(np.uint8)
    s = (s * 255).astype(np.uint8)
    v = (v * 255).astype(np.uint8)
    # Stack channels to get the final HSV image
    hsv_image = np.stack([h, s, v], axis=-1)

    return hsv_image
