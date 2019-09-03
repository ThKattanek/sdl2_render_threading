TEMPLATE = app
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

TARGET = "sdl2_render_threading"

SOURCES += \
        main.cpp
LIBS += -lsdl2 -lsdl2main
