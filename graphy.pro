SOURCES += main.cpp \
    mainwidget.cpp \
    imagewidget.cpp \
    capture.cpp \
    formatconvert.cpp \
    cameraprocess.cpp \
    bluetoothprocess.cpp \
    textedit.cpp \
    mycanvas.cpp
HEADERS += mainwidget.h \
    imagewidget.h \
    capture.h \
    formatconvert.h \
    cameraprocess.h \
    bluetoothprocess.h \
    textedit.h \
    mycanvas.h
LIBS += -lbluetooth \
    -lcwiid
