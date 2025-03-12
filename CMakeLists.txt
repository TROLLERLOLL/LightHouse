cmake_minimum_required (VERSION 3.8)

project("LightHouse")

set (OPENGL TRUE)
set (DX11 TRUE)
#There is currently "forced" Software support with DX11, starting with Win 7... 
#It's always supported and is currently used as a fallback but only when DX is enabled

#Is target 32 Bit
if (CMAKE_SIZEOF_VOID_P EQUAL 4)
	set (32BIT TRUE)
else()
	set (32BIT FALSE)
endif()

add_library (${PROJECT_NAME} STATIC "src/Graphics/Window.cpp" "src/Graphics/Spritebatch.cpp" "src/Utils/Camera.cpp" "src/Utils/AssetLoader.cpp"  "src/Graphics/Shader.cpp")

set(LIBS)

if (OPENGL)
	find_package(OpenGL REQUIRED)
	if (WIN32)
		set(LIBS ${LIBS} ${OPENGL_LIBRARY})
	elseif (UNIX AND NOT APPLE) #LINUX
		find_package(X11 REQUIRED)
		set(LIBS ${LIBS} OpenGL::GL X11::X11)
	endif()

	target_sources(${PROJECT_NAME} PRIVATE "src/Graphics/OpenGL/Graphics.GL.cpp" "src/Graphics/OpenGL/GL.cpp" "src/Graphics/OpenGL/Shader.GL.cpp" "src/Graphics/OpenGL/Spritebatch.GL.cpp")
	target_compile_definitions(${PROJECT_NAME} PUBLIC "GL")
endif()

if (DX11 AND WIN32)
	target_sources(${PROJECT_NAME} PRIVATE "src/Graphics/DX11/Graphics.DX11.cpp" "src/Graphics/DX11/Shader.DX11.cpp" "src/Graphics/DX11/Spritebatch.DX11.cpp")
	target_compile_definitions(${PROJECT_NAME} PUBLIC "DX11")
endif()

if (WIN32)
	target_sources(${PROJECT_NAME} PRIVATE "src/Graphics/Window/Window.Win32.cpp")
elseif (UNIX AND NOT APPLE)
	target_sources(${PROJECT_NAME} PRIVATE "src/Graphics/Window/Window.X11.cpp")
	target_compile_definitions(${PROJECT_NAME} PUBLIC "X11")
endif()

target_include_directories(${PROJECT_NAME} PUBLIC "src" "include")

target_link_libraries(${PROJECT_NAME} ${LIBS})