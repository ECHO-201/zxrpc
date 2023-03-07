CC = g++

SRCS := $(shell find ./* -type f | grep '\.cpp' | grep -v '\.svn' | grep -v '\./plugin' | grep -v '\./log' | grep -v 'main\.cpp' | grep -v 'callee\.cpp' | grep -v 'caller\.cpp')
$(warning SRCS is ${SRCS})

OBJS := $(patsubst %.cpp, %.o, $(filter %.cpp, $(SRCS)))
$(warning OBJS is ${OBJS})

CFLAGS = -g -O2 -Wall -Werror -Wno-unused -ldl -lpthread -ljsoncpp -lprotobuf -fPIC -std=c++14 -lzookeeper_mt -lhiredis -lmysqlclient
$(warning CFLAGS is ${CFLAGS})

INCLUDE_TEMP = $(shell find ./* -type d | grep -v '\.svn' | grep -v '\./plugin' | grep -v '\./client' | grep -v '\./config' | grep -v '\./log')
INCLUDE = $(patsubst %,-I %, $(INCLUDE_TEMP))
$(warning INCLUDE is ${INCLUDE})

SRC_MAIN = main.cpp proto/echo.pb.h proto/echo.pb.cc
OBJ_MAIN = ${SRC_MAIN:%.cpp=%.o}
EXE_MAIN = main

SRC_CALLEE = callee.cpp proto/echo.pb.h proto/echo.pb.cc
OBJ_CALLEE = ${SRC_CALLEE:%.cpp=%.o}
EXE_CALLEE = callee

SRC_CALLER = caller.cpp proto/echo.pb.h proto/echo.pb.cc
OBJ_CALLER = ${SRC_CALLER:%.cpp=%.o}
EXE_CALLER = caller

target: ${EXE_MAIN} ${EXE_CALLEE} ${EXE_CALLER}

callee: ${EXE_CALLEE}

caller: ${EXE_CALLER}

main: ${EXE_MAIN}

$(EXE_MAIN): $(OBJ_MAIN) $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(INCLUDE)

$(EXE_CALLEE): $(OBJ_CALLEE) $(OBJS) 
	$(CC) -o $@ $^ $(CFLAGS) $(INCLUDE)

$(EXE_CALLER): $(OBJ_CALLER) $(OBJS) 
	$(CC) -o $@ $^ $(CFLAGS) $(INCLUDE)

%.o: %.cpp
	${CC} ${CFLAGS} ${INCLUDE} -c $< -o $@

TEST_PULGIN = plugin/testplugin.so
TEST_SRC_PLUGIN = plugin/TestPlugin.cpp

USER_PLUGIN = plugin/userplugin.so
USER_SRC_PLUGIN = plugin/UserPlugin.cpp

ECHO_PLUGIN = plugin/echoplugin.so
ECHO_SRC_PLUGIN = plugin/EchoPlugin.cpp tools/Logger.cpp tools/System.cpp

${TEST_PULGIN}:
	${CC} -shared -fPIC ${INCLUDE} ${TEST_SRC_PLUGIN} -o ${TEST_PULGIN}

${USER_PLUGIN}:
	${CC} -shared -fPIC ${INCLUDE} ${USER_SRC_PLUGIN} -o ${USER_PLUGIN}

${ECHO_PLUGIN}:
	${CC} -shared -fPIC ${INCLUDE} ${ECHO_SRC_PLUGIN} -o ${ECHO_PLUGIN}

plugin: ${TEST_PULGIN} ${USER_PLUGIN} ${ECHO_PLUGIN}
	
clean:
	rm -f ${OBJS} ${OBJ_MAIN} ${EXE_MAIN} ${OBJ_CALLEE} ${OBJ_CALLER} ${EXE_CALLEE} ${EXE_CALLER}

