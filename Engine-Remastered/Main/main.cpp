#define GLFW_INCLUDE_VULKAN 
#include <GLFW/glfw3.h>

#include <iostream>  // for error handling
#include <stdexcept> // for error handling
#include <cstdlib>   // EXIT SUCCESS / FAILURE macros

#include "app/app.h"

// MARK: Main
int main()
{
    HelloTriangleApplication app;

    try { app.run(); }

    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}