#ifndef PYQUARK_LISTSHELPER_H
#define PYQUARK_LISTSHELPER_H

#include <Python.h>
#include <vector>
#include <functional>
#include <stdexcept>

static PyObject* PythonListIterToPythonList(PyObject* pyListIter)
{
    PyObject* pyList = PyList_New(0);

    if (pyListIter && pyList)
    {
        PyObject* item;

        while ((item = PyIter_Next(pyListIter)))
        {
            PyList_Append(pyList, item);
            Py_DECREF(item);
        }

        Py_DECREF(pyListIter);
    }

    Py_DECREF(pyList);
    return pyList;
}

template<typename T>
static std::vector<T> PythonListIteratorToVector(PyObject* iter, std::function<T(PyObject*)> converter)
{
    std::vector<T> vec;
    PyObject* item;
    while ((item = PyIter_Next(iter)))
    {
        vec.emplace_back(converter(item));
    }
    Py_DECREF(iter);

    return vec;
}

/// Source: https://pythonextensionpatterns.readthedocs.io/en/latest/cpp_and_cpython.html#python-list-to-c-std-vector-t
/// Adapted by Benjamin W.
/// Usage:
/// if (PyErr_Occurred()) { }
/// else { }
template <typename T>
static std::vector<T> PythonListToVector(PyObject *pyList, std::function<T(PyObject*)> converter)
{
    assert(cpython_asserts(pyList));
    std::vector<T> cppVector;

    if (PyObject_IsInstance(pyList, (PyObject*) &PyListIter_Type))
        pyList = PythonListIterToPythonList(pyList);

    if (PyList_Check(pyList))
    {
        cppVector.reserve(PyList_GET_SIZE(pyList));
        for (auto i = 0; i < PyList_GET_SIZE(pyList); ++i)
        {
            cppVector.emplace(cppVector.end(), converter(PyList_GetItem(pyList, i)));
            if (PyErr_Occurred())
            {
                cppVector.clear();
                break;
            }
        }
    }
    else
    {
        PyErr_Format(PyExc_TypeError, R"(Argument "pyList" to %s must be list not "%s")", __FUNCTION__, Py_TYPE(pyList)->tp_name);
    }
    return cppVector;
}

// Public
template <typename T>
PyObject* VectorToPythonList(const std::vector<T> &cppVector, std::function<PyObject*(const T&)> converter)
{
    assert(cpython_asserts());
    auto *r = PyList_New(cppVector.size());
    if (!r)
    {
        goto except;
    }

    for (auto i = 0; i < cppVector.size(); ++i)
    {
        auto *item = converter(cppVector[i]);
        if (!item || PyErr_Occurred() || PyList_SetItem(r, i, item))
        {
            goto except;
        }
    }
    assert(!PyErr_Occurred());
    assert(r);
    goto finally;
    except:
    assert(PyErr_Occurred());

    if(r)
    {
        for(auto i = 0; i < PyList_GET_SIZE(r); ++i)
        {
            Py_XDECREF(PyList_GET_ITEM(r, i));
        }

        Py_DECREF(r);
        r = nullptr;
    }

    finally:
    return r;
}

template<typename T>
std::vector<T> PythonIterableToVector(PyObject* o, std::function<T(PyObject*)> converter)
{
    if (PyObject_IsInstance(o, (PyObject*) &PyList_Type))
        return PythonListToVector<T>(o, std::move(converter));
    else if (PyObject_IsInstance(o, (PyObject*) &PyListIter_Type))
        return PythonListIteratorToVector(o, std::move(converter));

    throw std::runtime_error("Not iterable");
}

#endif //PYQUARK_LISTSHELPER_H
