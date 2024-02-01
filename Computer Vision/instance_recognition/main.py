import numpy as np
import cv2
import os
from color_utils import rgb_to_hsv


def custom_histogram(image, bin_count_per_channel, input_choice, grid_size = 0, type = -1):

    frequencies = None
    if input_choice == 1 or input_choice == 2:
        num_bins = bin_count_per_channel ** 3
        frequencies = np.zeros(num_bins, dtype=int)
        for row in image:
            for pixel in row:
                bin_indices = [int(pixel[i] / (256 / bin_count_per_channel)) for i in range(3)]
                combined_bin_index = bin_indices[0] * (bin_count_per_channel ** 2) + bin_indices[
                    1] * bin_count_per_channel + bin_indices[2]
                frequencies[combined_bin_index] += 1

    elif input_choice == 3 or input_choice == 4:
        frequencies = []
        for channel in range(3):
            frequencies2 = np.zeros(bin_count_per_channel, dtype=int)
            for pixel_value in image[:, :, channel].ravel():
                bin_index = int(pixel_value / (256 / bin_count_per_channel))
                frequencies2[bin_index] += 1
            frequencies.append(frequencies2)

    elif input_choice in [5, 6, 7]:
        rows = 96 // grid_size
        columns = 96 // grid_size
        frequencies = []

        if type == 0:
            for i in range(grid_size):
                for j in range(grid_size):
                    grid_block = image[i * rows: (i + 1) * rows, j * columns: (j + 1) * columns, :]
                    num_bins = bin_count_per_channel ** 3
                    frequencies2 = np.zeros(num_bins, dtype=int)
                    for sub_array in grid_block:
                        for pixel in sub_array:
                            bin_indices = [int(pixel[i] / (256 / bin_count_per_channel)) for i in range(3)]
                            combined_bin_index = bin_indices[0] * (bin_count_per_channel ** 2) + bin_indices[
                                1] * bin_count_per_channel + bin_indices[2]
                            frequencies2[combined_bin_index] += 1
                    frequencies.append(frequencies2)

        elif type == 1:
            for i in range(grid_size):
                for j in range(grid_size):
                    grid_block = image[i * rows: (i + 1) * rows, j * columns: (j + 1) * columns, :]
                    frequencies_block = []
                    for channel in range(3):
                        frequencies2 = np.zeros(bin_count_per_channel, dtype=int)
                        for pixel_value in grid_block[:, :, channel].ravel():
                            bin_index = int(pixel_value / (256 / bin_count_per_channel))
                            frequencies2[bin_index] += 1
                        frequencies_block.append(frequencies2)
                    frequencies.append(frequencies_block)

    return frequencies


def calculate_similarity(query_image, bin_count_per_channel, support_histograms, input_choice, grid_size = 0, type = -1, color_system = -1):
    if input_choice == 2 or input_choice == 4 or color_system == 1:
        query_image = rgb_to_hsv(query_image)

    hist_query = custom_histogram(query_image, bin_count_per_channel, input_choice, grid_size, type)
    similarities = []

    if input_choice == 1 or input_choice == 2:
        for hist_support in support_histograms:
            norm_hist_query = hist_query / np.sum(hist_query)
            norm_hist_support = hist_support / np.sum(hist_support)
            intersection = np.sum(np.minimum(norm_hist_query, norm_hist_support))
            similarities.append(intersection)

    elif input_choice == 3 or input_choice == 4:
        for hist_support in support_histograms:
            norm_hist_query_list = [hist_query[i] / np.sum(hist_query[i]) for i in range(3)]
            norm_hist_support_list = [hist_support[i] / np.sum(hist_support[i]) for i in range(3)]
            intersection_list = [np.sum(np.minimum(norm_hist_query_list[i], norm_hist_support_list[i])) for i in
                                 range(3)]
            intersection = np.mean(intersection_list)
            similarities.append(intersection)

    elif input_choice in [5, 6, 7]:
        for hist_support in support_histograms:
            if type == 0:
                norm_hist_query_list = [hist_query[i] / np.sum(hist_query[i]) for i in range(grid_size**2)]
                norm_hist_support_list = [hist_support[i] / np.sum(hist_support[i]) for i in range(grid_size**2)]
                intersection_list = [np.sum(np.minimum(norm_hist_query_list[i], norm_hist_support_list[i])) for i in
                                     range(grid_size**2)]
                intersection = np.mean(intersection_list)
                similarities.append(intersection)

            elif type == 1:
                norm_hist_query_list = [[hist_query[i][j] / np.sum(hist_query[i][j]) for j in range(3)] for i in
                                        range(grid_size ** 2)]
                norm_hist_support_list = [[hist_support[i][j] / np.sum(hist_support[i][j]) for j in range(3)] for i in
                                        range(grid_size ** 2)]
                intersection_list = [np.mean(
                    [np.sum(np.minimum(norm_hist_query_list[i][j], norm_hist_support_list[i][j])) for j in range(3)])
                                     for i in range(grid_size ** 2)]
                intersection = np.mean(intersection_list)
                similarities.append(intersection)

    return similarities


