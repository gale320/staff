# This file generated by staff_codegen
# For more information please visit: http://code.google.com/p/staff/

QT          += core
QT          -= gui

TARGET       = $(Project.Interfaces.Interface.Namespace.!dot||Project.Interfaces.Interface.Name.!tolower.!dot)
CONFIG      += console
CONFIG      -= app_bundle

TEMPLATE     = app

INCLUDEPATH += \$(STAFF_HOME)/include
LIBS        += -L\$(STAFF_HOME)/lib -lstaffutils -lstaffcommon -lstaffclient -lrise

#var nLastInterface $(Project.Interfaces.$Count)
#var nLastInterface $($nLastInterface.!dec)

STAFF_INTERFACES = \\

#foreach $(Project.Interfaces)
    $($SRCDIR)$(Interface.Name).h\
#ifneq($(Interface.$Num),$($nLastInterface))
    \\

#ifeqend
#end

SOURCES     += \\

    $($SRCDIR)main.cpp

HEADERS     += \\

#foreach $(Project.Interfaces)
    $($SRCDIR)$(Interface.Name).h\
#ifneq($(Interface.$Num),$($nLastInterface))
    \\

#ifeqend
#end

#ifeq($($bFromInterface),)

OTHER_FILES += \\

#foreach $(Project.Interfaces)
    $(Interface.FileName)\
#ifneq($(Interface.$Num),$($nLastInterface))
    \\

#ifeqend
#end
#ifeqend


staff_client_proxy_decl.name  = staff client proxy header
staff_client_proxy_decl.input = STAFF_INTERFACES
staff_client_proxy_decl.output  = ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}Proxy.h ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}.h
staff_client_proxy_decl.commands = \$(STAFF_HOME)/bin/staff_codegen -tclient -c${QMAKE_FILE_PATH} ${QMAKE_FILE_BASE}.h
staff_client_proxy_decl.variable_out = GENERATED_FILES
QMAKE_EXTRA_COMPILERS += staff_client_proxy_decl

staff_client_proxy_impl.name  = staff client proxy proxy implementation
staff_client_proxy_impl.input = STAFF_INTERFACES
staff_client_proxy_impl.output  = ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}Proxy.cpp
staff_client_proxy_impl.depends  = ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}Proxy.h ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}.h
staff_client_proxy_impl.commands = $$escape_expand("\\n")
staff_client_proxy_impl.variable_out = GENERATED_SOURCES
QMAKE_EXTRA_COMPILERS += staff_client_proxy_impl

target.depends = staff_client_proxy_impl
