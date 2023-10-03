#ifndef PYQUARK_QUARKLAYER_H
#define PYQUARK_QUARKLAYER_H

#include <iostream>
#include <sstream>
#include <Python.h>
#include <memory>
#include <Quark/Layers/AdalineLayer.h>
#include "Helpers/ListsHelper.h"
#include <Quark/Serialization/Perceptron/PerceptronSaverLoader.h>
#include <Quark/Serialization/ModelDeserializationException.h>
#include <Quark/Functions/Function.h>
#include <Quark/Functions/Step.h>
#include "Factories/LayerFactory.h"

typedef struct
{
    PyObject_HEAD
    std::unique_ptr<Quark::Layers::Layer> Layer;
} Quark_Layer;

// Wrappers
static PyObject* Quark_Layer_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    Quark_Layer *self;
    self = (Quark_Layer *)type->tp_alloc(type, 0);
    if (self != nullptr)
    {
        self->Layer = nullptr;
    }
    return (PyObject *)self;
}

static int Quark_Layer_init(Quark_Layer *self, PyObject *args, PyObject *kwds)
{
    static char *constructorList[] = { (char*) "name", (char*)"units", (char*)"inputs", (char*)"learning_rate",  (char*)"activation_function", (char*)"sigmoid_c_param", (char*)"randomize", nullptr };

    char *name;
    int units;
    int inputDim;
    double learningRate;
    char *activationFunction;
    double sigmoidCParam;
    int randomize = 0;

    if(PyArg_ParseTupleAndKeywords(args, kwds, "siids|db", constructorList, &name, &units, &inputDim, &learningRate, &activationFunction, &sigmoidCParam, &randomize))
    {
        auto function = ActivationFunctionFactory::Create(activationFunction, sigmoidCParam);

        if(!function)
        {
            ActivationFunctionFactory::RaisePythonError();
            return -1;
        }

        self->Layer = LayerFactory::Create(name, units, inputDim, learningRate, function, randomize);

        if(!self->Layer)
        {
            PyErr_SetString(PyExc_RuntimeError, R"(Argument error: Perceptron type not recognized (use "Adaline" or "Gradient"))");
            return -1;
        }

        return 0;
    }

    PyErr_SetString(PyExc_RuntimeError, R"(Missing arguments)");

    return -1;
}

static int Quark_Layer_dealloc(Quark_Layer *self)
{
    Py_TYPE(self)->tp_free((PyObject *)self);
    return 0;
}

static PyObject* Quark_Layer_train(Quark_Layer *self, PyObject *args)
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
    auto cppLabels = PythonIterableToVector<std::vector<double>>(labels, [](PyObject* o)
    {
        if (!PyObject_IsInstance(o, (PyObject*) &PyList_Type))
        {
            PyErr_Format(PyExc_TypeError, "\rAll entries in \"sample\" should be a list of numbers.");
            return std::vector<double>();
        }
        return PythonIterableToVector<double>(o, [](PyObject* po)
        {
            return PyFloat_AsDouble(po);
        });
    });

    self->Layer->Train(cppSamples, cppLabels, [maxEpoch](int epoch, int errorCount, double errorMean) { return epoch >= maxEpoch; });
    Py_RETURN_NONE;
}

static PyObject* Quark_Layer_predict(Quark_Layer *self, PyObject *args)
{
    PyObject* inputs;
    if (!PyArg_ParseTuple(args, "O", &inputs))
    {
        PyErr_SetString(PyExc_RuntimeError, R"("Missing argument: "inputs")");
        Py_RETURN_NONE;
    }

    if (!PyObject_IsInstance(inputs, (PyObject*) &PyList_Type))
    {
        PyErr_Format(
                PyExc_TypeError, "\rAll entries in \"inputs\" should be a list of numbers."
        );
        Py_RETURN_NONE;
    }

    auto cppInputs = PythonListToVector<double>(inputs, [](PyObject* o)
    {
        return PyFloat_AsDouble(o);
    });

    auto prediction = self->Layer->Predict(cppInputs);
    return VectorToPythonList<double>(prediction, [](double v)
    {
        return PyFloat_FromDouble(v);
    });
}

