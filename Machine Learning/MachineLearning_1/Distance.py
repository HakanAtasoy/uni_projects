import numpy as np
import math

class Distance:
    @staticmethod
    def calculateCosineDistance(x, y, TEMP=None):

        dot_product = np.dot(x, y)
        norm_x = np.linalg.norm(x)  #L2 norms of x and y vectors using linalg
        norm_y = np.linalg.norm(y)

        # Cosine distance according to pdf
        cosine_distance = 1 - dot_product / (norm_x * norm_y)

        return cosine_distance

    @staticmethod
    def calculateMinkowskiDistance(x, y, p=2):

        minkowski_distance = np.sqrt(np.sum((x - y)**2)) # squares of distances in each dimension is summed than square root is taken
        return minkowski_distance

    @staticmethod
    def calculateMahalanobisDistance(x,y, S_minus_1):

        difference = x - y
        mahalanobis_distance = np.matmul(np.transpose(difference), np.matmul(S_minus_1, difference)) # matrix multiplication formula in the pdf
        return mahalanobis_distance

