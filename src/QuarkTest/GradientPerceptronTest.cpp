#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include "Quark/Neurons/GradientPerceptron.h"
#include "Quark/Functions/Step.h"

using namespace Quark;

TEST_CASE("GradientPerceptron And Operator")
{
    // Given
    Neurons::GradientPerceptron::Samples samples = { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 }};
    std::vector<double> labels = { -1, -1, -1, 1 };

    // When
    Neurons::GradientPerceptron perceptron(2, 0.2, false, std::make_shared<Functions::Step>());
    perceptron.Train(samples, labels,[](int epoch, int errorCount, double errorMean)
    {
        return epoch >= 10000 || errorMean < 0.1250013;
    });

    // Then
    auto predictions = perceptron.Predict(samples);
    for(int i = 0; i < predictions.size(); i++)
    {
        REQUIRE(predictions[i]  == labels[i]);
    }
}

TEST_CASE("GradientPerceptron And Operator with randomized weights")
{
    // Given
    Neurons::GradientPerceptron::Samples samples = { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 }};
    std::vector<double> labels = { -1, -1, -1, 1 };

    // When
    Neurons::GradientPerceptron perceptron(2, 0.2, true, std::make_shared<Functions::Step>());
    perceptron.Train(samples, labels);

    // Then
    auto predictions = perceptron.Predict(samples);
    for(int i = 0; i < predictions.size(); i++)
    {
        REQUIRE(predictions[i]  == labels[i]);
    }
}

TEST_CASE("GradientPerceptron OR Operator")
{
    // Given
    Neurons::GradientPerceptron::Samples samples = { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 }};
    std::vector<double> labels = { -1, 1, 1, 1 };

    // When
    Neurons::GradientPerceptron perceptron(2, 0.2, false, std::make_shared<Functions::Step>());
    perceptron.Train(samples, labels);

    // Then
    auto predictions = perceptron.Predict(samples);
    for(int i = 0; i < predictions.size(); i++)
    {
        REQUIRE(predictions[i]  == labels[i]);
    }
}


TEST_CASE("GradientPerceptron Table 2.9 Light")
{    // Given
    Neurons::GradientPerceptron::Samples samples = {{ 1, 6 }, { 7, 9 }, { 1, 9 }, { 7, 10 }};
    std::vector<double> labels = { 1,-1,1,-1 };

    // When
    Neurons::GradientPerceptron perceptron(2, 0.0011, false, std::make_shared<Functions::Step>());
    perceptron.Train(samples, labels, [](int epoch, int errorCount, double errorMean)
    {
        return epoch >= 1000 || errorCount == 0;
    });

    // Then
    auto predictions = perceptron.Predict(samples);
    for(int i = 0; i < predictions.size(); i++)
    {
        REQUIRE(predictions[i]  == labels[i]);
    }
}

TEST_CASE("GradientPerceptron Table 2.9")
{    // Given
    Neurons::GradientPerceptron::Samples samples =
    {
            { 1, 6 }, { 7, 9 }, { 1, 9 }, { 7, 10 }, { 2, 5 }, { 2, 7 }, { 2, 8 },
            { 6, 8 }, { 6, 9 }, { 3, 5 }, { 3, 6 }, { 3, 8 }, { 3, 9 }, { 5, 7 },
            { 5, 8 }, { 5, 10 }, { 5, 11 }, { 4, 6 }, { 4, 7 }, { 4, 9 }, { 4, 10 }
    };
    std::vector<double> labels = { 1,-1,1,-1,-1,1,1,-1,-1,-1,-1,1,1,-1,-1,1,1,-1,-1,1,1 };

    // When
    Neurons::GradientPerceptron perceptron(2, 0.0011, false, std::make_shared<Functions::Step>());
    perceptron.Train(samples, labels, [](int epoch, int errorCount, double errorMean)
    {
        return epoch >= 1000 || errorCount == 0;
    });

    // Then
    auto predictions = perceptron.Predict(samples);
    for(int i = 0; i < predictions.size(); i++)
    {
        REQUIRE(predictions[i]  == labels[i]);
    }
}