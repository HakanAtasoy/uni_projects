import numpy as np
from DataLoader import DataLoader
from sklearn.neighbors import KNeighborsClassifier
from sklearn.svm import SVC
import pickle
from sklearn.model_selection import StratifiedKFold, RepeatedStratifiedKFold, GridSearchCV, cross_val_score
from sklearn.metrics import accuracy_score, f1_score
from sklearn.pipeline import make_pipeline
from sklearn.preprocessing import MinMaxScaler
from sklearn.tree import DecisionTreeClassifier, plot_tree
from itertools import product
from sklearn.ensemble import RandomForestClassifier
import matplotlib.pyplot as plt
import pandas as pd



data_path = "../data/credit.data"

dataset, labels = DataLoader.load_credit_with_onehot(data_path)

# Hyperparameter grids
knn_parameter_grid = {"kneighborsclassifier__metric": ["cosine", "euclidean", "manhattan"],
                      "kneighborsclassifier__n_neighbors": [2, 3, 4]}

svm_parameter_grid = {"svc__C": [0.1, 1, 10],
                      "svc__kernel": ["linear", "rbf", "poly"]}

dt_parameter_grid = {"decisiontreeclassifier__max_depth": [None, 5, 10],
                     "decisiontreeclassifier__criterion": ["gini", "entropy"]}

# Cross-validation setup
outer_cross_validation = RepeatedStratifiedKFold(n_splits=3, n_repeats=5, random_state=42)
inner_cross_validation = RepeatedStratifiedKFold(n_splits=5, n_repeats=5, random_state=42)

# Performance tracking ( results for tracking all hyperparameter configuration results from the inner loop and
# best results for tracking how the best configuration does on the outer test loop)
knn_f1_results = {}
knn_accuracy_results = {}
knn_best_f1_results = {}
knn_best_accuracy_results = {}

svm_f1_results = {}
svm_accuracy_results = {}
svm_best_f1_results = {}
svm_best_accuracy_results = {}

dt_f1_results = {}
dt_accuracy_results = {}
dt_best_f1_results = {}
dt_best_accuracy_results = {}

rf_f1_results = {}
rf_accuracy_results = {}
rf_best_f1_results = {}
rf_best_accuracy_results = {}

