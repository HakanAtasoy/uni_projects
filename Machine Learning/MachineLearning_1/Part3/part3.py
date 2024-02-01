import numpy as np
import math
import random
import pickle
import matplotlib.pyplot as plt
from sklearn.cluster import AgglomerativeClustering
from scipy.cluster.hierarchy import dendrogram
from sklearn.metrics import silhouette_score, silhouette_samples




# I basically copied the related recitation files and just changed the print statements a bit
# The results in the report can just be checked by changing the K value in in the file directly at line 48

def plot_dendrogram(model, **kwargs):
    # Create linkage matrix and then plot the dendrogram

    # create the counts of samples under each node
    counts = np.zeros(model.children_.shape[0])
    n_samples = len(model.labels_)
    for i, merge in enumerate(model.children_):
        current_count = 0
        for child_idx in merge:
            if child_idx < n_samples:
                current_count += 1  # leaf node
            else:
                current_count += counts[child_idx - n_samples]
        counts[i] = current_count

    linkage_matrix = np.column_stack(
        [model.children_, model.distances_, counts]
    ).astype(float)

    # Plot the corresponding dendrogram
    dendrogram(linkage_matrix, **kwargs)


dataset = pickle.load(open("../data/part3_dataset.data", "rb"))
max_value = np.max(np.abs(dataset), axis=0)
dataset /= max_value

plt.scatter(dataset[:, 0], dataset[:, 1])
plt.show(block=True) # I made every graph in this file block so that the execution won't continue before closing them

distance_metrics = ['euclidean', 'cosine']
linkage_criteria = ['single', 'complete']
K = 2 # this value can be changed to check for different configurations

# Iterating over combinations of distance and linkage metrics and printing the results
"""
Because of the warning below I changed "affinity" to "metric" in parameters in AgglomerativeClustering.
Attribute `affinity` was deprecated in version 1.2 and will be removed in 1.4. Use `metric` instead
  warnings.warn(
"""
for distance_metric in distance_metrics:
    for linkage_criterion in linkage_criteria:
        # Create AgglomerativeClustering instance
        hac = AgglomerativeClustering(n_clusters=K, metric=distance_metric, linkage=linkage_criterion)

        # Fit and predict clusters
        predicted = hac.fit_predict(dataset)

        # Calculate silhouette score
        silhouette_avg = silhouette_score(dataset, predicted)
        sample_silhouette_values = silhouette_samples(dataset, predicted)

        # Print results for the current combination
        print(f"Distance Metric: {distance_metric}, Linkage Criterion: {linkage_criterion}")
        print("Silhouette Avg:", silhouette_avg)
        print("Clusters: ", K)
        print("Predicted Labels:", predicted)
        print("Sample Silhouette Values:", sample_silhouette_values)

        # Plotting Silhouette Values with Average Line
        # I realized this was required later on so for the first 4 screenshots this part is not included
        plt.figure(figsize=(8, 6))
        plt.barh(range(len(dataset)), sample_silhouette_values, color=plt.cm.nipy_spectral(predicted / K))
        plt.axvline(x=silhouette_avg, color="red", linestyle="--")
        plt.title(f'Silhouette Plot ({distance_metric}, {linkage_criterion})')
        plt.xlabel('Silhouette Coefficient Values')
        plt.ylabel('Sample Index')
        plt.show()

        hac = AgglomerativeClustering(distance_threshold=0, n_clusters=None, linkage=linkage_criterion, metric=distance_metric)
        hac.fit(dataset)
        plot_dendrogram(hac, truncate_mode="level", p=10)
        plt.show(block=True)
