import codecs
import re
from operator import itemgetter

from nltk.corpus import stopwords
from sklearn import metrics
from sklearn.model_selection import cross_val_score, train_test_split

train_docs = []
bag_of_words = {}
vocabulary = []
feature_index = {}
X = []
Y = []
category_to_consider = ['ABBR', 'ENTY', 'DESC', 'HUM', 'LOC', 'NUM']
pos_tags_list = ['CC', 'CD', 'DT', 'EX', 'FW', 'IN', 'JJ', 'JJR', 'JJS', 'LS', 'MD', 'NN', 'NNS', 'NNP', 'NNPS', 'PDT',
                 'POS', 'PRP', 'PRP$', 'RB', 'RBR', 'RBS', 'RP', 'SYM', 'TO', 'UH', 'VB', 'VBD', 'VBG', 'VBN', 'VBP',
                 'VBZ', 'WDT', 'WP', 'WP$', 'WRB']

import nltk

nltk.download('punkt')
nltk.download('averaged_perceptron_tagger')


def preprocess_nltk(question):
    stopword = set(stopwords.words("english"))
    y = ""
    for word in question.split():
        if word not in stopword:
            y += word + " "
    return y


def run_decision_tree():
    from sklearn.tree import DecisionTreeClassifier
    classifier = DecisionTreeClassifier(max_depth = 20)
    score = cross_val_score(classifier, X, Y, scoring = "accuracy", cv = 12)
    print(sum(score) / len(score))


def read_corpus(corpus_file):
    out = []
    with codecs.open(corpus_file, 'r', encoding = 'utf-8',
                     errors = 'ignore') as f:
        i = 0
        for line in f:
            tokens = re.findall(r"[\w']+", line)
            out.append([tokens[0], tokens[1], tokens[2:]])
    return out


def main():
    train_docs = read_corpus('train_5500.label.txt')

    test_docs = read_corpus('TREC_10.label.txt')
    print(test_docs[0])

    y = [train_docs[i][0] for i in range(0, len(train_docs))]
    X = [train_docs[i][2] for i in range(0, len(train_docs))]

    from sklearn.feature_extraction.text import CountVectorizer

    count_vect = CountVectorizer()

    delimiter = ' '

    X_joined = [delimiter.join(X[i]) for i in range(0, len(X))]
    X_counts = count_vect.fit_transform(X_joined)
    X_train, X_test, y_train, y_test = train_test_split(X_counts, y, test_size = 0.33, random_state = 42)

    from sklearn.tree import DecisionTreeClassifier
    clf = DecisionTreeClassifier()

    # Train Decision Tree Classifer
    clf = clf.fit(X_train, y_train)

    # Predict the response for test dataset
    y_pred = clf.predict(X_test)

    # Model Accuracy, how often is the classifier correct?
    print("Accuracy:", metrics.accuracy_score(y_test, y_pred))


def n_gram_populations(ngram, count):
    for coarse_class, fine_class, question in train_docs:
        question = preprocess_nltk(question)
        vocabs = question.split(" ")
        for i in range(len(vocabs) - ngram + 1):
            string = ""
            for j in range(ngram):
                string += vocabs[i + j]
                string += " "
            if string.strip() not in bag_of_words:
                bag_of_words[string.strip()] = 1
            else:
                bag_of_words[string.strip()] += 1
    tot = 0
    for key, value in sorted(bag_of_words.items(), key = itemgetter(1), reverse = True):
        tot += 1
        vocabulary.append(key)
        if tot == count:
            break
            # print(feature_vector)
    bag_of_words.clear()


if __name__ == '__main__':
    main()