for train_indices, test_indices in outer_cross_validation.split(dataset, labels):
    current_training_part = dataset[train_indices]
    current_training_part_label = labels[train_indices]
    current_testing_part = dataset[test_indices]
    current_testing_part_label = labels[test_indices]
    
    # For the first three methods using the template from recitation manual version 1 and modifying some things to make the hyperparameter table
    # KNN
    knn_pipeline = make_pipeline(MinMaxScaler(feature_range=(-1, 1)), KNeighborsClassifier())
    knn_grid_search = GridSearchCV(knn_pipeline, param_grid=knn_parameter_grid, refit="f1", cv=inner_cross_validation,
                                   scoring=["f1", "accuracy"])

    knn_grid_search.fit(current_training_part, current_training_part_label)

    for params, f1_mean_test_score, acc_mean_test_score in zip(knn_grid_search.cv_results_['params'],
                                                               knn_grid_search.cv_results_['mean_test_f1'],
                                                               knn_grid_search.cv_results_['mean_test_accuracy']):
        key = tuple(params.items())
        if key not in knn_f1_results:
            knn_f1_results[key] = []
        knn_f1_results[key].append(f1_mean_test_score)

        if key not in knn_accuracy_results:
            knn_accuracy_results[key] = []
        knn_accuracy_results[key].append(acc_mean_test_score)

    # Use the best hyperparameters to train the model on the entire training set
    best_knn_model = knn_grid_search.best_estimator_

    # Evaluate the trained model on the testing set for both F1 and accuracy
    test_predictions = best_knn_model.predict(current_testing_part)
    test_f1_score = f1_score(current_testing_part_label, test_predictions, average='binary')
    test_accuracy = accuracy_score(current_testing_part_label, test_predictions)

    # Record results for both F1 and accuracy
    knn_best_f1_results[str(knn_grid_search.best_params_)] = test_f1_score
    knn_best_accuracy_results[str(knn_grid_search.best_params_)] = test_accuracy

    # SVM
    svm_pipeline = make_pipeline(MinMaxScaler(feature_range=(-1, 1)), SVC())
    svm_grid_search = GridSearchCV(svm_pipeline, param_grid=svm_parameter_grid, refit="f1", cv=inner_cross_validation,
                                   scoring=["f1", "accuracy"])
    svm_grid_search.fit(current_training_part, current_training_part_label)

    for params, f1_mean_test_score, acc_mean_test_score in zip(svm_grid_search.cv_results_['params'],
                                                               svm_grid_search.cv_results_['mean_test_f1'],
                                                               svm_grid_search.cv_results_['mean_test_accuracy']):
        key = tuple(params.items())
        if key not in svm_f1_results:
            svm_f1_results[key] = []
        svm_f1_results[key].append(f1_mean_test_score)

        if key not in svm_accuracy_results:
            svm_accuracy_results[key] = []
        svm_accuracy_results[key].append(acc_mean_test_score)

    # Use the best hyperparameters to train the model on the entire training set
    best_svm_model = svm_grid_search.best_estimator_

    # Evaluate the trained model on the testing set for both F1 and accuracy
    test_predictions = best_svm_model.predict(current_testing_part)
    test_f1_score = f1_score(current_testing_part_label, test_predictions, average='binary')
    test_accuracy = accuracy_score(current_testing_part_label, test_predictions)

    # Record results for both F1 and accuracy
    svm_best_f1_results[str(svm_grid_search.best_params_)] = test_f1_score
    svm_best_accuracy_results[str(svm_grid_search.best_params_)] = test_accuracy


    # Decision Tree
    dt_pipeline = make_pipeline(MinMaxScaler(feature_range=(-1, 1)), DecisionTreeClassifier())
    dt_grid_search = GridSearchCV(dt_pipeline, param_grid=dt_parameter_grid, refit="f1", cv=inner_cross_validation,
                                  scoring=["f1", "accuracy"])
    dt_grid_search.fit(current_training_part, current_training_part_label)

    for params, f1_mean_test_score, acc_mean_test_score in zip(dt_grid_search.cv_results_['params'],
                                                               dt_grid_search.cv_results_['mean_test_f1'],
                                                               dt_grid_search.cv_results_['mean_test_accuracy']):
        key = tuple(params.items())
        if key not in dt_f1_results:
            dt_f1_results[key] = []
        dt_f1_results[key].append(f1_mean_test_score)

        if key not in dt_accuracy_results:
            dt_accuracy_results[key] = []
        dt_accuracy_results[key].append(acc_mean_test_score)

    # Use the best hyperparameters to train the model on the entire training set
    best_dt_model = dt_grid_search.best_estimator_

    # Evaluate the trained model on the testing set for both F1 and accuracy
    test_predictions = best_dt_model.predict(current_testing_part)
    test_f1_score = f1_score(current_testing_part_label, test_predictions, average='binary')
    test_accuracy = accuracy_score(current_testing_part_label, test_predictions)

    # Record results for both F1 and accuracy
    dt_best_f1_results[str(dt_grid_search.best_params_)] = test_f1_score
    dt_best_accuracy_results[str(dt_grid_search.best_params_)] = test_accuracy
    
    # Random Forest (using recitation manual version 2 and adding one more loop for repetition since it is a method with randomization)
    # Since default value 100 takes too much time I set n_estimators = 10 (it is not one of the tested hyperparameters)
    # Even then it is still a lot slower than other methods so it can be tested separately
    current_test_part = dataset[test_indices]
    current_test_part_label = labels[test_indices]

    for inner_train_indices, inner_test_indices in inner_cross_validation.split(current_training_part,
                                                                                current_training_part_label):
        inner_training_dataset = current_training_part[inner_train_indices]
        inner_training_label = current_training_part_label[inner_train_indices]

        inner_test_dataset = current_training_part[inner_test_indices]
        inner_test_label = current_training_part_label[inner_test_indices]

        inner_scalar = MinMaxScaler(feature_range=(-1, 1))
        scaled_inner_training_dataset = inner_scalar.fit_transform(inner_training_dataset)
        scaled_inner_test_dataset = inner_scalar.transform(inner_test_dataset)

        for max_depth in [5, 10]:
            for criterion in ["gini", "entropy"]:
                f1_score_sum = 0.0
                accuracy_score_sum = 0.0

                for _ in range(5):  # Repeat 5 times for each configuration
                    rf = RandomForestClassifier(n_estimators=10, criterion=criterion, max_depth=max_depth)
                    rf.fit(scaled_inner_training_dataset, inner_training_label)
                    predicted = rf.predict(scaled_inner_test_dataset)

                    # Calculate F1 score and accuracy
                    f1_score_sum += f1_score(inner_test_label, predicted, average="binary")
                    accuracy_score_sum += accuracy_score(inner_test_label, predicted)

                # Calculate average F1 score and accuracy
                f1_score_avg = f1_score_sum / 5
                accuracy_avg = accuracy_score_sum / 5

                if (max_depth, criterion) not in rf_f1_results:
                    rf_f1_results[(max_depth, criterion)] = []
                    rf_accuracy_results[(max_depth, criterion)] = []

                # Append F1 and accuracy scores
                rf_f1_results[(max_depth, criterion)].append(f1_score_avg)
                rf_accuracy_results[(max_depth, criterion)].append(accuracy_avg)

    # Find the best parameters based on F1 score
    best_parameter_rf_f1 = max(rf_f1_results, key=lambda k: np.mean(rf_f1_results[k]))
    best_score_rf_f1 = np.mean(rf_f1_results[best_parameter_rf_f1])

    # Find the best parameters based on accuracy
    best_parameter_rf_accuracy = max(rf_accuracy_results, key=lambda k: np.mean(rf_accuracy_results[k]))
    best_score_rf_accuracy = np.mean(rf_accuracy_results[best_parameter_rf_accuracy])

    # Record results for F1 and accuracy
    rf_best_f1_results[str(best_parameter_rf_f1)] = best_score_rf_f1
    rf_best_accuracy_results[str(best_parameter_rf_accuracy)] = best_score_rf_accuracy

    # Print results
    print("Best Parameters for Random Forest based on F1 Score:", best_parameter_rf_f1)
    print("Best Score for Random Forest based on F1 Score:", best_score_rf_f1)

    print("Best Parameters for Random Forest based on Accuracy:", best_parameter_rf_accuracy)
    print("Best Score for Random Forest based on Accuracy:", best_score_rf_accuracy)

    outer_scaler = MinMaxScaler(feature_range=(-1, 1))
    # Scale the entire training set for final evaluation
    scaled_current_training_part = outer_scaler.fit_transform(current_training_part)
    scaled_current_test_part = outer_scaler.transform(current_test_part)

    # Train the Random Forest model with the best parameters on the entire training set
    rf_with_best_param = RandomForestClassifier(max_depth=best_parameter_rf_f1[0], criterion=best_parameter_rf_f1[1])
    rf_with_best_param.fit(scaled_current_training_part, current_training_part_label)

    # Make predictions on the test set
    rf_predicted = rf_with_best_param.predict(scaled_current_test_part)

    # Record F1 results
    rf_best_f1_results[str(best_parameter_rf_f1)] = f1_score(current_test_part_label, rf_predicted, average="binary")
    # Record Accuracy results
    rf_best_accuracy_results[str(best_parameter_rf_accuracy)] = accuracy_score(current_test_part_label, rf_predicted)



