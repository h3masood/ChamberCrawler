CXX = g++
CXXLAGS = -g -Wall -Werror -std=c++11 -MMD
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}			# makefile name

OBJECTS = cc3kDriver.o game.o cell.o gameobject.o player.o character.o item.o enemy.o
DEPENDS = ${OBJECTS:.o=.d}
EXEC = cc3k

${EXEC} : ${OBJECTS}
	${CXX} ${OBJECTS} -o  ${EXEC}

-include ${DEPENDS}

${OBJECTS} : ${MAKEFILE_NAME}	# OPTIONAL : changes to this file => recompile


.PHONY : clean
clean :
	rm -f ${DEPENDS} ${OBJECTS} ${EXEC}


