import numpy as np
import pickle
import matplotlib.pyplot as plt
import sklearn.datasets
from sklearn.decomposition import PCA
from sklearn.manifold import TSNE
from umap import UMAP
from sklearn.cluster import AgglomerativeClustering

color_map = {
    0: "red",1: "orange",2: "green",3: "blue",4: "purple",5: "brown",6: "pink",7: "gray",8: "cyan",9: "magenta"
}
dataset = pickle.load(open("../data/part3_dataset.data", "rb"))

mean = np.mean(dataset, axis=0)
std = np.std(dataset, axis=0)
dataset = (dataset-mean)/std

plt.scatter(dataset[:, 0], dataset[:, 1])
plt.show()

distance_metric = "euclidean"
linkage_creterion = "single"

hac = AgglomerativeClustering(n_clusters=4, metric=distance_metric, linkage=linkage_creterion)
labels = hac.fit_predict(dataset)


# method = TSNE(n_components=2, perplexity=30, metric="euclidean") # cannot transform other data instances after training
# method = PCA(n_components=2) # can transform other data instances after training
method = UMAP(n_neighbors=50, n_components=2) # can transform other data instances after training

# Printing the method and its parameters
method_name = method.__class__.__name__
method_params = method.get_params()

print(f"Method: {method_name}")
print("Parameters:")
for param, value in method_params.items():
    print(f"  {param}: {value}")


projected_data = method.fit_transform(dataset)
plt.scatter(projected_data[:, 0], projected_data[:, 1], c=[color_map[a] for a in labels])
plt.show()