# Print averaged and confidence interval KNN results
print("KNN Averaged Results with Confidence Intervals (F1 Score):")
for params, scores in knn_f1_results.items():
    avg_score = np.mean(scores)
    std_dev = np.std(scores)
    conf_interval = 2 * (std_dev / np.sqrt(len(scores)))  # Assuming 95% confidence interval
    print(f"Parameters: {params}, Average Mean Test F1 Score: {avg_score:.4f}, Confidence Interval: [{avg_score - conf_interval:.4f}, {avg_score + conf_interval:.4f}]")

print("KNN Averaged Results with Confidence Intervals (Accuracy):")
for params, scores in knn_accuracy_results.items():
    avg_score = np.mean(scores)
    std_dev = np.std(scores)
    conf_interval = 2 * (std_dev / np.sqrt(len(scores)))  # Assuming 95% confidence interval
    print(f"Parameters: {params}, Average Mean Test Accuracy: {avg_score:.4f}, Confidence Interval: [{avg_score - conf_interval:.4f}, {avg_score + conf_interval:.4f}]")

# Print averaged and confidence interval KNN best results
print("KNN Best Results with Confidence Intervals (F1 Score):")
for params, score in knn_best_f1_results.items():
    print(f"Parameters: {params}, Test F1 Score: {score:.4f}")

print("KNN Best Results with Confidence Intervals (Accuracy):")
for params, score in knn_best_accuracy_results.items():
    print(f"Parameters: {params}, Test Accuracy: {score:.4f}")

# Print averaged and confidence interval SVM results
print("\nSVM Averaged Results with Confidence Intervals (F1 Score):")
for params, scores in svm_f1_results.items():
    avg_score = np.mean(scores)
    std_dev = np.std(scores)
    conf_interval = 2 * (std_dev / np.sqrt(len(scores)))  # Assuming 95% confidence interval
    print(f"Parameters: {params}, Average Mean Test F1 Score: {avg_score:.4f}, Confidence Interval: [{avg_score - conf_interval:.4f}, {avg_score + conf_interval:.4f}]")

