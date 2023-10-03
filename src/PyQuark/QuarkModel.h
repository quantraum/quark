#ifndef PYQUARK_QUARKNETWORK_H
#define PYQUARK_QUARKNETWORK_H

#include <Quark/Models/Model.h>
#include "Factories/ModelFactory.h"

typedef struct
{
    PyObject_HEAD
    std::unique_ptr<Quark::Models::Model> Model;
} Quark_Model;

static PyObject* Quark_Model_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    Quark_Model *self;
    self = (Quark_Model *)type->tp_alloc(type, 0);
    if (self != nullptr)
    {
        self->Model = nullptr;
    }
    return (PyObject *)self;
}

static int Quark_Model_init(Quark_Model *self, PyObject *args, PyObject *kwds)
{
    static char *constructorList[] = { (char*) "name", nullptr };

    char *name;

    if(PyArg_ParseTupleAndKeywords(args, kwds, "s", constructorList, &name))
    {
        self->Model = ModelFactory::Create(name);

        if(!self->Model)
        {
            PyErr_SetString(PyExc_RuntimeError, R"(Argument error: Model type not recognized (use "FeedForward"))");
            return -1;
        }

        return 0;
    }

    PyErr_SetString(PyExc_RuntimeError, R"(Missing arguments)");

    return -1;
}

static int Quark_Model_dealloc(Quark_Model *self)
{
    Py_TYPE(self)->tp_free((PyObject *)self);
    return 0;
}

static PyObject* Quark_Model_push_layer(Quark_Model *self, PyObject *args)
{
    PyObject* layer;

    if(!PyArg_ParseTuple(args, "O", &layer))
    {
        PyErr_SetString(PyExc_RuntimeError, R"("Missing arguments)");
        Py_RETURN_NONE;
    }

    if (!PyObject_TypeCheck(layer, &quark_Layer))
    {
        PyErr_Format(PyExc_TypeError, R"(\rArgument should be a "Layer".)");
        Py_RETURN_NONE;
    }

    auto cppLayer = reinterpret_cast<Quark_Layer*>(layer);

    self->Model->AddLayer(std::move(cppLayer->Layer));
    Py_RETURN_NONE;
}

static PyObject* Quark_Model_fit(Quark_Model *self, PyObject *args)
{
    PyObject* samples;
    PyObject* labels;

    int maxIterations;
    double minQuadraticMean;

    if(!CommonPerceptron::CheckFitParams(args, samples, labels, maxIterations, minQuadraticMean))
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

    self->Model->Fit(cppSamples, cppLabels, { maxIterations, minQuadraticMean });
    Py_RETURN_NONE;
}

static PyObject* Quark_Model_predict(Quark_Model *self, PyObject *args)
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

    auto prediction = self->Model->Predict(cppInputs);
    return VectorToPythonList<double>(prediction, [](double v)
    {
        return PyFloat_FromDouble(v);
    });
}

static PyObject* Quark_Model_str(Quark_Model *self, PyObject *args)
{
    auto toReturn = "Model";
    return PyUnicode_FromStringAndSize(toReturn, strlen(toReturn));
}

// Getters Setters
static PyObject* Quark_Model_get_mse(Quark_Model* self, void *closure)
{
    return VectorToPythonList<double>(self->Model->GetQuadraticMeans(), [](double v)
    {
        return PyFloat_FromDouble(v);
    });
}

static PyGetSetDef Quark_Model_getsetters[] =
        {
                { "mse", (getter) Quark_Model_get_mse, nullptr, "Mse result", nullptr},
                {nullptr}
        };

// Methods
static PyMethodDef Quark_Model_methods[] =
        {
                { "push_layer", (PyCFunction) Quark_Model_push_layer, METH_VARARGS, "Pushes a layer to the model" },
                { "fit", (PyCFunction) Quark_Model_fit, METH_VARARGS, "Train the model on a given value with inputs" },
                { "predict", (PyCFunction) Quark_Model_predict, METH_VARARGS, "Predict a value from a given inputs set" },
                { "__str__", (PyCFunction) Quark_Model_str, METH_NOARGS, "String representation" },
                {nullptr}
        };

// Type
static PyTypeObject quark_Model =
        {
                PyVarObject_HEAD_INIT(nullptr, 0)
                "quark.Model",                                                    /* tp_name */
                sizeof(Quark_Model),                                           /* tp_basicsize */
                0,                                                                  /* tp_itemsize */
                reinterpret_cast<destructor>(Quark_Model_dealloc),             /* tp_dealloc */
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
                "Model object",                                                     /* tp_doc */
                0,                                                                  /* tp_traverse */
                0,                                                                  /* tp_clear */
                0,                                                                  /* tp_richcompare */
                0,                                                                  /* tp_weaklistoffset */
                0,                                                                  /* tp_iter */
                0,                                                                  /* tp_iternext */
                Quark_Model_methods,                                                /* tp_methods */
                nullptr, //PyCeptron_members,                                       /* tp_getset */
                Quark_Model_getsetters, // getter/setters
                0,                                                                  /* tp_base */
                0,                                                                  /* tp_dict */
                0,                                                                  /* tp_descr_get */
                0,                                                                  /* tp_descr_set */
                0,                                                                  /* tp_dictoffset */
                reinterpret_cast<initproc>(Quark_Model_init),                  /* tp_init */
                0,                                                                  /* tp_alloc */
                Quark_Model_new,                                               /* tp_new */
        };
#endif //PYQUARK_QUARKNETWORK_H
