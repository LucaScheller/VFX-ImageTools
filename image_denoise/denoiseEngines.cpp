#include "boost/python/detail/wrap_python.hpp"
#include <boost/python/numpy.hpp>
#include <string>
#include <iostream>
#include <OpenImageDenoise/oidn.hpp>

namespace python = boost::python;
namespace np = boost::python::numpy;

class OpenImageDenoiseEngine
{
public:
    OpenImageDenoiseEngine(std::string filterType);
    ~OpenImageDenoiseEngine();
    // Getter/Setters
    std::string getFilterType();
    // General/C++ API
    void colorFilterSet(std::string name, float value);
    void colorFilterSet(std::string name, bool value);
    bool denoise(int width, int height, np::ndarray &colorArray, np::ndarray &outputArray, bool prefilter = false);
    bool denoise(int width, int height, np::ndarray &colorArray, np::ndarray &outputArray, np::ndarray &albedoArray, np::ndarray &normalArray, bool prefilter = false);
    // Python API
    bool denoise_python_wrapper_1(int width, int height, np::ndarray &colorArray, np::ndarray &outputArray);
    bool denoise_python_wrapper_2(int width, int height, np::ndarray &colorArray, np::ndarray &outputArray, bool prefilter);
    bool denoise_python_wrapper_3(int width, int height, np::ndarray &colorArray, np::ndarray &outputArray, np::ndarray &albedoArray, np::ndarray &normalArray);
    bool denoise_python_wrapper_4(int width, int height, np::ndarray &colorArray, np::ndarray &outputArray, np::ndarray &albedoArray, np::ndarray &normalArray, bool prefilter);

private:
    std::string filterType;
    oidn::DeviceRef device;
    oidn::FilterRef colorFilter;
    oidn::FilterRef albedoFilter;
    oidn::FilterRef normalFilter;
};

// Constructor
OpenImageDenoiseEngine::OpenImageDenoiseEngine(std::string filterType = "RT")
{
    // Class internals
    this->filterType = filterType;
    // Create Intel Open Image Denoise device
    device = oidn::newDevice();
    device.commit();
    // Create Intel Open Image Denoise filters (Only "Ray Tracing" filter for now!)
    this->colorFilter = device.newFilter(filterType.c_str());
    this->albedoFilter = device.newFilter(filterType.c_str());
    this->normalFilter = device.newFilter(filterType.c_str());
}
// Deconstructor
OpenImageDenoiseEngine::~OpenImageDenoiseEngine(void)
{
}

std::string OpenImageDenoiseEngine::getFilterType()
{
    return this->filterType;
}

void OpenImageDenoiseEngine::colorFilterSet(std::string name, float value)
{
    this->colorFilter.set(name.c_str(), value);
}

void OpenImageDenoiseEngine::colorFilterSet(std::string name, bool value)
{
    this->colorFilter.set(name.c_str(), value);
}

void (OpenImageDenoiseEngine::*colorFilterSet_python_wrapper_1)(std::string, float) = &OpenImageDenoiseEngine::colorFilterSet;

void (OpenImageDenoiseEngine::*colorFilterSet_python_wrapper_2)(std::string, bool) = &OpenImageDenoiseEngine::colorFilterSet;

bool OpenImageDenoiseEngine::denoise(int width, int height, np::ndarray &colorArray, np::ndarray &outputArray, bool prefilter)
{
    np::ndarray emptyArray = np::zeros(python::make_tuple(0), np::dtype::get_builtin<float>());
    return OpenImageDenoiseEngine::denoise(width, height, colorArray, outputArray, emptyArray, emptyArray, prefilter);
}

