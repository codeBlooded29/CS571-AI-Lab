import re
import math

# reading dataset


def read_corpus(corpus_file):
    out = []
    with open(corpus_file) as f:
        i = 0
        for line in f:
            tokens = re.findall(r"[\w']+", line)
            out.append((tokens[1], tokens[3:]))
            if i > 100:
                break
            i = i + 1
    return out


all_docs = read_corpus('all_sentiment_shuffled.txt')

# testing the model (5-fold cross-validation)
avg_accuracy = 0.0
avg_precision = 0.0
avg_recall = 0.0
avg_f_score = 0.0

i = 0.2
alpha = 1
while i <= 1.0:

    # creating testing and training sets
    split_point1 = int((i-0.2) * len(all_docs))
    split_point2 = int(i * len(all_docs))

    test_docs = all_docs[split_point1:split_point2]

    train_docs = all_docs[0:split_point1] + all_docs[split_point2:]

    i = i + 0.2

    # count of positive and negative classes
    count_pos = 0
    count_neg = 0

    vocabulary = set()

    # making single docs of both positive and negative classes
    text_pos = list()
    text_neg = list()

    for a, b in train_docs:  # a = test labels, b = text

        if a == 'pos':
            count_pos = count_pos + 1
            text_pos.extend(b)

        elif a == 'neg':
            count_neg = count_neg + 1
            text_neg.extend(b)

        for token in b:
            vocabulary.add(token)

    # print(vocabulary)

    # probabilities of positive and negative classes
    prob_pos = (1.0 * count_pos + alpha) / (1.0 * len(train_docs) + 2 * alpha)
    prob_neg = (1.0 * count_neg + alpha) / (1.0 * len(train_docs) + 2 * alpha)

    # classwise probabilities of each token in vocabulary
    prob_vocabulary = dict()

    for token in vocabulary:  # finding +ve and -ve probs of all tokens

        count_pos_token = text_pos.count(token)
        pos_prob_token = (1.0 * count_pos_token + alpha) / \
            (1.0 * (len(text_pos) + alpha * len(vocabulary)))

        count_neg_token = text_neg.count(token)
        neg_prob_token = (1.0 * count_neg_token + alpha) / \
            (1.0 * (len(text_neg) + alpha * len(vocabulary)))

        prob_vocabulary[token] = (pos_prob_token, neg_prob_token)

    # testing the model
    false_positives = 0
    false_negatives = 0
    true_positives = 0
    true_negatives = 0

    right_prediction_count = 0
    for a, b in test_docs:  # a = test labels, b = text

        log_prob_pos = math.log(prob_pos)
        log_prob_neg = math.log(prob_neg)

        for token in b:  # argmax formula using log

            if token not in prob_vocabulary:
                count_pos_token = text_pos.count(token)
                pos_prob_token = (1.0 * count_pos_token + alpha) / \
                    (1.0 * (len(text_pos) + alpha * len(vocabulary)))

                count_neg_token = text_neg.count(token)
                neg_prob_token = (1.0 * count_neg_token + alpha) / \
                    (1.0 * (len(text_neg) + alpha * len(vocabulary)))

                prob_vocabulary[token] = (pos_prob_token, neg_prob_token)

            log_prob_pos += math.log(prob_vocabulary[token][0])
            log_prob_neg += math.log(prob_vocabulary[token][1])

        if log_prob_pos >= log_prob_neg:
            predicted_class = 'pos'
        else:
            predicted_class = 'neg'

        if predicted_class == a:
            right_prediction_count += 1

        if predicted_class == 'pos' and a == 'pos':
            true_positives += 1
        elif predicted_class == 'pos' and a == 'neg':
            false_positives += 1
        elif predicted_class == 'neg' and a == 'pos':
            false_negatives += 1
        else:
            true_negatives += 1

    accuracy = (1.0 * right_prediction_count)/(1.0 * len(test_docs))
    avg_accuracy += accuracy

    precision = (1.0 * true_positives) / \
        (1.0 * (true_positives + false_positives))

    avg_precision += precision

    recall = (1.0 * true_positives)/(1.0 * (true_positives + false_negatives))

    avg_recall += recall

    f_score = 2.0/((1.0/precision)+(1.0/recall))

    avg_f_score += f_score

    print('Iteration:')
    print('ACCURACY = ' + str(accuracy))
    print('PRECISION = ' + str(precision))
    print('RECALL = ' + str(recall))
    print('F-SCORE = ' + str(f_score))
    print('')


avg_accuracy = (1.0 * avg_accuracy)/5.0
avg_precision = (1.0*avg_precision)/5.0
avg_recall = (1.0 * avg_recall)/5.0
avg_f_score = (1.0 * avg_f_score)/5.0

print('AVERAGE ACCURACY = ' + str(avg_accuracy))
print('AVERAGE PRECISION = ' + str(avg_precision))
print('AVERAGE RECALL = ' + str(avg_recall))
print('AVERAGE F-SCORE = ' + str(avg_f_score))
