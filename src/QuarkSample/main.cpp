#include <iostream>
#include <Neurons/AdalinePerceptron.h>
#include <Neurons/GradientPerceptron.h>
#include <vector>
#include "Images/Image.h"
#include "Layers/Layer.h"
#include "Functions/Step.h"

using namespace Quark;

int main(int argc, char** argv)
{
    Neurons::GradientPerceptron::Samples samples = {
            { 1, 6 }, { 7, 9 }, { 1, 9 }, { 7, 10 }, { 2, 5 }, { 2, 7 }, { 2, 8 },
            { 6, 8 }, { 6, 9 }, { 3, 5 }, { 3, 6 }, { 3, 8 }, { 3, 9 }, { 5, 7 },
            { 5, 8 }, { 5, 10 }, { 5, 11 }, { 4, 6 }, { 4, 7 }, { 4, 9 }, { 4, 10 }
    };
    std::vector<double> labels = { 1,-1,1,-1,-1,1,1,-1,-1,-1,-1,1,1,-1,-1,1,1,-1,-1,1,1 };

    // When
    Neurons::GradientPerceptron perceptron(2, 0.2, false, std::make_shared<Functions::Step>());
    auto epochCount = perceptron.Train(samples, labels);

    auto predictions = perceptron.Predict(samples);

    std::cout << (predictions[0] < 0 ? -1 : 1) << std::endl;
    std::cout << (predictions[1] < 0 ? -1 : 1) << std::endl;
    std::cout << (predictions[2] < 0 ? -1 : 1) << std::endl;
    std::cout << (predictions[3] < 0 ? -1 : 1) << std::endl;

    return EXIT_SUCCESS;
}
