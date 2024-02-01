from Part2.Kmedoids import KMedoids
import numpy as np
import pickle
import matplotlib.pyplot as plt

def calculate_scores(dataset):

    # normalizing each feature to [0, 1] range
    max_value = np.max(np.abs(dataset), axis=0)
    dataset /= max_value

    scores = []
    for i in range(2, 11):

        min_losses = []  # To store the minimum loss for each set of 10 runs
        for _ in range(10):  # Perform 10 sets of 10 runs for each K value
            set_losses = []  # To store the loss for each run in the set
            for _ in range(10):
                kmedoids = KMedoids(dataset, i, "cosine")
                _, _, loss = kmedoids.run()
                set_losses.append(loss)  # Store the loss for each run in the set
            min_losses.append(np.min(set_losses))  # Store the first minimum for this K value

        average_min_loss = np.mean(min_losses)  # Calculate the average of the minimum losses for this K value
        scores.append(average_min_loss)
        print("K: ", i)
        print("loss: ", average_min_loss)

    return scores


# loading and showing the K-loss plots for both databases with block=True so it doesn't continue execution
# before closing the graph window
dataset1 = pickle.load(open("../data/part2_dataset_1.data", "rb"))
scores1 = calculate_scores(dataset1)
plt.plot(range(2, 11), scores1)
plt.show(block=True)


dataset2 = pickle.load(open("../data/part2_dataset_2.data", "rb"))
scores2 = calculate_scores(dataset2)
plt.plot(range(2, 11), scores2)
plt.show(block=True)