static PyObject* Quark_Layer_str(Quark_Layer *self, PyObject *args)
{
    auto toReturn = "Layer";
    return PyUnicode_FromStringAndSize(toReturn, strlen(toReturn));
}

// Getters Setters
static PyObject* Quark_Layer_get_weights(Quark_Layer* self, void *closure)
{
    auto weights = self->Layer->GetWeights();
    std::vector<PyObject *> PyWeights;
    for (int i = 0; i < weights.size(); ++i)
    {
        auto weight = VectorToPythonList<double>(weights[i], [](double v) {
            return PyFloat_FromDouble(v);
        });
        PyWeights.push_back(weight);
    }
    PyObject *list = PyList_New(PyWeights.size());
    for (int i = 0; i < PyWeights.size(); ++i)
    {
        PyList_SetItem(list, i, PyWeights[i]);
    }
    return list;
}

static PyObject* Quark_Layer_get_mse(Quark_Layer* self, void *closure)
{
    auto mses = self->Layer->GetQuadraticMeans();
    auto allMses = PyList_New(0);
    for(const auto& layerMse : mses)
    {
        auto mseVector = VectorToPythonList<double>(layerMse, [](double v)
        {
            return PyFloat_FromDouble(v);
        });

        PyList_Append(allMses, mseVector);
    }

    return allMses;
}

static PyGetSetDef Quark_Layer_getsetters[] =
        {
                { "weights", (getter) Quark_Layer_get_weights, nullptr, "Weights of the layer", nullptr},
                { "mse", (getter) Quark_Layer_get_mse, nullptr, "Mse result", nullptr},
                {nullptr}
        };

// Methods
static PyMethodDef Quark_Layer_methods[] =
        {
                { "train", (PyCFunction) Quark_Layer_train, METH_VARARGS, "Train the perceptron on a given value with inputs" },
                { "predict", (PyCFunction) Quark_Layer_predict, METH_VARARGS, "Predict a value from a given inputs set" },
                { "predict", (PyCFunction) Quark_Layer_predict, METH_VARARGS, "Predict a value from a given inputs set" },
                { "__str__", (PyCFunction) Quark_Layer_str, METH_NOARGS, "String representation" },
                {nullptr}
        };

// Type
static PyTypeObject quark_Layer =
        {
                PyVarObject_HEAD_INIT(nullptr, 0)
                "quark.Layer",                                                    /* tp_name */
                sizeof(Quark_Layer),                                           /* tp_basicsize */
                0,                                                                  /* tp_itemsize */
                reinterpret_cast<destructor>(Quark_Layer_dealloc),             /* tp_dealloc */
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
                "Layer object",                                                     /* tp_doc */
                0,                                                                  /* tp_traverse */
                0,                                                                  /* tp_clear */
                0,                                                                  /* tp_richcompare */
                0,                                                                  /* tp_weaklistoffset */
                0,                                                                  /* tp_iter */
                0,                                                                  /* tp_iternext */
                Quark_Layer_methods,                                                /* tp_methods */
                nullptr, //PyCeptron_members,                                       /* tp_getset */
                Quark_Layer_getsetters,
                0,                                                                  /* tp_base */
                0,                                                                  /* tp_dict */
                0,                                                                  /* tp_descr_get */
                0,                                                                  /* tp_descr_set */
                0,                                                                  /* tp_dictoffset */
                reinterpret_cast<initproc>(Quark_Layer_init),                  /* tp_init */
                0,                                                                  /* tp_alloc */
                Quark_Layer_new,                                               /* tp_new */
        };
#endif //PYQUARK_QUARKLAYER_H
