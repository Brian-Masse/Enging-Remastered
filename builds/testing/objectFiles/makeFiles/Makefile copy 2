#app/buffers
.SILENT:

PWD := $(shell pwd)

OBJ_DIR := ${MAIN_PWD}/objectFiles

MAIN_FILES := $(wildcard *.cpp)
MAIN_OBJ_FILES := $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(MAIN_FILES))

main: ${MAIN_OBJ_FILES}

	
${OBJ_DIR}/%.o: %.cpp ${UNIVERSAL} ${APP_H_DEPENDENCIES} ../../universalConstructors/universalConstructors.h ../app.h ${MAIN_FILES}
	@echo "remaking buffers"
	@${CC} ${FLAGS} ${INCLUDE} ${LIBRARIES} -c $< -o $@