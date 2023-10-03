#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include <limits>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "Quark/Layers/Layer.h"
#include "Quark/Images/Image.h"
#include "Quark/Layers/AdalineLayer.h"
#include "Quark/Functions/Step.h"

using namespace Quark;

std::vector<double> ToBinarySample(const std::string& path)
{
    Image image(path);

    std::vector<double> v(image.GetWidth() * image.GetHeight());
    auto pixels = image.GetPixels();
    std::ranges::transform(pixels.begin(), pixels.end(), v.begin(), [](uint32_t pixel)
    {
        return ((pixel & 0x00'FF'FF'FF) == 0x0) ? // deleting alpha channel
               1.0 :
               0;
    });

    return v;
};

TEST_CASE("MonoLayer images batch")
{
    // Given
    std::vector<std::vector<double>> samples;
    std::vector<std::vector<double>> labels;

    for(auto c = 0; c < 4; c ++)
    {
        std::stringstream filePath;
        filePath << "./Datasets/Images/c_" << c << "_0.bmp";
        auto v = ToBinarySample(filePath.str());

        samples.emplace_back(std::move(v));

        std::vector<double> classes;
        for(auto i = 0; i < 4; i ++)
        {
            classes.emplace_back((i == c) ? 1 : -1);
        }

        labels.emplace_back(std::move(classes));
    }

    Quark::Layers::AdalineLayer layer(4, 25, 0.001, std::make_shared<Functions::Step>());

    //When
    layer.Train(samples, labels, [](int epoch, int errorCount, double errorMean)
    {
        return epoch >= 1000 || errorMean <= 0.05;
    });

    //Then
    for(auto className = 0; className < 4; className ++)
    {
        for(auto j = 0; j < 3; j ++)
        {
            std::stringstream imageFile;
            imageFile << "./Datasets/Images/c_" << className << "_" << j << ".bmp";

            auto predictions = layer.Predict(ToBinarySample(imageFile.str()));
            REQUIRE(predictions[className] > 0);

        }
    }
}

TEST_CASE("Table 3.1")
{
    // Given
    std::vector<std::vector<double>> samples = {{2.8, 1.9},
                                                {1.3, 5.6},
                                                {-1.2, 1.6},
                                                {2.9, 1.8},
                                                {2.2, 1.5},
                                                {2.5, 1.7},
                                                {3.1, 2.3},
                                                {2.8, 2.4},
                                                {-1.2, 1.5},
                                                {1.4, 6.1},
                                                {-1.2, 1.3},
                                                {3.1, 1.8},
                                                {3.3, 2.5},
                                                {3.4, 2.3},
                                                {1.5, 5.9},
                                                {1.3, 5.6},
                                                {-1.2, 1.0},
                                                {3.0, 2.1},
                                                {1.4, 5.2},
                                                {1.2, 5.5},
                                                {1.4, 6.7},
                                                {-1.3, 1.7},
                                                {1.0, 5.7},
                                                {2.6, 1.4},
                                                {1.6, 6.0},
                                                {1.2, 5.8},
                                                {1.4, 6.1},
                                                {1.3, 5.7},
                                                {-1.2, 1.4},
                                                {1.0, 5.5},
                                                {3.1, 2.1},
                                                {-1.4, 1.7},
                                                {1.6, 6.3},
                                                {-1.2, 1.4},
                                                {2.7, 1.9},
                                                {1.1, 5.5},
                                                {3.0, 1.8},
                                                {3.8, 2.0},
                                                {2.8, 2.1},
                                                {1.0, 5.0},
                                                {2.8, 2.0},
                                                {-1.2, 1.6},
                                                {1.3, 6.6},
                                                {1.6, 6.0},
                                                {3.0, 1.6},
                                                {-1.1, 1.1},
                                                {2.9, 1.8},
                                                {3.0, 1.8},
                                                {-1.2, 1.4},
                                                {1.0, 5.8},
                                                {1.2, 5.7},
                                                {2.8, 1.5},
                                                {-1.4, 1.6},
                                                {-1.2, 1.4},
                                                {2.8, 2.2},
                                                {-1.5, 1.7},
                                                {1.0, 4.9},
                                                {1.3, 5.7},
                                                {3.2, 2.3},
                                                {-1.3, 1.5},
                                                {-1.2, 1.2},
                                                {-1.2, 1.6},
                                                {3.0, 2.1},
                                                {-1.4, 1.5},
                                                {1.3, 5.5},
                                                {3.3, 2.5},
                                                {3.1, 2.4},
                                                {1.8, 5.9},
                                                {2.7, 1.9},
                                                {1.5, 6.3},
                                                {-1.3, 1.4},
                                                {1.0, 5.0},
                                                {3.2, 2.3},
                                                {-1.3, 1.4},
                                                {3.0, 2.1},
                                                {3.2, 2.0},
                                                {1.3, 6.3},
                                                {1.4, 7.0},
                                                {-1.2, 1.7},
                                                {1.4, 6.6},
                                                {1.2, 5.8},
                                                {3.0, 2.0},
                                                {-1.2, 1.6},
                                                {1.5, 6.2},
                                                {1.5, 6.4},
                                                {-1.2, 1.9},
                                                {3.2, 2.3},
                                                {-1.2, 1.5},
                                                {1.5, 5.4},
                                                {1.5, 6.0},
                                                {1.5, 6.9},
                                                {-1.4, 1.5},
                                                {1.3, 6.4},
                                                {2.8, 2.0},
                                                {-1.2, 1.5},
                                                {-1.1, 1.5},
                                                {3.6, 2.5},
                                                {1.1, 5.1},
                                                {-1.1, 1.5},
                                                {-1.2, 1.5},
                                                {3.4, 2.4},
                                                {-1.3, 1.3},
                                                {3.0, 2.3},
                                                {-1.1, 1.5},
                                                {1.1, 5.6},
                                                {1.3, 5.6},
                                                {-1.2, 1.5},
                                                {-1.2, 1.3},
                                                {1.7, 6.7},
                                                {-1.2, 1.3},
                                                {-1.4, 1.3},
                                                {2.5, 1.8},
                                                {-1.4, 1.5},
                                                {1.5, 6.5},
                                                {2.6, 2.3},
                                                {-1.2, 1.4},
                                                {1.5, 5.6},
                                                {1.3, 5.7},
                                                {1.2, 6.1},
                                                {3.0, 2.2},
                                                {3.0, 1.8},
                                                {-1.1, 1.4},
                                                {1.4, 6.8},
                                                {-1.6, 1.6},
                                                {-1.2, 1.4},
                                                {3.2, 1.8},
                                                {-1.2, 1.5},
                                                {2.7, 1.8},
                                                {-1.3, 1.4},
                                                {1.3, 6.2},
                                                {-1.3, 1.3},
                                                {3.0, 1.8},
                                                {2.7, 1.9},
                                                {-1.2, 1.4},
                                                {-1.2, 1.2},
                                                {1.5, 6.7},
                                                {2.5, 1.9},
                                                {3.3, 2.1},
                                                {2.8, 1.8},
                                                {-1.2, 1.3},
                                                {3.8, 2.2},
                                                {2.5, 2.0},
                                                {1.3, 6.1},
                                                {-1.2, 1.4},
                                                {-1.1, 1.5},
                                                {3.0, 2.3},
                                                {-1.4, 1.9},
                                                {-1.2, 1.6},
                                                {1.0, 6.0},
                                                {1.3, 5.5}};

    std::vector<std::vector<double>> labels = {{-1, -1, 1},
                                               {-1, 1, -1},
                                               {1, -1, -1},
                                               {-1, -1, 1},
                                               {-1, -1, 1},
                                               {-1, -1, 1},
                                               {-1, -1, 1},
                                               {-1, -1, 1},
                                               {1, -1, -1},
                                               {-1, 1, -1},
                                               {1, -1, -1},
                                               {-1, -1, 1},
                                               {-1, -1, 1},
                                               {-1, -1, 1},
                                               {-1, 1, -1},
                                               {-1, 1, -1},
                                               {1, -1, -1},
                                               {-1, -1, 1},
                                               {-1, 1, -1},
                                               {-1, 1, -1},
                                               {-1, 1, -1},
                                               {1, -1, -1},
                                               {-1, 1, -1},
                                               {-1, -1, 1},
                                               {-1, 1, -1},
                                               {-1, 1, -1},
                                               {-1, 1, -1},
                                               {-1, 1, -1},
                                               {1, -1, -1},
                                               {-1, 1, -1},
                                               {-1, -1, 1},
                                               {1, -1, -1},
                                               {-1, 1, -1},
                                               {1, -1, -1},
                                               {-1, -1, 1},
                                               {-1, 1, -1},
                                               {-1, -1, 1},
                                               {-1, -1, 1},
                                               {-1, -1, 1},
                                               {-1, 1, -1},
                                               {-1, -1, 1},
                                               {1, -1, -1},
                                               {-1, 1, -1},
                                               {-1, 1, -1},
                                               {-1, -1, 1},
                                               {1, -1, -1},
                                               {-1, -1, 1},
                                               {-1, -1, 1},
                                               {1, -1, -1},
                                               {-1, 1, -1},
                                               {-1, 1, -1},
                                               {-1, -1, 1},
                                               {1, -1, -1},
                                               {1, -1, -1},
                                               {-1, -1, 1},
                                               {1, -1, -1},
                                               {-1, 1, -1},
                                               {-1, 1, -1},
                                               {-1, -1, 1},
                                               {1, -1, -1},
                                               {1, -1, -1},
                                               {1, -1, -1},
                                               {-1, -1, 1},
                                               {1, -1, -1},
                                               {-1, 1, -1},
                                               {-1, -1, 1},
                                               {-1, -1, 1},
                                               {-1, 1, -1},
                                               {-1, -1, 1},
                                               {-1, 1, -1},
                                               {1, -1, -1},
                                               {-1, 1, -1},
                                               {-1, -1, 1},
                                               {1, -1, -1},
                                               {-1, -1, 1},
                                               {-1, -1, 1},
                                               {-1, 1, -1},
                                               {-1, 1, -1},
                                               {1, -1, -1},
                                               {-1, 1, -1},
                                               {-1, 1, -1},
                                               {-1, -1, 1},
                                               {1, -1, -1},
                                               {-1, 1, -1},
                                               {-1, 1, -1},
                                               {1, -1, -1},
                                               {-1, -1, 1},
                                               {1, -1, -1},
                                               {-1, 1, -1},
                                               {-1, 1, -1},
                                               {-1, 1, -1},
                                               {1, -1, -1},
                                               {-1, 1, -1},
                                               {-1, -1, 1},
                                               {1, -1, -1},
                                               {1, -1, -1},
                                               {-1, -1, 1},
                                               {-1, 1, -1},
                                               {1, -1, -1},
                                               {1, -1, -1},
                                               {-1, -1, 1},
                                               {1, -1, -1},
                                               {-1, -1, 1},
                                               {1, -1, -1},
                                               {-1, 1, -1},
                                               {-1, 1, -1},
                                               {1, -1, -1},
                                               {1, -1, -1},
                                               {-1, 1, -1},
                                               {1, -1, -1},
                                               {1, -1, -1},
                                               {-1, -1, 1},
                                               {1, -1, -1},
                                               {-1, 1, -1},
                                               {-1, -1, 1},
                                               {1, -1, -1},
                                               {-1, 1, -1},
                                               {-1, 1, -1},
                                               {-1, 1, -1},
                                               {-1, -1, 1},
                                               {-1, -1, 1},
                                               {1, -1, -1},
                                               {-1, 1, -1},
                                               {1, -1, -1},
                                               {1, -1, -1},
                                               {-1, -1, 1},
                                               {1, -1, -1},
                                               {-1, -1, 1},
                                               {1, -1, -1},
                                               {-1, 1, -1},
                                               {1, -1, -1},
                                               {-1, -1, 1},
                                               {-1, -1, 1},
                                               {1, -1, -1},
                                               {1, -1, -1},
                                               {-1, 1, -1},
                                               {-1, -1, 1},
                                               {-1, -1, 1},
                                               {-1, -1, 1},
                                               {1, -1, -1},
                                               {-1, -1, 1},
                                               {-1, -1, 1},
                                               {-1, 1, -1},
                                               {1, -1, -1},
                                               {1, -1, -1},
                                               {-1, -1, 1},
                                               {1, -1, -1},
                                               {1, -1, -1},
                                               {-1, 1, -1},
                                               {-1, 1, -1}};

    Quark::Layers::AdalineLayer layer(3, 2, 0.001, std::make_shared<Functions::Linear>());

    //When
    layer.Train(samples, labels, [](int epoch, int errorCount, double errorMean)
    {
        return epoch >= 300 || errorMean <= 0.01;
    });

    //Then
    auto predictionClass1 = layer.Predict({-1, 1});
    auto predictionClass2 = layer.Predict({1, 6});
    auto predictionClass3 = layer.Predict({3, 3});

    auto mses = layer.GetQuadraticMeans();

    REQUIRE(predictionClass1[0] > 0);
    REQUIRE(predictionClass1[1] < 0);
    REQUIRE(predictionClass1[2] < 0);

    REQUIRE(predictionClass2[0] < 0);
    REQUIRE(predictionClass2[1] > 0);
    REQUIRE(predictionClass2[2] < 0);

    REQUIRE(predictionClass3[0] < 0);
    REQUIRE(predictionClass3[1] < 0);
    REQUIRE(predictionClass3[2] > 0);

    REQUIRE_THAT(*std::min_element(std::begin(mses[0]), std::end(mses[0])), Catch::Matchers::WithinRel(0.009996, 0.001));
    REQUIRE_THAT(*std::min_element(std::begin(mses[1]), std::end(mses[1])), Catch::Matchers::WithinRel(0.013157, 0.001));
    REQUIRE_THAT(*std::min_element(std::begin(mses[2]), std::end(mses[2])), Catch::Matchers::WithinRel(0.009509, 0.001));
}
