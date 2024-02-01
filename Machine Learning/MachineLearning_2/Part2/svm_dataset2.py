import pickle
import numpy as np
from sklearn.model_selection import RepeatedStratifiedKFold, GridSearchCV
from sklearn.svm import SVC
from sklearn.pipeline import make_pipeline
from sklearn.preprocessing import StandardScaler
from scipy.stats import t

dataset, labels = pickle.load(open("../data/part2_dataset2.data", "rb"))

# Create RepeatedStratifiedKFold
cv = RepeatedStratifiedKFold(n_splits=10, n_repeats=5, random_state=11)

# Define parameter grid
param_grid = {'svc__C': [0.1, 1, 10], 'svc__kernel': ['linear', 'rbf', 'poly']}

# Create a pipeline with StandardScaler and SVM
pipeline = make_pipeline(StandardScaler(), SVC())

# Initialize GridSearchCV with the pipeline
grid_search = GridSearchCV(pipeline, param_grid, scoring='accuracy', cv=cv)

# Fit the model to the data (X, y)
grid_search.fit(dataset, labels)

# Access the entire cv_results dictionary
cv_results = grid_search.cv_results_

# Print all results with 95% confidence intervals
print("All Results:")
for mean_score, std_score, params in zip(
    cv_results['mean_test_score'],
    cv_results['std_test_score'],
    cv_results['params']
):
    all_fold_scores = []
    for split_idx in range(cv.get_n_splits()):
        for repeat_idx in range(cv.n_repeats):
            fold_score_key = f"split{split_idx}_test_score"
            repeat_fold_scores = cv_results[fold_score_key][repeat_idx::cv.n_repeats]
            all_fold_scores.extend(repeat_fold_scores)

    n_samples = len(all_fold_scores)

    confidence_interval = t.interval(
        0.95,  # Confidence level
        df=n_samples - 1,  # Degrees of freedom
        loc=mean_score,
        scale=std_score / np.sqrt(n_samples)
    )
    print(f"Mean Test Score: {mean_score:.4f} with 95% Confidence Interval: ({confidence_interval[0]:.4f}, {confidence_interval[1]:.4f}) for {params}")
