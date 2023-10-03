// Types
#include "ITPerceptron.h"
#include "GradientPerceptron.h"
#include "AdalinePerceptron.h"
#include "QuarkLayer.h"
#include "QuarkModel.h"

// Module
static PyModuleDef QuarkModule =
        {
                PyModuleDef_HEAD_INIT,
                "quark",
                "Basic AI library",
                -1,
                nullptr, nullptr, nullptr, nullptr, nullptr
        };

PyMODINIT_FUNC PyInit_quark(void)
{
    PyObject *m;
    if (PyType_Ready(&quark_Perceptron) < 0)
        return nullptr;
    if (PyType_Ready(&quark_GradientPerceptron) < 0)
        return nullptr;
    if (PyType_Ready(&quark_AdalinePerceptron) < 0)
        return nullptr;
    if (PyType_Ready(&quark_Layer) < 0)
        return nullptr;
    if (PyType_Ready(&quark_Model) < 0)
        return nullptr;

    m = PyModule_Create(&QuarkModule);
    if (m == nullptr)
        return nullptr;

    Py_INCREF(&quark_Perceptron);
    Py_INCREF(&quark_GradientPerceptron);
    Py_INCREF(&quark_AdalinePerceptron);
    Py_INCREF(&quark_Layer);
    Py_INCREF(&quark_Model);

    // Add types
    PyModule_AddObject(m, "Perceptron", (PyObject *)&quark_Perceptron);
    PyModule_AddObject(m, "GradientPerceptron", (PyObject *)&quark_GradientPerceptron);
    PyModule_AddObject(m, "AdalinePerceptron", (PyObject *)&quark_AdalinePerceptron);
    PyModule_AddObject(m, "Layer", (PyObject *)&quark_Layer);
    PyModule_AddObject(m, "Model", (PyObject *)&quark_Model);
    return m;
}