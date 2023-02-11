#app
.SILENT:

PWD := $(shell pwd)

OBJ_DIR := ${MAIN_PWD}/objectFiles

MAIN_FILES := $(wildcard *.cpp)
MAIN_OBJ_FILES := $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(MAIN_FILES))

main: ${MAIN_OBJ_FILES}

${OBJ_DIR}/%.o: %.cpp ${UNIVERSAL} ../VertexHandler/vertexReader.h ../universalConstructors/universalConstructors.h ${MAIN_FILES}
	@echo "remaking objects"
	@${CC} ${FLAGS} ${INCLUDE} ${LIBRARIES} -c $< -o $@


