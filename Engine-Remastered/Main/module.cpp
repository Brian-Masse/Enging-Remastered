// Libs
#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <mach-o/dyld.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

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

#include <pybind11/pybind11.h>

using namespace std;

void sum( int x, int y ) {
    cout << x + y << endl;
}


PYBIND11_MODULE(pybind11Module, module) {

    module.doc() = "pybind11Module";

    module.def( "python_sum", &sum );

}

