import pickle
import numpy as np

from sklearn.svm import SVC
from sklearn.cluster import KMeans
from scipy.cluster.vq import vq


def create_dataset(dataset):
        X = []
        Y = []

        for video in dataset:
            X.append(video['features'])
            Y.append(video['category'])
        return X, Y


class HumanActivityRecognition(object):
    def __init__(self, verbose=2):
        """Initial parameters for model

        Keyword Arguments:
            verbose {int} -- Verbosity level (default: {2})
        """

        self.verbose = verbose

    def load_data(self, path_file):
        """Load CSV file generated from c++,
        and split data in three parts: train, dev, test data

        Arguments:
            path_file {str} -- Path to CSV File
        """

        self.dataset = pickle.load(open(path_file, 'rb'))
        self.dataset = self.dataset[:1000]

    def clustering(
            self, method='k-means++', clusters=200, n_init=10):
        """Clustering data set.

        Keyword Arguments:
            method {str} -- Method for initialization Kmeans
                (default: {'k-means++'})
            clusters {int} -- Number of cluster (default: {200})
            n_init {int} -- Number of time the k-means algorithm
                will be run with different centroid seeds.  (default: {10})
        """

        features = pickle.load(
            open('data/train_keypoints.p', 'rb'))

        features = features[:1000]

        self.kmeans = KMeans(
            init=method,
            n_clusters=clusters,
            n_init=n_init,
            n_jobs=2,
            verbose=self.verbose,
        )
        self.kmeans.fit(features)

    def bagofwords(self, tf_idf=True):
        clusters = self.kmeans.cluster_centers_

        n_videos = len(self.dataset)

        bow = np.zeros((n_videos, clusters.shape[0]), dtype=np.float)

        for i, video in enumerate(self.dataset):
            w_ids = vq(video['features'], clusters)[0]
            for wid in w_ids:
                bow[i, wid] += 1

        if tf_idf:
            freq = np.sum((bow > 0) * 1, axis=0)
            idf = np.log((n_videos + 1) / (freq + 1))
            bow = bow * idf

        for i in range(n_videos):
            self.dataset[i]['features'] = bow[i]


    def fit(self, C, kernel):
        X, Y = [], []
        model = SVC(C=C, kernel=kernel, verbose=self.verbose)
        model.fit(X, Y)


if __name__ == '__main__':
    model = HumanActivityRecognition(verbose=0)
    model.load_data('data/train.p')
    model.clustering()
    model.bagofwords()
