TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle

SOURCES += \
        main.cpp

include(torchaudio.pri)
include(torch.pri)

enable_visualization {
    include(opencv.pri)
    DEFINES+=ENABLE_VISUALIZATION
}
