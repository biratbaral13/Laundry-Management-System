QT       += core gui widgets sql
TARGET    = LaundryF
TEMPLATE  = app
CONFIG   += c++17

SOURCES += \
    main.cpp \
    database.cpp \
    mainwindow.cpp \
    rolescreen.cpp \
    ownersignup.cpp \
    ownerlogin.cpp \
    forgotpassword.cpp \
    ownermainmenu.cpp \
    neworder.cpp \
    customerdetails.cpp \
    billingview.cpp \
    customerflow.cpp

HEADERS += \
    enums.h \
    database.h \
    mainwindow.h \
    rolescreen.h \
    ownersignup.h \
    ownerlogin.h \
    forgotpassword.h \
    ownermainmenu.h \
    neworder.h \
    customerdetails.h \
    billingview.h \
    customerflow.h

RESOURCES += \
    resources.qrc
