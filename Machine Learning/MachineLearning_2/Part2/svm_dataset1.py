import pickle
import numpy as np
import matplotlib.pyplot as plt
from itertools import product
from sklearn.svm import SVC
from sklearn.metrics import accuracy_score


dataset, labels = pickle.load(open("../data/part2_dataset1.data", "rb"))

# The method from the recitation
def model_display_boundary(X, model, label):
    h = .01  # step size in the mesh, we can decrease this value for smooth plots, i.e 0.01 (but ploting may slow down)
    # create a mesh to plot in
    x_min, x_max = X[:, 0].min() - 3, X[:, 0].max() + 3
    y_min, y_max = X[:, 1].min() - 3, X[:, 1].max() + 3
    xx, yy = np.meshgrid(np.arange(x_min, x_max, h),
                         np.arange(y_min, y_max, h))

    aa = np.c_[xx.ravel(), yy.ravel()]
    Z = model.predict(aa)
    print(Z, set(Z))
    Z = Z.reshape(xx.shape)
    # plt.contour(xx, yy, Z, cmap=plt.cm.Paired)
    plt.contourf(xx, yy, Z, c=Z,  alpha=0.25) # cmap="Paired_r",
    # plt.contour(xx, yy, Z, colors='k', linewidths=0.7)
    plt.scatter(X[:, 0], X[:, 1], c=label, cmap="Paired_r", edgecolors='k');
    x_ = np.array([x_min, x_max])

    plt.xlim([-2, 2])
    plt.ylim([-2, 2])
    plt.show()


# Define values to try for C and kernel
C_values = [0.1, 1, 10]
kernel_values = ['rbf', 'linear', 'poly', 'sigmoid']
degrees = [1, 5]

# Create all combinations of C and kernel
configurations = list(product(C_values, kernel_values, degrees))

# Plot decision boundaries for each configuration
for config in configurations:
    C, kernel, degree = config

    # Create an instance of SVC with the current configuration
    svm = SVC(C=C, kernel=kernel, degree=degree)

    # Fit the model on the training data
    svm.fit(dataset, labels)

    # Make predictions on the test data
    predicted = svm.predict(dataset)
    print("Configuration: C:", C, "kernel:", kernel, "degree:", degree)
    print("Accuracy : ", accuracy_score(labels, predicted))

    model_display_boundary(dataset, svm, labels)