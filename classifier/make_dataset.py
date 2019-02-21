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
        train = []
        dev = []
        test = []

    def run(self):
        for category in categories:
            print("Processing category %s" % category)
            category_features = pickle.load(open("data/optflow_%s.p" % category, "rb"))

            for video in category_features:
                person_id = int(video["filename"].split("_")[0][6:])

                if person_id in TRAIN_PEOPLE_ID:
                    train.append(video)

                    for frame in video["features"]:
                        train_keypoints.append(frame)

                elif person_id in DEV_PEOPLE_ID:
                    dev.append(video)
                else:
                    test.append(video)
