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

#include "vertexReader.h"
// #include "proxy.h"


using namespace std;

//MARK: Open File:

ifstream openFile(string fileName) {
    ifstream inData;
    
    inData.open( getAbsoluteDirectory( "VertexMeshes/", fileName) );
    if (!inData) { throw runtime_error("could not open file"); }

    return inData;
}

//MARK: helperFunctions
ifstream& skipLines( ifstream& file, int num ) {
    for (int i = 0; i < num; i++) { file.ignore(numeric_limits<streamsize>::max(), '\n'); }
    return file;
}

ifstream& goToString( ifstream& file, int num ) {
    for ( int i = 0; i < num - 1; i++ ) { file.ignore( numeric_limits<streamsize>::max(), ' ' ); }
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

    // cout << stringNumber << endl;
    float number = stof( stringNumber );

    return number;
}


//MARK: ReadFile
EngineObject::BufferInformation extractInformation(string fileName) {

    ifstream inData = openFile(fileName);

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

    vector<EngineObject::Vertex> vertices;
    vertices.resize( vertexCount + 1 );
    for (int i = 0; i < vertexCount; i++) {

        float x = extractFloat(inData, 0, 0);
        float z = extractFloat(inData, 0, 0);
        float y = -extractFloat(inData, 0, 0);

        float nx = extractFloat(inData, 0, 0);
        float ny = extractFloat(inData, 0, 0);
        float nz = extractFloat(inData, 0, 0);

        float u = 0;
        float v = 0;

        if (propertyCount == 8) {
            u = extractFloat(inData, 0, 0);
            v = extractFloat(inData, 0, 0);
        }

        // cout << x << ", " << y << ", " << z << ", " << nx << ", " << ny << ", " << nz << ", " << u << ", " << v <<  endl;

        vertices[i].pos = { x, y, z };  
        vertices[i].color = {1.0f, 1.0f, 1.0f};
        vertices[i].normal = { nx, ny, nz };
        vertices[i].UV = { u, v };

        skipLines(inData, 1);
    }

    vector<uint16_t> indices;

    // inData.ignore( numeric_limits<streamsize>::max(), ' ' );
    // for converting fan to strip:

    for (int i = 0; i < faceCount; i++) {
        int count = extractInt(inData, 0, 0);
        int extra = floor(count / 2) - 1;

        int faceTotal = (count - 2) * 3;

        vector<uint16_t> face;
        face.resize(faceTotal);

        int first = extractInt(inData, 0, 0);

        int oldValue = extractInt(inData, 0, 0);

        for (int f = 1; f < faceTotal; f+=3) {
            
            int newValue = extractInt(inData, 0, 0);

            face[f - 1] = first;
            face[f] = oldValue;
            face[f + 1] = newValue;

            oldValue = newValue;
        }

        indices.insert( indices.end(), face.begin(), face.end() );
        skipLines(inData, 1);
    }


    EngineObject::BufferInformation bufferInformation = {
        vertices,
        indices
    };

    inData.close(); 

    return bufferInformation;
}