bool OpenImageDenoiseEngine::denoise(int width, int height, np::ndarray &colorArray, np::ndarray &outputArray, np::ndarray &albedoArray, np::ndarray &normalArray, bool prefilter)
{
    // Validate args
    bool albedoValid = albedoArray.shape(0) != 0;
    bool normalValid = normalArray.shape(0) != 0;
    if (albedoValid && !normalValid)
    {
        PyErr_SetString(PyExc_TypeError, "Valid albedo, but invalid normal array given!");
        python::throw_error_already_set();
    }
    if (!albedoValid && normalValid)
    {
        PyErr_SetString(PyExc_TypeError, "Valid normal, but invalid albedo array given!");
        python::throw_error_already_set();
    }
    // Validate dtype
    if (colorArray.get_dtype() != np::dtype::get_builtin<float>())
    {
        PyErr_SetString(PyExc_TypeError, "Incorrect array data type!");
        python::throw_error_already_set();
    }

    // Convert to array
    // std::cout << python::extract<char const *>(python::str(input_array.get_dtype()));
    float *colorArrayPtr = reinterpret_cast<float *>(colorArray.get_data());
    float *albedoArrayPtr = reinterpret_cast<float *>(albedoArray.get_data());
    float *normalArrayPtr = reinterpret_cast<float *>(normalArray.get_data());
    float *outputArrayPtr = reinterpret_cast<float *>(outputArray.get_data());

    // Set filter data
    this->colorFilter.setImage("color", colorArrayPtr, oidn::Format::Float3, width, height);
    if (albedoValid)
    {
        this->colorFilter.setImage("albedo", albedoArrayPtr, oidn::Format::Float3, width, height);
    }
    if (normalValid)
    {
        this->colorFilter.setImage("normal", normalArrayPtr, oidn::Format::Float3, width, height);
    }
    this->colorFilter.setImage("output", outputArrayPtr, oidn::Format::Float3, width, height);
    this->colorFilter.set("cleanAux", prefilter);
    this->colorFilter.commit();
    // Prefilter auxiliary aovs
    if (prefilter)
    {
        if (albedoValid)
        {
            // Albedo filter
            this->albedoFilter.setImage("albedo", albedoArrayPtr, oidn::Format::Float3, width, height);
            this->albedoFilter.setImage("output", albedoArrayPtr, oidn::Format::Float3, width, height);
            this->albedoFilter.commit();
            this->albedoFilter.execute();
        }
        if (normalValid)
        {
            // Normal filter
            this->normalFilter.setImage("normal", normalArrayPtr, oidn::Format::Float3, width, height);
            this->normalFilter.setImage("output", normalArrayPtr, oidn::Format::Float3, width, height);
            this->normalFilter.commit();
            this->normalFilter.execute();
        }
    }
    // Filter color image
    this->colorFilter.execute();
    // Check for errors
    const char *errorMessage;
    if (device.getError(errorMessage) != oidn::Error::None)
    {
        std::cout << "Error: " << errorMessage << std::endl;
        return false;
    }
    return true;
}

bool OpenImageDenoiseEngine::denoise_python_wrapper_1(int width, int height, np::ndarray &colorArray, np::ndarray &outputArray)
{
    np::ndarray emptyArray = np::zeros(python::make_tuple(0), np::dtype::get_builtin<float>());
    return OpenImageDenoiseEngine::denoise(width, height, colorArray, outputArray, emptyArray, emptyArray, false);
}

bool OpenImageDenoiseEngine::denoise_python_wrapper_2(int width, int height, np::ndarray &colorArray, np::ndarray &outputArray, bool prefilter)
{
    np::ndarray emptyArray = np::zeros(python::make_tuple(0), np::dtype::get_builtin<float>());
    return OpenImageDenoiseEngine::denoise(width, height, colorArray, outputArray, emptyArray, emptyArray, prefilter);
}

bool OpenImageDenoiseEngine::denoise_python_wrapper_3(int width, int height, np::ndarray &colorArray, np::ndarray &outputArray, np::ndarray &albedoArray, np::ndarray &normalArray)
{
    return OpenImageDenoiseEngine::denoise(width, height, colorArray, outputArray, albedoArray, normalArray, false);
}

bool OpenImageDenoiseEngine::denoise_python_wrapper_4(int width, int height, np::ndarray &colorArray, np::ndarray &outputArray, np::ndarray &albedoArray, np::ndarray &normalArray, bool prefilter)
{
    return OpenImageDenoiseEngine::denoise(width, height, colorArray, outputArray, albedoArray, normalArray, prefilter);
}


static std::string __OpenImageDenoiseEngineRepr(OpenImageDenoiseEngine &ctx) {
    std::string repr = "OpenImageDenoiseEngine(";
    repr += "filterType='" + ctx.getFilterType() + "'";
    repr += ")";
    return repr;
}


BOOST_PYTHON_MODULE(denoiseEngines)
{
    Py_Initialize();
    np::initialize();

    python::class_<OpenImageDenoiseEngine>("OpenImageDenoiseEngine")
        .def(python::init<python::optional<std::string>>())
        .def("__repr__", &__OpenImageDenoiseEngineRepr)
        .add_property("filterType", &OpenImageDenoiseEngine::getFilterType)
        .def("colorFilterSet", colorFilterSet_python_wrapper_1)
        .def("colorFilterSet", colorFilterSet_python_wrapper_2)
        .def("denoise", &OpenImageDenoiseEngine::denoise_python_wrapper_1)
        .def("denoise", &OpenImageDenoiseEngine::denoise_python_wrapper_2)
        .def("denoise", &OpenImageDenoiseEngine::denoise_python_wrapper_3)
        .def("denoise", &OpenImageDenoiseEngine::denoise_python_wrapper_4);
}