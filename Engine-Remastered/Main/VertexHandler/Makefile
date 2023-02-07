#app

PWD := $(shell pwd)

OBJ_DIR := ${MAIN_PWD}/objectFiles

MAIN_FILES := $(wildcard *.cpp)
MAIN_OBJ_FILES := $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(MAIN_FILES))

main: ${MAIN_OBJ_FILES}
	echo "THIS IS RUNNING"
	echo ${MAIN_FILES}
	echo ${MAIN_OBJ_FILES}
	echo ${OBJ_DIR}

${OBJ_DIR}/%.o: %.cpp vertexReader.h ${MAIN_FILES}
	@${CC} ${FLAGS} ${INCLUDE} ${LIBRARIES} -c $< -o $@


