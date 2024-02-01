import numpy as np
# In the decision tree, non-leaf nodes are going to be represented via TreeNode
class TreeNode:
    def __init__(self, attribute):
        self.attribute = attribute
        # dictionary, k: subtree, key (k) an attribute value, value is either TreeNode or TreeLeafNode
        self.subtrees = {}

# In the decision tree, leaf nodes are going to be represented via TreeLeafNode
class TreeLeafNode:
    def __init__(self, data, label):
        self.data = data
        self.labels = label

class DecisionTree:
    def __init__(self, dataset: list, labels, features, criterion="information gain"):
        """
        :param dataset: array of data instances, each data instance is represented via an Python array
        :param labels: array of the labels of the data instances
        :param features: the array that stores the name of each feature dimension
        :param criterion: depending on which criterion ("information gain" or "gain ratio") the splits are to be performed
        """
        self.dataset = dataset
        # Convert the dataset list to a numpy array
        self.dataset = np.array(dataset)
        self.labels = labels
        self.features = features
        self.criterion = criterion
        # it keeps the root node of the decision tree
        self.root = None

        # further variables and functions can be added...


    def calculate_entropy__(self, dataset, labels):
        """
        :param dataset: array of the data instances
        :param labels: array of the labels of the data instances
        :return: calculated entropy value for the given dataset
        """
        # Count the occurrences of each unique class label
        class_counts = np.unique(labels, return_counts=True)[1]

        # Calculate the probabilities of each class label
        probabilities = class_counts / len(labels)

        # Calculate the entropy using the formula: -Σ(p_i * log2(p_i))
        entropy_value = -np.sum(probabilities * np.log2(probabilities))

        # Return the calculated entropy
        return entropy_value

    def calculate_average_entropy__(self, dataset, labels, attribute):
        """
        :param dataset: array of the data instances on which an average entropy value is calculated
        :param labels: array of the labels of those data instances
        :param attribute: for which attribute an average entropy value is going to be calculated...
        :return: the calculated average entropy value for the given attribute
        """
        # Get unique values of the specified attribute in the dataset
        unique_values = np.unique(dataset[:, attribute])

        # Initialize the variable to store the average entropy
        average_entropy = 0.0

        # Iterate over each unique value of the attribute
        for value in unique_values:
            # Find indices where the attribute has the current value
            subset_indices = np.where(dataset[:, attribute] == value)[0]

            # Get the dataset composed of points that have this value for this attribute
            subset_dataset = dataset[subset_indices]

            # Extract subset labels corresponding to the current attribute value
            subset_labels = np.array(labels)[subset_indices]

            # Calculate the subset entropy using the calculate_entropy method and multiply with the probability of this value
            subset_entropy = len(subset_labels) / len(labels) * self.calculate_entropy__(subset_dataset, subset_labels)

            # Accumulate the subset entropy to compute the average later
            average_entropy += subset_entropy

        # Return the calculated average entropy
        return average_entropy

    def calculate_information_gain__(self, dataset, labels, attribute):
        """
        :param dataset: array of the data instances on which an information gain score is going to be calculated
        :param labels: array of the labels of those data instances
        :param attribute: for which attribute the information gain score is going to be calculated...
        :return: the calculated information gain score
        """
        # Calculate the total entropy of the original set of labels
        total_entropy = self.calculate_entropy__(dataset, labels)

        # Calculate the average entropy for the specified attribute
        average_entropy = self.calculate_average_entropy__(dataset, labels, attribute)

        # Calculate the information gain as the difference between total and average entropy
        information_gain = total_entropy - average_entropy

        # Return the calculated information gain
        return information_gain

    def calculate_intrinsic_information__(self, dataset, labels, attribute):
        """
        :param dataset: array of data instances on which an intrinsic information score is going to be calculated
        :param labels: array of the labels of those data instances
        :param attribute: for which attribute the intrinsic information score is going to be calculated...
        :return: the calculated intrinsic information score
        """
        # Get unique values of the specified attribute in the dataset
        unique_values = np.unique(dataset[:, attribute])

        # Initialize the variable to store the intrinsic information
        intrinsic_info = 0.0

        # Iterate over each unique value of the attribute
        for value in unique_values:
            # Find indices where the attribute has the current value
            subset_indices = np.where(dataset[:, attribute] == value)[0]

            # Calculate the probability of the subset for the current attribute value
            subset_prob = len(subset_indices) / len(labels)

            # Update the intrinsic information using the subset probability
            intrinsic_info -= subset_prob * np.log2(subset_prob)

        # Return the calculated intrinsic information
        return intrinsic_info


    def calculate_gain_ratio__(self, dataset, labels, attribute):
        """
        :param dataset: array of data instances with which a gain ratio is going to be calculated
        :param labels: array of labels of those instances
        :param attribute: for which attribute the gain ratio score is going to be calculated...
        :return: the calculated gain ratio score
        """
        # Calculate the information gain for the specified attribute
        information_gain = self.calculate_information_gain__(dataset, labels, attribute)

        # Calculate the intrinsic information for the specified attribute
        intrinsic_info = self.calculate_intrinsic_information__(dataset, labels, attribute)

        # Avoid division by zero
        if intrinsic_info == 0.0:
            return 0.0

        # Calculate the gain ratio as the ratio of information gain to intrinsic information
        gain_ratio = information_gain / intrinsic_info

        # Return the calculated gain ratio
        return gain_ratio


    def ID3__(self, dataset, labels, used_attributes):
        """
        Recursive function for ID3 algorithm
        :param dataset: data instances falling under the current  tree node
        :param labels: labels of those instances
        :param used_attributes: while recursively constructing the tree, already used labels should be stored in used_attributes
        :return: it returns a created non-leaf node or a created leaf node
        """
        # Check if all instances have the same label
        if len(np.unique(labels)) == 1:
            # If true, create a leaf node with the unique label
            return TreeLeafNode(dataset, labels)

        # Check if all attributes are used
        if len(used_attributes) == len(self.features):
            # If true, create a leaf node with the majority label
            majority_label = np.argmax(np.bincount(labels))
            return TreeLeafNode(dataset, [majority_label])

        # Choose the best attribute and add it to the used attributes
        best_attribute = self.choose_best_attribute(dataset, labels, used_attributes)
        used_attributes.append(best_attribute)

        # Create a non-leaf node with the chosen attribute as the node's attribute
        node = TreeNode(best_attribute)

        # Get unique values of the chosen attribute in the dataset
        unique_values = np.unique(dataset[:, best_attribute])

        # Iterate over each unique value of the chosen attribute
        for value in unique_values:
            # Find indices where the attribute has the current value
            subset_indices = np.where(dataset[:, best_attribute] == value)[0]

            # Extract subset data and labels based on the current attribute value
            subset_data = dataset[subset_indices]
            subset_labels = np.array(labels)[subset_indices]

            # Recursively create subtrees
            subtree = self.ID3__(subset_data, subset_labels, used_attributes.copy())

            # Assign the subtree to the current attribute value in the node's subtrees dictionary
            node.subtrees[value] = subtree

        # Return the created non-leaf node
        return node

    #Method for choosing the best attribute for a split
    def choose_best_attribute(self, dataset, labels, used_attributes):
        # Initialize variables for tracking the best attribute
        best_attribute = None
        best_score = -1

        # Iterate over each attribute
        for attribute in range(len(self.features)):
            # Check if the attribute is not already used
            if attribute not in used_attributes:
                # Calculate the score based on the specified criterion
                if self.criterion == "information gain":
                    score = self.calculate_information_gain__(dataset, labels, attribute)
                elif self.criterion == "gain ratio":
                    score = self.calculate_gain_ratio__(dataset, labels, attribute)
                else:
                    raise ValueError("Invalid criterion")

                # Update the best attribute if the current attribute has a higher score
                if score > best_score:
                    best_score = score
                    best_attribute = attribute

        # Return the index of the best attribute
        return best_attribute

    def predict(self, x):
        """
        :param x: a data instance, 1 dimensional Python array 
        :return: predicted label of x
        
        If a leaf node contains multiple labels in it, the majority label should be returned as the predicted label
        """
        return self.predict_recursive(x, self.root)

    def predict_recursive(self, x, node):
        if isinstance(node, TreeLeafNode):
            return node.labels[0]
        else:
            attribute_value = x[node.attribute]
            if attribute_value not in node.subtrees:
                # If attribute value is not in the subtree, return majority label
                return np.argmax(np.bincount(node.labels))
            else:
                # Recursively traverse the tree
                return self.predict_recursive(x, node.subtrees[attribute_value])

    def train(self):
        self.root = self.ID3__(self.dataset, self.labels, [])
        print("Training completed")