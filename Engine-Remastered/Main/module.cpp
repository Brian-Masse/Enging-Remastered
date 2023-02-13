// Libs
#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <mach-o/dyld.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <pybind11/pybind11.h>

// std 
#include <algorithm>
#include <vector>
#include <array>
#include <stdexcept>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "app.h"
#include "universalConstructors.h"
#include "vertexReader.h"

using namespace std;

void sum( int x, int y ) {
    cout << x + y << endl;
}

PYBIND11_MODULE(EngineRemastered, module) {

    module.doc() = "EngineRemastered";

    pybind11::class_<EngineRemastered> (module, "EngineRemasteredApp")
        .def ( pybind11::init<> ()) 
        .def ( "initApp", &EngineRemastered::initialization )
        .def ( "runApp", &EngineRemastered::run )
        .def ( "createObj", &EngineRemastered::createObject, "function");
}

