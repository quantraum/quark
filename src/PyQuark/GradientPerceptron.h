#ifndef PYQUARK_GRADIENTPERCEPTRON_H
#define PYQUARK_GRADIENTPERCEPTRON_H

#include <iostream>
#include <sstream>
#include <Python.h>
#include <memory>
#include <Quark/Neurons/GradientPerceptron.h>
#include "Helpers/ListsHelper.h"
#include "Quark/Serialization/Perceptron/PerceptronSaverLoader.h"
#include "Quark/Serialization/ModelDeserializationException.h"
#include "Quark/Functions/Step.h"
#include "Factories/ActivationFunctionFactory.h"
#include "CommonPerceptron.h"

typedef struct
{
    PyObject_HEAD
    std::unique_ptr<Quark::Neurons::GradientPerceptron> GradientPerceptron;
} Quark_GradientPerceptron;

// Wrappers
static PyObject* Quark_GradientPerceptron_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    Quark_GradientPerceptron *self;
    self = (Quark_GradientPerceptron *)type->tp_alloc(type, 0);
    if (self != nullptr)
    {
        self->GradientPerceptron = nullptr;
    }
    return (PyObject *)self;
}

static int Quark_GradientPerceptron_init(Quark_GradientPerceptron *self, PyObject *args, PyObject *kwds)
{
    static char *constructorList[] = { (char*)"inputs", (char*)"learning_rate", (char*)"randomize", (char*) "activation_function", (char*) "sigmoid_c_param", nullptr };

    int inputDim;
    double learningRate;
    int randomize;
    char *activationFunction;
    double sigmoidCParam;

    if(PyArg_ParseTupleAndKeywords(args, kwds, "idps|d", constructorList, &inputDim, &learningRate, &randomize, &activationFunction, &sigmoidCParam))
    {
        auto function = ActivationFunctionFactory::Create(activationFunction, sigmoidCParam);

        if(!function)
        {
            ActivationFunctionFactory::RaisePythonError();
            return -1;
        }

        self->GradientPerceptron = std::make_unique<Quark::Neurons::GradientPerceptron>(inputDim, learningRate, randomize, std::move(function));
        return 0;
    }

    PyErr_SetString(PyExc_RuntimeError, R"("Missing arguments)");

    return -1;
}

/*
static PyObject* Quark_GradientPerceptron_from_file(PyTypeObject* type, PyObject *args, PyObject *kwds)
{
    const char* path;
    Quark_GradientPerceptron* self;

    static char *argumentsList[] = { (char*)"path", nullptr };
    if(PyArg_ParseTupleAndKeywords(args, kwds, "s", argumentsList, &path))
    {
        self = (Quark_GradientPerceptron *)type->tp_alloc(type, 0);

        if(!self)
        {
            Py_RETURN_NONE;
        }

        self->GradientPerceptron = std::make_unique<Quark::Neurons::GradientPerceptron>(0, 0.0);
        Quark::PerceptronSaverLoader loader;

        try
        {
            loader.Load(path, self->GradientPerceptron.get());
        }
        catch(Quark::ModelDeserializationException& e)
        {
            type->tp_dealloc((PyObject*)self);
            PyErr_Format(
                    PyExc_FileNotFoundError, e.what()
            );
            Py_RETURN_NONE;
        }

        return (PyObject*) self;
    }

    PyErr_SetString(PyExc_RuntimeError, R"("Missing arguments)");
    Py_RETURN_NONE;
}*/

static int Quark_GradientPerceptron_dealloc(Quark_GradientPerceptron *self)
{
    Py_TYPE(self)->tp_free((PyObject *)self);
    return 0;
}
/*
static PyObject* Quark_GradientPerceptron_save(Quark_GradientPerceptron *self, PyObject *args)
{
    const char* path;
    if(!PyArg_ParseTuple(args, "s", &path))
        Py_RETURN_NONE;

    Quark::PerceptronSaverLoader saverLoader;

    saverLoader.Save(path, *self->GradientPerceptron);
    Py_RETURN_NONE;
}
*/
static PyObject* Quark_GradientPerceptron_train(Quark_GradientPerceptron *self, PyObject *args)
{
    PyObject* samples;
    PyObject* labels;
    long maxEpoch = 1000;

    if(!CommonPerceptron::CheckTrainingParams(args, maxEpoch, samples, labels))
    {
        Py_RETURN_NONE;
    }

    // Samples
    auto cppSamples = PythonIterableToVector<std::vector<double>>(samples, [](PyObject* o)
    {
        if (!PyObject_IsInstance(o, (PyObject*) &PyList_Type))
        {
            PyErr_Format(PyExc_TypeError, R"(\rAll entries in "sample" should be a list of numbers.)");
            return std::vector<double>();
        }
        return PythonIterableToVector<double>(o, [](PyObject* po)
        {
            return PyFloat_AsDouble(po);
        });
    });

    // Labels
    auto cppLabels = PythonIterableToVector<double>(labels, [](PyObject* o)
    {
        return (int)PyFloat_AsDouble(o);
    });

    self->GradientPerceptron->Train(cppSamples, cppLabels, [maxEpoch](int epoch, int errorCount, double errorMean) { return epoch >= maxEpoch; });
    Py_RETURN_NONE;
}