print("SVM Averaged Results with Confidence Intervals (Accuracy):")
for params, scores in svm_accuracy_results.items():
    avg_score = np.mean(scores)
    std_dev = np.std(scores)
    conf_interval = 2 * (std_dev / np.sqrt(len(scores)))  # Assuming 95% confidence interval
    print(f"Parameters: {params}, Average Mean Test Accuracy: {avg_score:.4f}, Confidence Interval: [{avg_score - conf_interval:.4f}, {avg_score + conf_interval:.4f}]")

# Print averaged and confidence interval SVM best results
print("\nSVM Best Results with Confidence Intervals (F1 Score):")
for params, score in svm_best_f1_results.items():
    print(f"Parameters: {params}, Test F1 Score: {score:.4f}")

print("SVM Best Results with Confidence Intervals (Accuracy):")
for params, score in svm_best_accuracy_results.items():
    print(f"Parameters: {params}, Test Accuracy: {score:.4f}")

# Print averaged and confidence interval Decision Tree results
print("\nDecision Tree Averaged Results with Confidence Intervals (F1 Score):")
for params, scores in dt_f1_results.items():
    avg_score = np.mean(scores)
    std_dev = np.std(scores)
    conf_interval = 2 * (std_dev / np.sqrt(len(scores)))  # Assuming 95% confidence interval
    print(f"Parameters: {params}, Average Mean Test F1 Score: {avg_score:.4f}, Confidence Interval: [{avg_score - conf_interval:.4f}, {avg_score + conf_interval:.4f}]")

print("Decision Tree Averaged Results with Confidence Intervals (Accuracy):")
for params, scores in dt_accuracy_results.items():
    avg_score = np.mean(scores)
    std_dev = np.std(scores)
    conf_interval = 2 * (std_dev / np.sqrt(len(scores)))  # Assuming 95% confidence interval
    print(f"Parameters: {params}, Average Mean Test Accuracy: {avg_score:.4f}, Confidence Interval: [{avg_score - conf_interval:.4f}, {avg_score + conf_interval:.4f}]")

# Print averaged and confidence interval Decision Tree best results
print("\nDecision Tree Best Results with Confidence Intervals (F1 Score):")
for params, score in dt_best_f1_results.items():
    print(f"Parameters: {params}, Test F1 Score: {score:.4f}")

print("Decision Tree Best Results with Confidence Intervals (Accuracy):")
for params, score in dt_best_accuracy_results.items():
    print(f"Parameters: {params}, Test Accuracy: {score:.4f}")


# Print averaged and confidence interval Random Forest results
print("\nRandom Forest Averaged Results with Confidence Intervals (F1 Score):")
for params, scores in rf_f1_results.items():
    avg_score = np.mean(scores)
    std_dev = np.std(scores)
    conf_interval = 2 * (std_dev / np.sqrt(len(scores)))  # Assuming 95% confidence interval
    print(
        f"Parameters: {params}, Average Mean Test F1 Score: {avg_score:.4f}, Confidence Interval: [{avg_score - conf_interval:.4f}, {avg_score + conf_interval:.4f}]")

print("Random Forest Averaged Results with Confidence Intervals (Accuracy):")
for params, scores in rf_accuracy_results.items():
    avg_score = np.mean(scores)
    std_dev = np.std(scores)
    conf_interval = 2 * (std_dev / np.sqrt(len(scores)))  # Assuming 95% confidence interval
    print(
        f"Parameters: {params}, Average Mean Test Accuracy: {avg_score:.4f}, Confidence Interval: [{avg_score - conf_interval:.4f}, {avg_score + conf_interval:.4f}]")

# Print averaged and confidence interval Random Forest best results
print("\nRandom Forest Best Results with Confidence Intervals (F1 Score):")
for params, score in rf_best_f1_results.items():
    print(f"Parameters: {params}, Test F1 Score: {score:.4f}")

print("Random Forest Best Results with Confidence Intervals (Accuracy):")
for params, score in rf_best_accuracy_results.items():
    print(f"Parameters: {params}, Test Accuracy: {score:.4f}")


# Load the credit application dataset without one-hot encoding
dataset2, labels2 = DataLoader.load_credit(data_path)

# Assuming dataset2 and labels2 are your features and labels

