from Distance import Distance

class KNN:
    def __init__(self, dataset, data_label, similarity_function, similarity_function_parameters=None, K=1):
        """
        :param dataset: dataset on which KNN is executed, 2D numpy array
        :param data_label: class labels for each data sample, 1D numpy array
        :param similarity_function: similarity/distance function, Python function
        :param similarity_function_parameters: auxiliary parameter or parameter array for distance metrics
        :param K: how many neighbors to consider, integer
        """
        self.K = K
        self.dataset = dataset
        self.dataset_label = data_label
        self.similarity_function = similarity_function
        self.similarity_function_parameters = similarity_function_parameters

    def predict(self, instance):

        distances = []
        # for each training data point calculating the distance with this instance
        # and recording them in the distances array with their label
        for i, data_point in enumerate(self.dataset):
            if self.similarity_function == "minkowski":
                distance = Distance.calculateMinkowskiDistance(instance, data_point,
                                                               p=self.similarity_function_parameters)
            elif self.similarity_function == "cosine":
                distance = Distance.calculateCosineDistance(instance, data_point)

            elif self.similarity_function == "mahalanobis":
                distance = Distance.calculateMahalanobisDistance(instance, data_point,
                                                                 self.similarity_function_parameters)

            distances.append((distance, self.dataset_label[i]))

        # Sorting the distances and getting the K closest ones
        sorted_distances = sorted(distances, key=lambda x: x[0])
        k_nearest_neighbors = sorted_distances[:self.K]

        # Counting the occurrences of each class in the K nearest neighbors
        class_counts = {}
        for _, label in k_nearest_neighbors:
            if label in class_counts:
                class_counts[label] += 1
            else:
                class_counts[label] = 1

        # Returning the class with the highest count
        predicted_label = max(class_counts, key=class_counts.get)
        return predicted_label