static PyObject* Quark_GradientPerceptron_predict(Quark_GradientPerceptron *self, PyObject *args)
{
    PyObject* inputs;
    if (!PyArg_ParseTuple(args, "O", &inputs))
    {
        PyErr_SetString(PyExc_RuntimeError, R"("Missing argument: "inputs")");
        Py_RETURN_NONE;
    }

    return CommonPerceptron::PredictOnDouble(self->GradientPerceptron.get(), inputs);
}

static PyObject* Quark_GradientPerceptron_str(Quark_GradientPerceptron *self, PyObject *args)
{
    auto weights = self->GradientPerceptron->GetWeights();
    auto weightsSize = weights.size();
    std::stringstream formulaBuilder;

    formulaBuilder << "1 * " << weights[0];

    for(auto i = 1; i < weightsSize; i ++)
    {
        formulaBuilder << (weights[i] < 0 ? " - " : " + ");
        formulaBuilder << weights[i] << " *x" << i;
    }

    auto formula = formulaBuilder.str();
    return PyUnicode_FromStringAndSize(formula.c_str(), formula.length());
}

// Getters setters
static PyObject* Quark_GradientPerceptron_get_weights(Quark_GradientPerceptron* self, void *closure)
{
    auto weights = self->GradientPerceptron->GetWeights();
    return VectorToPythonList<double>(weights, [](double v)
    {
        return PyFloat_FromDouble(v);
    });
}

static PyObject* Quark_GradientPerceptron_get_mse(Quark_GradientPerceptron* self, void *closure)
{
    return VectorToPythonList<double>(self->GradientPerceptron->GetQuadraticMeans(), [](double v)
    {
        return PyFloat_FromDouble(v);
    });
}

static PyGetSetDef Quark_GradientPerceptron_getsetters[] =
        {
                { "weights", (getter) Quark_GradientPerceptron_get_weights, nullptr, "Weights of the perceptron model", nullptr},
                { "mse", (getter) Quark_GradientPerceptron_get_mse, nullptr, "Mse result", nullptr},
                {nullptr}
        };

// Methods
static PyMethodDef Quark_GradientPerceptron_methods[] =
        {
                //{ "save", (PyCFunction)Quark_GradientPerceptron_save, METH_VARARGS, "Saves the model to a specified path" },
                { "train", (PyCFunction)Quark_GradientPerceptron_train, METH_VARARGS, "Train the perceptron on a given value with inputs" },
                { "predict", (PyCFunction) Quark_GradientPerceptron_predict, METH_VARARGS, "Predict a value from a given inputs set" },
                { "predict", (PyCFunction) Quark_GradientPerceptron_predict, METH_VARARGS, "Predict a value from a given inputs set" },
                { "__str__", (PyCFunction) Quark_GradientPerceptron_str, METH_NOARGS, "String representation" },
                // Static
                //{ "from_file", (PyCFunction) Quark_GradientPerceptron_from_file, METH_VARARGS | METH_CLASS, "Load a perceptron model from a file" },
                {nullptr}
        };

// Type
static PyTypeObject quark_GradientPerceptron =
        {
                PyVarObject_HEAD_INIT(nullptr, 0)
                "quark.GradientPerceptron",                                                    /* tp_name */
                sizeof(Quark_GradientPerceptron),                                           /* tp_basicsize */
                0,                                                                  /* tp_itemsize */
                reinterpret_cast<destructor>(Quark_GradientPerceptron_dealloc),             /* tp_dealloc */
                0,                                                                  /* tp_print */
                0,                                                                  /* tp_getattr */
                0,                                                                  /* tp_setattr */
                0,                                                                  /* tp_reserved */
                0,                                                                  /* tp_repr */
                0,                                                                  /* tp_as_number */
                0,                                                                  /* tp_as_sequence */
                0,                                                                  /* tp_as_mapping */
                0,                                                                  /* tp_hash  */
                0,                                                                  /* tp_call */
                0,                                                                  /* tp_str */
                0,                                                                  /* tp_getattro */
                0,                                                                  /* tp_setattro */
                0,                                                                  /* tp_as_buffer */
                Py_TPFLAGS_DEFAULT,                                                 /* tp_flags */
                "Gradient Perceptron object",                                                  /* tp_doc */
                0,                                                                  /* tp_traverse */
                0,                                                                  /* tp_clear */
                0,                                                                  /* tp_richcompare */
                0,                                                                  /* tp_weaklistoffset */
                0,                                                                  /* tp_iter */
                0,                                                                  /* tp_iternext */
                Quark_GradientPerceptron_methods,                                           /* tp_methods */
                nullptr, //PyCeptron_members,                                       /* tp_members */
                Quark_GradientPerceptron_getsetters,                                        /* tp_getset */
                0,                                                                  /* tp_base */
                0,                                                                  /* tp_dict */
                0,                                                                  /* tp_descr_get */
                0,                                                                  /* tp_descr_set */
                0,                                                                  /* tp_dictoffset */
                reinterpret_cast<initproc>(Quark_GradientPerceptron_init),                  /* tp_init */
                0,                                                                  /* tp_alloc */
                Quark_GradientPerceptron_new,                                               /* tp_new */
        };
#endif //PYQUARK_GRADIENTPERCEPTRON_H
