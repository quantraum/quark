#ifndef PYQUARK_COMMONPERCEPTRON_H
#define PYQUARK_COMMONPERCEPTRON_H

namespace CommonPerceptron
{
    bool CheckTrainingParams(PyObject* args, long& maxEpoch, PyObject* &samples, PyObject* &labels)
    {
        if(!PyArg_ParseTuple(args, "OO|l", &samples, &labels, &maxEpoch))
        {
            PyErr_SetString(PyExc_RuntimeError, R"(Missing arguments)");
            return false;
        }

        if (!PyObject_IsInstance(samples, (PyObject*) &PyList_Type))
        {
            PyErr_Format(PyExc_TypeError, "\rSamples should be a list.");
            return false;
        }

        if (!PyObject_IsInstance(samples, (PyObject*) &PyList_Type))
        {
            PyErr_Format(PyExc_TypeError, R"(\r\"labels\" should be a list.)");
            return false;
        }

        return true;
    }

    bool CheckFitParams(PyObject* args, PyObject* &samples, PyObject* &labels,
                        int& maxIterations, double& minQuadraticMean)
    {
        if(!PyArg_ParseTuple(args, "OO|id", &samples, &labels, &maxIterations, &minQuadraticMean))
        {
            PyErr_SetString(PyExc_RuntimeError, R"(Missing arguments)");
            return false;
        }

        if (!PyObject_IsInstance(samples, (PyObject*) &PyList_Type))
        {
            PyErr_Format(PyExc_TypeError, "\rSamples should be a list.");
            return false;
        }

        if (!PyObject_IsInstance(samples, (PyObject*) &PyList_Type))
        {
            PyErr_Format(PyExc_TypeError, R"(\r\"labels\" should be a list.)");
            return false;
        }

        return true;
    }

    PyObject* PredictOnDouble(Quark::Neurons::Perceptron<double>* perceptron, PyObject* inputs)
    {
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

        auto prediction = perceptron->Predict(cppInputs);
        return PyFloat_FromDouble(prediction);
    }

    PyObject* PredictOnInt(Quark::Neurons::Perceptron<int>* perceptron, PyObject* inputs)
    {
        if (!PyObject_IsInstance(inputs, (PyObject*) &PyList_Type))
        {
            PyErr_Format(
                    PyExc_TypeError, R"(\\rAll entries in "inputs" should be a list of numbers.)"
            );
            Py_RETURN_NONE;
        }

        auto cppInputs = PythonListToVector<int>(inputs, [](PyObject* o)
        {
            return PyLong_AsLong(o);
        });

        auto prediction = perceptron->Predict(cppInputs);
        return PyLong_FromLong(prediction);
    }
}

#endif //PYQUARK_COMMONPERCEPTRON_H