def match_images(query_set_images, support_set, bin_count_per_channel, support_histograms, input_choice, grid_size = 0, type = -1, color_system = -1):
    correct_matches = 0

    for query_image in query_set_images:
        similarities = calculate_similarity(query_image[0], bin_count_per_channel, support_histograms, input_choice, grid_size, type, color_system)
        retrieved_index = np.argmax(similarities)

        query_class, query_image_id = query_image[1].split('_')[0], query_image[1].split('_')[-1].split('.')[0]
        retrieved_class, retrieved_image_id = support_set[retrieved_index].split('_')[0], \
        support_set[retrieved_index].split('_')[-1].split('.')[0]

        if query_class == retrieved_class and query_image_id == retrieved_image_id:
            correct_matches += 1

    return correct_matches / len(query_set_images)

type = -1
color_system = -1
input_choice = int(input("Enter one of the pdf section numbers 1-7: "))
if input_choice in [5, 6, 7]:
    type = int(input("Enter 0 for 3d or 1 for per channel: "))
    color_system = int(input("Enter 0 for rgb or 1 for hsv: "))
bin_count_per_channel = int(input("Enter bin count per channel: "))

query_set_paths = ["query_1/", "query_2/", "query_3/"]
support_set_path = "support_96/"

query_sets = [os.listdir(query_set_path) for query_set_path in query_set_paths]
support_set = os.listdir(support_set_path)

support_set_images = [cv2.imread(os.path.join(support_set_path, filename)) for filename in support_set]

if input_choice == 2 or input_choice == 4 or color_system == 1:
    support_set_images = [rgb_to_hsv(image) for image in support_set_images]

if input_choice in [0, 1, 2, 3, 4]:
    support_histograms = [custom_histogram(support_image, bin_count_per_channel, input_choice) for support_image in
                        support_set_images]

if input_choice in [5, 6, 7]:
    query_images = [(cv2.imread(os.path.join(query_set_paths[input_choice - 5], filename)), filename) for filename in
                    query_sets[input_choice - 5]]
    for grid_size in [2, 4, 6, 8]:
        support_histograms = [custom_histogram(support_image, bin_count_per_channel, input_choice, grid_size, type) for support_image in
                              support_set_images]
        accuracy_top_1 = match_images(query_images, support_set, bin_count_per_channel, support_histograms,
                                      input_choice, grid_size, type, color_system)
        print(f'Top-1 Accuracy for Query Set {input_choice - 4}, grid_size {grid_size}: {accuracy_top_1 * 100:.2f}%')
else:
    for i, query_set_filenames in enumerate(query_sets):
        query_set_images = [(cv2.imread(os.path.join(query_set_paths[i], filename)), filename) for filename in
                            query_set_filenames]
        accuracy_top_1 = match_images(query_set_images, support_set, bin_count_per_channel, support_histograms,
                                      input_choice)
        print(f'Top-1 Accuracy for Query Set {i + 1}: {accuracy_top_1 * 100:.2f}%')
