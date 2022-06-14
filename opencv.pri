linux {
    INCLUDEPATH += /usr/local/include/opencv4   

    LIBS += -lopencv_core \
            -lopencv_highgui \
            -lopencv_imgproc
}

INCLUDEPATH += $${PWD}
