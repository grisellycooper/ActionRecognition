### How to run
1. Based on the datset splitting instruction on KTH webpage, split the computed optical flow features into train, dev, and test set. This also generates a file `train_keypoints.p` of all optical flow features in the train set whose format is specifically used for clustering.
```bash
$ python make_dataset.py
```
2. Run K-means on `train_keypoints.p` with 200 as the number of clusters and produce the codebook.
```bash
$ python clustering.py --dataset=data/train_keypoints.p --clusters=200
```
4. Make BoW vector for every video in the training set, using the computed clusters with TF-IDF weighting scheme.
```bash
$ python make_bow_vector.py --codebook=data/cb_200clusters.p --tfidf=1 --dataset=data/train.p --output=data/train_bow_c200.p
```
5. Train linear SVM on BoW vectors of training set.
```bash
$ python train_svm.py --dataset_bow=data/train_bow_c200.p --C=1 --output=data/svm_C1_c200.p
```
6. Use computed SVM classifier to classify videos in test set and get accuracy result.
```bash
$ python evaluate.py --svm_file=data/svm_C1_c200.p --bow_file=data/test_bow_c200.p
```

