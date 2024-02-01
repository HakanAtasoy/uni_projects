import pickle
import numpy as np
from Distance import Distance
from Part1.Knn import KNN
from sklearn.model_selection import RepeatedStratifiedKFold
from sklearn.metrics import accuracy_score



dataset, labels = pickle.load(open("../data/part1_dataset.data", "rb"))
def run_experiment(dataset, labels, distance_measure, distance_parameter, K):

    kfold = RepeatedStratifiedKFold(n_splits=10, n_repeats=5) # folds 10 and repeats 5

    # Initializing a list to store accuracy values for each fold
    accuracies = []
    for train_indices, test_indices in kfold.split(dataset, labels):
        current_train = dataset[train_indices]
        current_train_label = labels[train_indices]

        # Using our custom KNN with different configurations
        knn = KNN(current_train, current_train_label, distance_measure, distance_parameter, K)

        current_test = dataset[test_indices]
        current_test_labels = labels[test_indices]

        # using the prediction from the implementation and printing the results for each configuration
        predicted = [knn.predict(instance) for instance in current_test]
        accuracy = accuracy_score(current_test_labels, predicted)
        accuracies.append(accuracy)

    # Calculating the average accuracy after all folds and repeats
    average_accuracy = sum(accuracies) / len(accuracies)

    # Printing the average accuracy for the current configuration
    print(f"Distance Measure: {distance_measure}, K: {K}, Average Accuracy: {average_accuracy:.2f}")

# List of configurations to iterate over
distance_measures_and_their_parameters = [["minkowski", 2], ["cosine", None], ["mahalanobis", np.linalg.inv(np.cov(dataset, rowvar=False))]]
K_values = [1,2,3,4,5,6,7,8,9,10]

# Running the experiment for each configuration
for distance_measure, distance_parameter in distance_measures_and_their_parameters:
        for K_value in K_values:
            run_experiment(dataset, labels, distance_measure, distance_parameter, K_value)
