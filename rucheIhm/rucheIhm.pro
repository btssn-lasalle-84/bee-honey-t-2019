#-------------------------------------------------
#
# Project created by QtCreator 2019-02-27T15:14:26
#
#-------------------------------------------------

QT       += core gui mqtt sql webkitwidgets
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rucheIhm
TEMPLATE = app


INCLUDEPATH += /usr/include/qwt/ simple-mail/
LIBS += -lqwt-qt5


SOURCES += main.cpp \
    ruche.cpp \
    infosEnsoleillement.cpp \
    infosHumidite.cpp \
    infosPressionAtmospherique.cpp \
    infosTemperature.cpp \
    rucheIhm.cpp \
    communication.cpp \
    baseDeDonnees.cpp \
    nouvelleRucheIhm.cpp \
    alertes.cpp \
    reglagesAlertesIhm.cpp \
    infosBatterie.cpp \
    infosPoids.cpp \
    simple-mail/emailaddress.cpp \
    simple-mail/mimeattachment.cpp \
    simple-mail/mimecontentformatter.cpp \
    simple-mail/mimefile.cpp \
    simple-mail/mimehtml.cpp \
    simple-mail/mimeinlinefile.cpp \
    simple-mail/mimemessage.cpp \
    simple-mail/mimemultipart.cpp \
    simple-mail/mimepart.cpp \
    simple-mail/mimetext.cpp \
    simple-mail/quotedprintable.cpp \
    simple-mail/sender.cpp

HEADERS  += \
    ruche.h \
    infosTemperature.h \
    infosHumidite.h \
    infosEnsoleillement.h \
    infosPressionAtmospherique.h \
    rucheIhm.h \
    communication.h \
    baseDeDonnees.h \
    parametres.h \
    nouvelleRucheIhm.h \
    alertes.h \
    reglagesAlertesIhm.h \
    infosBatterie.h \
    infosPoids.h \
    simple-mail/emailaddress.h \
    simple-mail/emailaddress_p.h \
    simple-mail/mimeattachment.h \
    simple-mail/mimecontentformatter.h \
    simple-mail/mimefile.h \
    simple-mail/mimehtml.h \
    simple-mail/mimeinlinefile.h \
    simple-mail/mimemessage.h \
    simple-mail/mimemessage_p.h \
    simple-mail/mimemultipart.h \
    simple-mail/mimemultipart_p.h \
    simple-mail/mimepart.h \
    simple-mail/mimepart_p.h \
    simple-mail/mimetext.h \
    simple-mail/mimetext_p.h \
    simple-mail/quotedprintable.h \
    simple-mail/sender.h \
    simple-mail/sender_p.h \
    simple-mail/smtpexports.h \
    simple-mail/SmtpMime

FORMS    += rucheihm.ui \
    nouvelleRucheIhm.ui \
    reglagesAlertesIhm.ui

DISTFILES += \
    images/soleilCouvert.png

RESOURCES += \
    ressources.qrc
