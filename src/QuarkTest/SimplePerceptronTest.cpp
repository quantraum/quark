#include <catch2/catch_test_macros.hpp>
#include "Quark/Neurons/SimplePerceptron.h"

using namespace Quark;

TEST_CASE("SimplePerceptron AND Operator")
{
    // Given
    Neurons::SimplePerceptron::Samples samples = { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 }};
    std::vector<int> labels = { 0, 0, 0, 1 };

    // When
    Neurons::SimplePerceptron perceptron(2, 1, false, std::make_shared<Functions::BinaryStep>());
    perceptron.Train(samples, labels);

    // Then
    auto predictions = perceptron.Predict(samples);
    for(int i = 0; i < predictions.size(); i++)
    {
        REQUIRE(predictions[i]  == labels[i]);
    }
}

TEST_CASE("SimplePerceptron AND Operator with randomized weights")
{
    // Given
    Neurons::SimplePerceptron::Samples samples = { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 }};
    std::vector<int> labels = { 0, 0, 0, 1 };

    // When
    Neurons::SimplePerceptron perceptron(2, 1, true, std::make_shared<Functions::BinaryStep>());
    perceptron.Train(samples, labels);

    // Then

    auto predictions = perceptron.Predict(samples);
    for(int i = 0; i < predictions.size(); i++)
    {
        REQUIRE(predictions[i]  == labels[i]);
    }

}