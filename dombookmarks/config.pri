# Config.pri file version 2.0. Auto-generated by IDE. Any changes made by user will be lost!
BASEDIR = $$quote($$_PRO_FILE_PWD_)

device {
    CONFIG(debug, debug|release) {
        profile {
            INCLUDEPATH += $$quote(${QNX_TARGET}/usr/include/qt4/QtXml)

            DEPENDPATH += $$quote(${QNX_TARGET}/usr/include/qt4/QtXml)

            LIBS += -lQtXml

            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        } else {
            INCLUDEPATH += $$quote(${QNX_TARGET}/usr/include/qt4/QtXml)

            DEPENDPATH += $$quote(${QNX_TARGET}/usr/include/qt4/QtXml)

            LIBS += -lQtXml

            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        }

    }

    CONFIG(release, debug|release) {
        !profile {
            INCLUDEPATH += $$quote(${QNX_TARGET}/usr/include/qt4/QtXml)

            DEPENDPATH += $$quote(${QNX_TARGET}/usr/include/qt4/QtXml)

            LIBS += -lQtXml

            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        }
    }
}

simulator {
    CONFIG(debug, debug|release) {
        !profile {
            INCLUDEPATH += $$quote(${QNX_TARGET}/usr/include/qt4/QtXml)

            DEPENDPATH += $$quote(${QNX_TARGET}/usr/include/qt4/QtXml)

            LIBS += -lQtXml

            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        }
    }
}

config_pri_assets {
    OTHER_FILES += \
        $$quote($$BASEDIR/assets/BookmarkItem.qml) \
        $$quote($$BASEDIR/assets/FolderItem.qml) \
        $$quote($$BASEDIR/assets/SeparatorItem.qml) \
        $$quote($$BASEDIR/assets/frank.xbel) \
        $$quote($$BASEDIR/assets/images/background.png) \
        $$quote($$BASEDIR/assets/images/bookmark.png) \
        $$quote($$BASEDIR/assets/images/folder.png) \
        $$quote($$BASEDIR/assets/images/separator.png) \
        $$quote($$BASEDIR/assets/jennifer.xbel) \
        $$quote($$BASEDIR/assets/main.qml)
}

config_pri_source_group1 {
    SOURCES += \
        $$quote($$BASEDIR/src/app.cpp) \
        $$quote($$BASEDIR/src/main.cpp) \
        $$quote($$BASEDIR/src/xbelgenerator.cpp) \
        $$quote($$BASEDIR/src/xbelparser.cpp)

    HEADERS += \
        $$quote($$BASEDIR/src/app.hpp) \
        $$quote($$BASEDIR/src/xbelgenerator.hpp) \
        $$quote($$BASEDIR/src/xbelparser.hpp)
}

INCLUDEPATH += $$quote($$BASEDIR/src)

lupdate_inclusion {
    SOURCES += \
        $$quote($$BASEDIR/../src/*.c) \
        $$quote($$BASEDIR/../src/*.c++) \
        $$quote($$BASEDIR/../src/*.cc) \
        $$quote($$BASEDIR/../src/*.cpp) \
        $$quote($$BASEDIR/../src/*.cxx) \
        $$quote($$BASEDIR/../assets/*.qml) \
        $$quote($$BASEDIR/../assets/*.js) \
        $$quote($$BASEDIR/../assets/*.qs) \
        $$quote($$BASEDIR/../assets/images/*.qml) \
        $$quote($$BASEDIR/../assets/images/*.js) \
        $$quote($$BASEDIR/../assets/images/*.qs)

    HEADERS += \
        $$quote($$BASEDIR/../src/*.h) \
        $$quote($$BASEDIR/../src/*.h++) \
        $$quote($$BASEDIR/../src/*.hh) \
        $$quote($$BASEDIR/../src/*.hpp) \
        $$quote($$BASEDIR/../src/*.hxx)
}

TRANSLATIONS = $$quote($${TARGET}.ts)