# Instantiate the classifier with a limited max_depth
clf = DecisionTreeClassifier(random_state=np.random.randint(1, 1000), max_depth=3)

# Fit the model (Using the whole dataset since we are just trying to see the important features not intereseted in testing)
clf.fit(dataset2, labels2)

# Get feature importances and indices
feature_importances = clf.feature_importances_
feature_indices = range(len(feature_importances))

# Sort feature importances and indices together
sorted_indices = sorted(feature_indices, key=lambda k: feature_importances[k], reverse=True)
sorted_features = [f"Attribute {index + 1}" for index in sorted_indices]
sorted_importances = [feature_importances[index] for index in sorted_indices]

# Report the top features and their importance scores
print("Top Features and Their Importance Scores:")
for feature, importance in zip(sorted_features, sorted_importances):
    print(f"{feature}: Importance Score = {importance:.4f}")

# Plot the decision tree with limited depth and adjusted figure size
plt.figure(figsize=(16, 10))
plot_tree(clf, filled=True, class_names=True, feature_names=sorted_features)
plt.show()


# Separate features and labels
X = np.array(dataset2, dtype=np.float32)
y = np.array(labels2, dtype=np.int32)

# Define feature names
feature_names = [
    'Status_A11', 'Status_A12', 'Status_A13', 'Status_A14',
    'Duration_in_month',
    'Credit_history_A30', 'Credit_history_A31', 'Credit_history_A32', 'Credit_history_A33', 'Credit_history_A34',
    'Purpose_A40', 'Purpose_A41', 'Purpose_A42', 'Purpose_A43', 'Purpose_A44', 'Purpose_A45', 'Purpose_A46', 'Purpose_A47', 'Purpose_A48', 'Purpose_A49', 'Purpose_A410',
    'Credit_amount',
    'Savings_A61', 'Savings_A62', 'Savings_A63', 'Savings_A64', 'Savings_A65',
    'Present_employment_since_A71', 'Present_employment_since_A72', 'Present_employment_since_A73', 'Present_employment_since_A74', 'Present_employment_since_A75',
    'Installment_rate',
    'Personal_status_and_sex_A91', 'Personal_status_and_sex_A92', 'Personal_status_and_sex_A93', 'Personal_status_and_sex_A94', 'Personal_status_and_sex_A95',
    'Other_debtors_guarantors_A101', 'Other_debtors_guarantors_A102', 'Other_debtors_guarantors_A103',
    'Present_residence_since',
    'Property_A121', 'Property_A122', 'Property_A123', 'Property_A124',
    'Age_in_years',
    'Other_installment_plans_A141', 'Other_installment_plans_A142', 'Other_installment_plans_A143',
    'Housing_A151', 'Housing_A152', 'Housing_A153',
    'Number_of_existing_credits_at_this_bank',
    'Job_A171', 'Job_A172', 'Job_A173', 'Job_A174',
    'Number_of_people_being_liable_to_provide_maintenance_for',
    'Telephone_A191', 'Telephone_A192',
    'Foreign_worker_A201', 'Foreign_worker_A202'
]


# Separate features and labels
X = np.array(dataset2, dtype=np.float32)
y = np.array(labels2, dtype=np.int32)

# Apply MinMaxScaler to scale the features
scaler = MinMaxScaler(feature_range=(-1, 1))
X_scaled = scaler.fit_transform(X)

# Instantiate and train the SVC model with MinMaxScaler
svm_model = SVC(kernel='linear')
svm_model.fit(X_scaled, y)

# Reverse MinMax scaling on support vectors
original_support_vectors = scaler.inverse_transform(svm_model.support_vectors_)

# Get support vectors for positive class (class 1)
positive_class_support_vectors = original_support_vectors[y[svm_model.support_].ravel() == 1]

# Get support vectors for negative class (class 0)
negative_class_support_vectors = original_support_vectors[y[svm_model.support_].ravel() == 0]

# Extract feature importance from SVM
feature_importance_df = pd.DataFrame({'Feature': feature_names[:X_scaled.shape[1]], 'Weight': svm_model.coef_[0]})
feature_importance_df = feature_importance_df.reindex(feature_importance_df['Weight'].abs().sort_values(ascending=False).index)

# Print or visualize the top features
print("Subset of support vectors for positive class:")
print(positive_class_support_vectors[:10])  # Print the first 10 support vectors

print("Subset of support vectors for negative class:")
print(negative_class_support_vectors[:10])  # Print the first 10 support vectors

print("\nFeature importance from SVM:")
print(feature_importance_df.head(10))