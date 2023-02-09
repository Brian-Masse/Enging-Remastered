#app/Construcors

PWD := $(shell pwd)

OBJ_DIR := ${MAIN_PWD}/objectFiles

MAIN_FILES := $(wildcard *.cpp)
MAIN_OBJ_FILES := $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(MAIN_FILES))

main: ${MAIN_OBJ_FILES}
	
${OBJ_DIR}/main.o: main.cpp ${APP_H_DEPENDENCIES} app/app.h ${MAIN_FILES}
	@echo "remaking main"
	@${CC} ${FLAGS} ${INCLUDE} ${LIBRARIES} -c $< -o $@