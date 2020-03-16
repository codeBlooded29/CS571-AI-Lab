import numpy as np
import matplotlib.pyplot as plt

# auxiliary functions
def sigmoid_function_calc(x):
    return 1 / (1 + np.exp(-x))


def sigmoid_function_derivative_calc(x):
    return x * (1 - x)


def main():
    number_of_inputLayer_neurons = 2
    number_of_outputLayer_neurons = 1
    number_of_hiddenLayer_neurons = 2

    # Initializing Random Weights for edges
    hidden_weights_array = np.random.uniform(size=(number_of_inputLayer_neurons, number_of_hiddenLayer_neurons))
    hidden_bias_array = np.random.uniform(size=(1, number_of_hiddenLayer_neurons))
    output_weights_array = np.random.uniform(size=(number_of_hiddenLayer_neurons, number_of_outputLayer_neurons))
    output_bias_array = np.random.uniform(size=(1, number_of_outputLayer_neurons))

    print("Initial hidden weights: ", end='')
    print(*hidden_weights_array)
    print("Initial hidden biases: ", end='')
    print(*hidden_bias_array)
    print("Initial output weights: ", end='')
    print(*output_weights_array)
    print("Initial output biases: ", end='')
    print(*output_bias_array)

    number_of_iterations = 50000  # iterations
    eta = 0.5  # learning rate

    input_parameters = np.array([[0, 0], [0, 1], [1, 0], [1, 1]])
    correct_outputs = np.array([[0], [1], [1], [0]])

    # Training algorithm
    i = 0
    x=[]
    y=[]
    while i < number_of_iterations:

        # Forward Propagation
        hidden_layer_total_input = np.dot(input_parameters, hidden_weights_array)
        hidden_layer_total_input = hidden_layer_total_input + hidden_bias_array
        hidden_layer_output = sigmoid_function_calc(hidden_layer_total_input)

        output_layer_total_input = np.dot(hidden_layer_output, output_weights_array)
        output_layer_total_input = output_layer_total_input + output_bias_array
        predicted_outputs = sigmoid_function_calc(output_layer_total_input)

        # I/O behaviour of hidden neuros
        # print("Input to hidden layer: ", end='')
        # print(*hidden_layer_total_input)
        # print("Output from hidden layer: ", end='')
        # print(*predicted_outputs)

        # Backpropagation
        error = correct_outputs - predicted_outputs
        y_val = 0.0
        for val in error:
            y_val += abs(val)
        y_val = y_val/(4.0)
        y.append(y_val)

        d_predicted_outputs = error * sigmoid_function_derivative_calc(predicted_outputs)

        error_hidden_layer = d_predicted_outputs.dot(output_weights_array.T)
        d_hidden_layer = error_hidden_layer * sigmoid_function_derivative_calc(hidden_layer_output)

        # Updating Weights and Biases
        output_weights_array += hidden_layer_output.T.dot(d_predicted_outputs) * eta
        output_bias_array += np.sum(d_predicted_outputs, axis=0, keepdims=True) * eta
        hidden_weights_array += input_parameters.T.dot(d_hidden_layer) * eta
        hidden_bias_array += np.sum(d_hidden_layer, axis=0, keepdims=False) * eta

        i = i + 1
        x.append(i)

    print("Final hidden weights: ", end='')
    print(*hidden_weights_array)
    print("Final hidden bias: ", end='')
    print(*hidden_bias_array)
    print("Final output weights: ", end='')
    print(*output_weights_array)
    print("Final output bias: ", end='')
    print(*output_bias_array)

    print("\nInput to neural network: ", end='')
    print(*input_parameters)
    print("Output from neural network after " + str(number_of_iterations) + " iterartions : ", end='')
    print(*predicted_outputs)
    print("\n")

    plt.plot(x,y)
    plt.ylabel('Loss')
    plt.xlabel('epoch')
    plt.title('Loss v/s epoch for LR = ' + str(eta))
    plt.show()

main()
