import json
import numpy as np
import pandas as pd
import pickle


categories = [
    'boxing',
    'handclapping',
    'handwaving',
    'jogging',
    'running',
    'walking',
]

train_people_id = [11, 12, 13, 14, 15, 16, 17, 18]
dev_people_id = [19, 20, 21, 23, 24, 25, 1, 4]
test_people_id = [22, 2, 3, 5, 6, 7, 8, 9, 10]


class MakeDataset(object):
    def __init__(self):
        self.train = []
        self.dev = []
        self.test = []
        self.train_keypoints = []


    def run(self):
        for category in categories:
            data = pd.read_csv('data/haralick_%s.csv' % category, sep="|", header=None)
            data = data.values
            print('Processing category %s' % category)
            for i, row in enumerate(data):
                mat = np.array([float(d) for d in row[3][:-1].split(',')])
                size = row[2].split(',')
                size = (int(size[0]), int(size[1]))
                np.array(mat)
                mat = mat.reshape(*size)
                row[3] = mat
                category = row[0]
                filename = row[1]
                features = list(mat)
                person_id = int(filename.split('_')[0][-2:])
                obj = {
                    'features': features,
                    'category': category,
                    'filename': filename,
                }

                for i, x in enumerate(features):
                    features[i] = np.array(x )

                if person_id in train_people_id:
                    self.train.append(obj)

                    for frame in features:
                        self.train_keypoints.append(frame)

                elif person_id in dev_people_id:
                    self.dev.append(obj)
                else:
                    self.test.append(obj)

        print("Saving train/dev/test set to files")
        pickle.dump(self.train, open("data/train.p", "wb"))
        pickle.dump(self.dev, open("data/dev.p", "wb"))
        pickle.dump(self.test, open("data/test.p", "wb"))

        print("Saving keypoints in training set")
        pickle.dump(self.train_keypoints, open("data/train_keypoints.p", "wb"))


if __name__ == '__main__':
    md = MakeDataset()
    md.run()
