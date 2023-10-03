from quark import Perceptron, AdalinePerceptron, GradientPerceptron, Layer, Model
import sys
from typing import Optional


def create_perceptron(path: str) -> Perceptron:
    if len(path) == 0:
        print("Creating...")
        inputs_dim = 2
        learning_rate = 1.0
        randomize = False

        p = Perceptron(inputs_dim, learning_rate, randomize)

        print("Training...")
        samples = [[0, 0], [0, 1], [1, 0], [1, 1]]
        labels = [0, 0, 0, 1]

        p.train(samples, labels, 100)

        print(f"Trained perceptron: {p.__str__()}")
        return p
    else:
        print("Loading...")
        p = Perceptron.from_file(path)
        print(f"Loaded perceptron: {p.__str__()}")
        return p


if __name__ == "__main__":

    #Simple perceptron
    samples = [[0, 0], [0, 1], [1, 0], [1, 1]]
    labels = [0, 0, 0, 1]
    p = Perceptron(2, 1.0, False, "BinaryStep")

    p.train(samples, labels, 100)
    p1, p2, p3, p4 = p.predict([0, 0]), p.predict([0, 1]), p.predict([1, 0]), p.predict([1, 1])

    print(f"Weights: {p.weights}")

    print(f"""Prediction one (False, False): {("True" if p1 > 0 else "False")}""")
    print(f"""Prediction two (False, True): {("True" if p2 > 0 else "False")}""")
    print(f"""Prediction three (True, False): {("True" if p3 > 0 else "False")}""")
    print(f"""Prediction four (True, True): {("True" if p4 > 0 else "False")}""")
    #Should have "Simple" in this :
    print(f"{p.__str__()}\n")

    #Gradient Perceptron
    samples2 = [[0, 0], [0, 1], [1, 0], [1, 1]]
    labels2 = [-1, -1, -1, 1]
    gp = GradientPerceptron(2, 0.03, False, "Step")
    gp.train(samples2, labels2, 100)

    gp1, gp2, gp3, gp4 = gp.predict([0, 0]), gp.predict([0, 1]), gp.predict([1, 0]), gp.predict([1, 1])

    print(f"Weights: {gp.weights}")

    print(f"""Prediction one (False, False): {("True" if gp1 > 0 else "False")}""")
    print(f"""Prediction two (False, True): {("True" if gp2 > 0 else "False")}""")
    print(f"""Prediction three (True, False): {("True" if gp3 > 0 else "False")}""")
    print(f"""Prediction four (True, True): {("True" if gp4 > 0 else "False")}""")
    #Should have "Gradient" in this :
    print(f"{gp.__str__()}\n")

    #Adaline Perceptron
    samples3 = [[0, 0], [0, 1], [1, 0], [1, 1]]
    labels3 = [-1, -1, -1, 1]
    ap = AdalinePerceptron(2, 0.03, False, "Step")
    ap.train(samples3, labels3, 100)

    ap1, ap2, ap3, ap4 = gp.predict([0, 0]), gp.predict([0, 1]), gp.predict([1, 0]), gp.predict([1, 1])

    print(f"Weights: {gp.weights}")

    print(f"""Prediction one (False, False): {("True" if ap1 > 0 else "False")}""")
    print(f"""Prediction two (False, True): {("True" if ap2 > 0 else "False")}""")
    print(f"""Prediction three (True, False): {("True" if ap3 > 0 else "False")}""")
    print(f"""Prediction four (True, True): {("True" if ap4 > 0 else "False")}""")
    #Should have "Adaline" in this :
    print(f"{ap.__str__()}\n")

    #Monolayer
    #3.5
    layer = Layer("Adaline", 4, 25, 0.001, "Step")
    samples4 = [[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
                [0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0],
                [1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1],
                [0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0]]
    labels4 = [[1, -1, -1, -1], [-1, 1, -1, -1], [-1, -1, 1, -1], [-1, -1, -1, 1]]
    layer.train(samples4, labels4)
    lp1 = layer.predict([0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
    lp2 = layer.predict([0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0])
    lp3 = layer.predict([1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1])
    lp4 = layer.predict([0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0])

    wt = layer.weights
    print(wt[0])

    print(f"Should be 1st category: {next(i + 1 for i, c in enumerate(lp1) if c > 0)}")
    print(f"Should be 2nd category: {next(i + 1 for i, c in enumerate(lp2) if c > 0)}")
    print(f"Should be 3rd category: {next(i + 1 for i, c in enumerate(lp3) if c > 0)}")
    print(f"Should be 4th category: {next(i + 1 for i, c in enumerate(lp4) if c > 0)}")
    print(len(layer.mse))
    print(f"{layer.__str__()}\n")

    #Same principle with example 3.1 but samples and labels are 150 lines each so won't show it here.

    # Stupid feed forward
    ff_samples = [[0, 0], [0, 1], [1, 0], [1, 1]]
    ff_labels = [ [0], [1], [1], [0] ]
    ff_model = Model("IterativeFeedForward")

    ff_l1 = Layer("Gradient", 2, 2, 0.8, "Sigmoid", 1.0, True)
    ff_l2 = Layer("Gradient", 1, 2, 0.8, "Sigmoid", 1.0, True)
    ff_model.push_layer(ff_l1)
    ff_model.push_layer(ff_l2)

    ff_model.fit(ff_samples, ff_labels, 10000, 0.00001)

    int_to_bool = lambda x: "True" if x == 1 else "False"
    pred_to_bool = lambda x: "True" if x > 0.5 else "False"

    for i in range(len(ff_samples)):
        prediction = ff_model.predict(ff_samples[i])
        print(f"""{int_to_bool(ff_samples[i][0])} && {int_to_bool(ff_samples[i][1])} => {pred_to_bool(prediction[0])}""")

    print(f"{ff_model.__str__()}\n")