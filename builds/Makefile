.SILENT:
.PHONY = all clean

#//MARK: DIRS

export MAIN_PWD := $(shell pwd)
APP := ${MAIN_PWD}/../Engine-Remastered/Main/app
OBJ_DIR := objectFiles
PIPELINE_DIR = ${APP}/GraphicsPipeline
PIPELINE_OBJS = ${PIPELINE_DIR}/shader.frag ${PIPELINE_DIR}/shader.vert

#//MARK: ARGS

export CC := clang++
export FLAGS := -std=c++17 -fdiagnostics-color=always -Wall -g -w
export INCLUDE := -I${MAIN_PWD}../dependencies/include -I${MAIN_PWD}/../dependencies/library/stb
export LIBRARIES := -L${MAIN_PWD}/../dependencies/library ${MAIN_PWD}/../dependencies/library/libglfw.3.3.dylib ${MAIN_PWD}/../dependencies/library/Vulkan/libvulkan.1.3.231.dylib 

#//MARK: DEPENDENCIES

export APP_H_DEPENDENCIES := ${APP}/proxy.h 

#//MARK: LINKING VARIABLES

OBJ_FILES := $(wildcard ${OBJ_DIR}/*.o)

#//MARK: MAIN
app: make
	@echo "Starting Linker"
	@${CC} ${FLAGS} ${INCLUDE} ${LIBRARIES} ${OBJ_FILES} -o app
	@echo "Linking finished successfully!"
	
make: 
	
	@echo "Starting Build Tasks"
	@cd ../Engine-Remastered/Main && $(MAKE)
	@cd ../Engine-Remastered/Main/objects && $(MAKE)
	@cd ../Engine-Remastered/Main/app && $(MAKE)
	@cd ../Engine-Remastered/Main/app/Buffers && $(MAKE)
	@cd ../Engine-Remastered/Main/app/Constructor && $(MAKE)
	@cd ../Engine-Remastered/Main/VertexHandler && $(MAKE)
	@echo "Build finished successfully!\n"

clean:
	rm -rvf ${OBJ_DIR}/*.o

shader: ${PIPELINE_OBJS}
	@echo "Compiling Shader Files"
	sh ${PIPELINE_DIR}/compile.sh
	@echo "Shaders Compiled successfully!\n"

# $< matches prerequisites 
# $@ mathces targets