import numpy as np
import json
import copy

class KMeans:

    def __init__(self, dataset, K=2):
        """
        :param dataset: 2D numpy array, the whole dataset to be clustered
        :param K: integer, the number of clusters to form
        """
        self.K = K
        self.dataset = dataset
        # self.clusters stores the data points of each cluster in a dictionary
        self.clusters = {i: [] for i in range(K)}

        # Initializing cluster centers by choosing random indexes from the dataset
        random_indices = np.random.choice(len(dataset), size=K, replace=False)
        self.cluster_centers = {i: dataset[idx] for i, idx in enumerate(random_indices)}

    def assign_points_to_clusters(self):

        # Clearing the clusters before reassigning points
        self.clusters = {i: [] for i in range(self.K)}
        # Assign data points to the nearest cluster based on the current cluster centers.
        for i, point in enumerate(self.dataset):
            # calculating the distance between the point and each cluster center
            distances = [np.linalg.norm(point - center) for center in self.cluster_centers.values()]
            # choosing the smallest distance and adding the points index to that means cluster
            closest_cluster = np.argmin(distances)
            self.clusters[closest_cluster].append(i)

    def update_cluster_means(self):

        # using the current clusters to get the mean and replacing the old center with this new mean
        for cluster_idx, point_indices in self.clusters.items():
            if len(point_indices) > 0:
                cluster_points = self.dataset[point_indices]
                new_center = np.mean(cluster_points, axis=0) # since we are calculating the mean of each feature we consider the columns and axis is 0
                self.cluster_centers[cluster_idx] = new_center


    def calculate_loss(self):

        loss = 0.0
        for cluster_idx, point_indices in self.clusters.items():

            cluster_center = self.cluster_centers[cluster_idx]
            cluster_points = self.dataset[point_indices]

            # Sum of squared distances within each cluster ( the formula 1 in the pdf )
            cluster_loss = np.sum(np.linalg.norm(cluster_points - cluster_center, axis=1) ** 2) # since each row is a datapoint axis is 1
            loss += cluster_loss

        return loss

    def run(self):
        """Kmeans algorithm implementation"""
        prev_clusters = None
        for iteration in range(1, 100): # 100 iterations at most if the clusters don't converge for some reason
            # Assigning points to clusters based on current cluster centers
            self.assign_points_to_clusters()

            # Updating cluster centers means based on current assignment
            self.update_cluster_means()

            # Checking for convergence by comparing current clusters with previous ones
            if prev_clusters is not None and json.dumps(self.clusters, sort_keys=True) == json.dumps(prev_clusters, sort_keys=True):
                break

            # Storing current clusters for the next iteration
            prev_clusters = copy.deepcopy(self.clusters)

        return self.cluster_centers, self.clusters, self.calculate_loss()
