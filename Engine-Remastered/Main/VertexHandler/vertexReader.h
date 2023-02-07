#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <mach-o/dyld.h>


using namespace std;

BufferInformation extractInformation(string fileName);
vector<Vertex>& translateVertices(vector<Vertex>& vertices);

