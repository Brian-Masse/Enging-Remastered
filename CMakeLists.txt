# cmake -DCMAKE_INSTALL_PREFIX=~/Installations -DMUQ_USE_PYTHON=ON -DPYTHON_EXECUTABLE=~/opt/miniconda3/bin/python3.9 ../
cmake_minimum_required(VERSION 3.16)
project( "Test Project" )
set(CMAKE_CXX_STANDARD 17)

#UNIVERSALS
set( APP_NAME "app")
set( MOD_NAME "EngineRemastered")

set( BASE "${PROJECT_SOURCE_DIR}/Engine-Remastered" )
set( MAIN "${BASE}/Main" )
set( DEPENDENCIES "${PROJECT_SOURCE_DIR}/dependencies" )

set( COMPILE_FLAGS "-g -Wall" )

#INCLUDES
set( INCLUDES "${BASE}/include" )
set( DEPEND_INCLUDES "${DEPENDENCIES}/include" )
set( DEPEND_LIBRARY "${DEPENDENCIES}/library" )


#DIRECTORIES:

set( APP "${MAIN}/app" )
set( OBJECT "${MAIN}/objects" )
set( UCONSTRUCTORS "${MAIN}/universalConstructors" )


add_definitions(${COMPILE_FLAGS})
find_package(Boost COMPONENTS python REQUIRED)
add_subdirectory(${DEPEND_INCLUDES}/pybind11) 


set( CPP_FILES 

    ${MAIN}/module.cpp
    ${MAIN}/app.cpp
    ${MAIN}/main.cpp

    ${APP}/validation.cpp
    ${MAIN}/module.cpp

    ${APP}/buffers.cpp
    ${APP}/depthBuffer.cpp
    ${APP}/commands.cpp
    ${APP}/devices.cpp
    ${APP}/draw.cpp
    ${APP}/pipeline.cpp
    ${APP}/swapchain.cpp
    

    ${OBJECT}/object.cpp
    ${OBJECT}/vertexBuffer.cpp
    ${OBJECT}/vertexReader.cpp

    ${UCONSTRUCTORS}/imageReader.cpp
    ${UCONSTRUCTORS}/proxy.cpp
    ${UCONSTRUCTORS}/uBuffers.cpp
    ${UCONSTRUCTORS}/uCommands.cpp

)

pybind11_add_module(
    EngineRemastered

    SHARED

    ${CPP_FILES}
)

target_include_directories(
    EngineRemastered
    
    PRIVATE ${MAIN}
    PRIVATE ${APP}
    PRIVATE ${OBJECT}
    PRIVATE ${UCONSTRUCTORS}
    
    PRIVATE ${INCLUDES}
    PRIVATE ${DEPEND_INCLUDES}
    PRIVATE ${DEPEND_INCLUDES}/Vulkan
    PRIVATE ${DEPEND_INCLUDES}/pybind11/include
    PRIVATE ${DEPEND_LIBRARY}
)

target_link_libraries(
    EngineRemastered

    PRIVATE ${Boost_LIBRARIES}
    PRIVATE ${DEPEND_LIBRARY}/libglfw.3.3.dylib
    PRIVATE ${DEPEND_LIBRARY}/Vulkan/libvulkan.1.3.231.dylib
)

#app
add_executable(
    ${APP_NAME}

    ${CPP_FILES}

)

target_include_directories(
    ${APP_NAME}
    PRIVATE ${MAIN}
    PRIVATE ${APP}
    PRIVATE ${OBJECT}
    PRIVATE ${UCONSTRUCTORS}
    
    PRIVATE ${INCLUDES}
    PRIVATE ${DEPEND_INCLUDES}
    PRIVATE ${DEPEND_INCLUDES}/Vulkan
    PRIVATE ${DEPEND_INCLUDES}/pybind11/include
    PRIVATE ${DEPEND_LIBRARY}
)

target_link_libraries(
    ${APP_NAME}

    PRIVATE ${Boost_LIBRARIES}
    PRIVATE ${DEPEND_LIBRARY}/libglfw.3.3.dylib
    PRIVATE ${DEPEND_LIBRARY}/Vulkan/libvulkan.1.3.231.dylib

    PRIVATE pybind11::embed
)