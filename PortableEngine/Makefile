# INCDIR=-IC\:/Users/Caleb/Documents/common/glm-0.9.8.5 -I../emsdk/upstream/emscripten/tests/glbook/Common -I../stb
# INCDIR=-I../../common/glm-0.9.8.5/glm -I../emsdk/upstream/emscripten/tests/glbook/Common -I../stb
INCDIR=-I../../../common/glm/glm -I../../../Github/entt/src/entt

LIBS=-lGLESv2 -lEGL -lm -lX11
COMMONSRC= ./esUtil.c		                                                \
			./PortableGame.cpp												\
			./GameWindow.cpp												\
			./EmscriptenPlatform.cpp										\
			./OpenGLAPI.cpp													\
			./OpenGLESContext.cpp

		# ./Shader.cpp
# ../emsdk/upstream/emscripten/tests/glbook/Common/esShader.c   \
# 		../emsdk/upstream/emscripten/tests/glbook/Common/esTransform.c 	\
#         ../emsdk/upstream/emscripten/tests/glbook/Common/esShapes.c
COMMONHDR=./esUtil.h														\
			./PortableGame.h												\
			./GameWindow.h													\
			./IPlatform.h													\
			./IGraphicsAPI.h												\
			./EmscriptenPlatform.h											\
			./OpenGLAPI.h													\
			./OpenGLESContext.h
SRC=./Main.cpp


default: ./EmscriptenOpenGL.bc

./EmscriptenOpenGL.bc: ${COMMONSRC} ${COMMONHDR} ${SRC}
	$(CC) ${COMMONSRC} ${SRC} -o $@ ${INCDIR} ${LIBS}
