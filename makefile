NAME			:= cc3k
SRC_DIR			:= src
OBJ_DIR			:= obj

SRCS			:= ${wildcard ${SRC_DIR}/*.cc}
OBJS			:= ${SRCS:${SRC_DIR}/%.cc=${OBJ_DIR}/%.o}

CC				:= g++
CXXFLAGS		:= -I ./include
CCFLAGS			:= -Wall -g

MAKEFLAGS		:= --no-print-directory

all: ${NAME}

${NAME}: ${OBJS}
	${CC} ${OBJS} -o ${NAME}

${OBJ_DIR}/%.o: ${SRC_DIR}/%.cc
	mkdir -p ${@D}
	${CC} ${CXXFLAGS} ${CCFLAGS} -c -o $@ $<
	${info CREATED $@}

clean:
	rm -f ${OBJS}

fclean: clean
	rm -f ${NAME}

re:
	${MAKE} fclean
	${MAKE} all

.PHONY: clean fclean re
.SILENT: