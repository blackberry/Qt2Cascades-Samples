TEMPLATE = app
TARGET = i18n

CONFIG += qt warn_on debug_and_release cascades

INCLUDEPATH += ../src
SOURCES += ../src/*.cpp
HEADERS += ../src/*.hpp ../src/*.h
RESOURCES += i18n.qrc

device {
    CONFIG(release, debug|release) {
        DESTDIR = o.le-v7
    }
    CONFIG(debug, debug|release) {
        DESTDIR = o.le-v7-g
    }
}

simulator {
    CONFIG(release, debug|release) {
        DESTDIR = o
    }
    CONFIG(debug, debug|release) {
        DESTDIR = o-g
    }
}

OBJECTS_DIR = $${DESTDIR}/.obj
MOC_DIR = $${DESTDIR}/.moc
RCC_DIR = $${DESTDIR}/.rcc
UI_DIR = $${DESTDIR}/.ui

TRANSLATIONS += translations/i18n_ar.ts \
                translations/i18n_cs.ts \
                translations/i18n_de.ts \
                translations/i18n_el.ts \
                translations/i18n_en.ts \
                translations/i18n_eo.ts \
                translations/i18n_fr.ts \
                translations/i18n_it.ts \
                translations/i18n_jp.ts \
                translations/i18n_ko.ts \
                translations/i18n_no.ts \
                translations/i18n_ru.ts \
                translations/i18n_sv.ts \
                translations/i18n_zh.ts