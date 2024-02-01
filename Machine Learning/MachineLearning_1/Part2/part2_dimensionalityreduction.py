import pickle
import numpy as np
import matplotlib.pyplot as plt
import sklearn.datasets
from sklearn.decomposition import PCA
from sklearn.manifold import TSNE
from umap import UMAP
from Part2.Kmeans import KMeans
from Part2.Kmedoids import KMedoids

dataset1 = pickle.load(open("../data/part2_dataset_1.data", "rb"))
dataset2 = pickle.load(open("../data/part2_dataset_2.data", "rb"))
color_map = {
    0: "red",1: "orange",2: "green",3: "blue",4: "purple",5: "brown",6: "pink",7: "gray",8: "cyan",9: "magenta"
}


def test_dataset(dataset, kmeans_or_medoids = 0, K= 2):
    mean = np.mean(dataset, axis=0)
    std = np.std(dataset, axis=0)
    dataset = (dataset-mean)/std
    clustering_algorithm = None # used later on to use the cluster for dimensionality reduction whether its kmeans or kmedoids
    # if kmeans_or_medoids is 0 it uses Kmeans to form the cluster and uses Kmedoids if it is 1
    if kmeans_or_medoids == 0:
        min_loss = float('inf')  # Initializing with positive infinity
        for _ in range(10): # 10 runs for each K value
            kmeans = KMeans(dataset, K)
            _, _, loss = kmeans.run()
            if loss < min_loss:
                clustering_algorithm = kmeans
                min_loss = loss

    elif kmeans_or_medoids == 1:
        min_loss = float('inf')  # Initializing with positive infinity
        for _ in range(10): # 10 runs for each K value
            kmedoids = KMedoids(dataset, K, "cosine")
            _, _, loss = kmedoids.run()
            if loss < min_loss:
                clustering_algorithm = kmedoids
                min_loss = loss


    method = TSNE(n_components=2, perplexity=30, metric="euclidean") # cannot transform other data instances after training
    #method = PCA(n_components=2) # can transform other data instances after training
    #method = UMAP(n_neighbors=100, n_components=2) # can transform other data instances after training

    projected_data = method.fit_transform(dataset)

    for cluster_index, data_point_indices in clustering_algorithm.clusters.items():
        cluster_color = color_map[cluster_index]
        plt.scatter(projected_data[data_point_indices, 0], projected_data[data_point_indices, 1], c=cluster_color,
                    label=f'Cluster {cluster_index}')

    plt.legend()
    plt.show()
    print(dataset.shape)

test_dataset(dataset1, K = 5, kmeans_or_medoids=1)
#test_dataset(dataset2, K = 3, kmeans_or_medoids= 1)

