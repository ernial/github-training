#!/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt

from sklearn.cluster import KMeans
from sklearn.datasets import make_blobs

plt.figure(figsize=(12, 12))

n_samples = 1500
random_state = 170
X, y = make_blobs(n_samples=n_samples, random_state=random_state)

# Incorrect number of clusters
# y_pred = KMeans(n_clusters=2, random_state=random_state).fit_predict(X)
# 
# plt.subplot(221)
# plt.scatter(X[:, 0], X[:, 1], c=y_pred)
# plt.title("Incorrect Number of Blobs")
# 
# plt.show()

transformation = [[0.60834549, -0.63667341], [-0.40887718, 0.85253229]]
X_aniso = np.dot(X, transformation)
y_pred = KMeans(n_clusters=3, random_state=random_state).fit_predict(X_aniso)
print(y_pred)

plt.subplot(222)
plt.scatter(X_aniso[:, 0], X_aniso[:, 1], c=y_pred)
plt.title("Anisotropicly Distributed Blobs")
# plt.show()
