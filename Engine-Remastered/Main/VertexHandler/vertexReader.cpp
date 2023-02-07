// Libs
#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <mach-o/dyld.h>
#include <GLFW/glfw3.h>

// std 
#include <algorithm>
#include <vector>
#include <array>
#include <stdexcept>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <set>

#include "../app/Buffers/vertexBuffer.h"

using namespace std;

//MARK: Open File:

ifstream openFile(string fileName) {
    ifstream inData;

    ostringstream pathName;
    char buf [PATH_MAX];
    uint32_t bufSize = PATH_MAX;
    if ( _NSGetExecutablePath(buf, &bufSize) != 0 ) { throw runtime_error("error getting executable path"); }

    uint32_t bufLength = strlen(buf);
    *(buf + bufLength - 3) = '\0'; 

    pathName << buf << "../Engine-Remastered/VertexMeshes/" << fileName;
    inData.open(pathName.str());
    if (!inData) { throw runtime_error("could not open file"); }

    return inData;
}

//MARK: helperFunctions
ifstream& skipLines( ifstream& file, int num ) {

    for (int i = 0; i < num; i++) {
        file.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return file;
}

ifstream& goToString( ifstream& file, int num ) {
    for ( int i = 0; i < num - 1; i++ ) {
        file.ignore( numeric_limits<streamsize>::max(), ' ' );
    }
    return file;
}



int extractInt( ifstream& file, int row, int column ) {
    string stringNumber;
    skipLines(file, row - 1);
    goToString(file, column);

    file >> stringNumber;
    int number = stoi( stringNumber );
    
    return number;
}

float extractFloat( ifstream& file, int row, int column ) {
    string stringNumber;
    skipLines(file, row - 1);
    goToString(file, column);

    file >> stringNumber;
    float number = stof( stringNumber );

    return number;
}


//MARK: ReadFile
BufferInformation extractInformation(string fileName) {

    ifstream inData = openFile(fileName);
    double test;
    string str;

    int vertexCount = extractInt( inData, 4, 3 );


    // //check how many properties are on each vertex
    string tempString;
    int propertyCount = 0;

    skipLines(inData, 1);
    inData >> tempString;

    while ( tempString[0] == 'p' ) {
        skipLines(inData, 1);
        inData >> tempString; 
        propertyCount++;
    }

    int faceCount = extractInt( inData, 0, 3 );
    skipLines(inData, 3);

    vector<Vertex> vertices;
    vertices.resize( vertexCount );
    for (int i = 0; i < vertexCount; i++) {
        float x = extractFloat(inData, 0, 0);
        float y = extractFloat(inData, 0, 0);
        float z = extractFloat(inData, 0, 0);

        vertices[i].color = {1.0f, 0.0f, 0.0f};
        vertices[i].pos = { x, y, z };

        skipLines(inData, 1);
    }

    vector<uint16_t> indices;

    int indicesCount = extractInt(inData, 0, 0);
    indices.resize( faceCount * indicesCount );

    for (int i = 0; i < faceCount; i++) {
        inData.ignore( numeric_limits<streamsize>::max(), ' ' );

        for (int f = 0; f < indicesCount; f++) {
            int index = extractInt(inData, 0, 0);

            int insertion = f;

            //remaps indices to Triangle strip 
            if (indicesCount == 4) {
                if (f == 0) { insertion = 1; } 
                if (f == 1) { insertion = 0; } 
            }

            indices[(i * indicesCount) + insertion] = index;
        }
        skipLines(inData, 1);
    }

    BufferInformation bufferInformation = {
        vertices,
        indices
    };

    inData.close(); 

    return bufferInformation;
}