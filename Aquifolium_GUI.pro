#-------------------------------------------------
#
# Project created by QtCreator 2018-08-20T10:58:11
#
#-------------------------------------------------

QT       += core gui opengl printsupport
CONFIG += c++14
INCLUDEPATH += ../Aquifolium/include
INCLUDEPATH += ../jsoncpp/include/
INCLUDEPATH += include/

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Aquifolium_GUI
TEMPLATE = app



# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS QT_version Aquifolium DEBUG

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    src/mainwindow.cpp \
    ../Aquifolium/src/Block.cpp \
    ../Aquifolium/src/BTC.cpp \
    ../Aquifolium/src/BTCSet.cpp \
    ../Aquifolium/src/Expression.cpp \
    ../Aquifolium/src/Matrix.cpp \
    ../Aquifolium/src/Matrix_arma.cpp \
    ../Aquifolium/src/MetaModel.cpp \
    ../Aquifolium/src/NormalDist.cpp \
    ../Aquifolium/src/Object.cpp \
    ../Aquifolium/src/Quan.cpp \
    ../Aquifolium/src/QuanSet.cpp \
    ../Aquifolium/src/QuickSort.cpp \
    ../Aquifolium/src/System.cpp \
    ../Aquifolium/src/Vector.cpp \
    ../Aquifolium/src/Vector_arma.cpp \
    ../jsoncpp/src/lib_json/json_reader.cpp \
    ../jsoncpp/src/lib_json/json_value.cpp \
    ../jsoncpp/src/lib_json/json_valueiterator.inl \
    ../jsoncpp/src/lib_json/json_writer.cpp \
    src/diagramviewer.cpp \
    src/edge.cpp \
    src/GWidget.cpp \
    src/node.cpp \
    src/utility_funcs.cpp \
    src/XString.cpp \
    src/mProp.cpp \
    src/mProplist.cpp \
    src/PropList.cpp \
    src/PropListItem.cpp \
    src/PropModel.cpp \
    src/colorScheme.cpp \
    src/treeitem.cpp \
    src/treemodel.cpp \
    src/ray.cpp \
    src/entity.cpp \
    src/plotWindow.cpp \
    src/process.cpp \
    src/gridWindow.cpp \
    src/results.cpp \
    src/helpWindow.cpp \
    src/CustomPlotZoom.cpp \
    src/qcustomplot.cpp \
    src/texToHtml.cpp \
    src/tth_source.c \
    src/scriptExport.cpp \
    src/delegate.cpp \
    src/UnitTextBox3.cpp \
    src/runtimeWindow.cpp \
    src/slndetailswindow.cpp \
    src/csvEditor.cpp \
    src/TableWidgetDateItem.cpp \
    src/logwindow.cpp \
    src/main.cpp \
    ../Aquifolium/src/Source.cpp \
    ../Aquifolium/src/Rule.cpp \
    ../Aquifolium/src/Objective_Function_Set.cpp \
    ../Aquifolium/src/Objective_Function.cpp \
    ../Aquifolium/src/Precipitation.cpp \
    ../Aquifolium/src/Condition.cpp \
    ../Aquifolium/src/Parameter_Set.cpp \
    ../Aquifolium/src/Parameter.cpp \
    ../Aquifolium/src/ErrorHandler.cpp \
    src/CCommand.cpp \
    ../Aquifolium/src/Link.cpp \
    src/expEditor.cpp \
    src/statusviewer.cpp \
    src/expressioneditor.cpp \
    ../Aquifolium/src/GA/DistributionNUnif.cpp

HEADERS += \
    include/mainwindow.h \
    ../Aquifolium/include/Block.h \
    ../Aquifolium/include/BTC.h \
    ../Aquifolium/include/BTCSet.h \
    ../Aquifolium/include/Expression.h \
    ../Aquifolium/include/Link.h \
    ../Aquifolium/include/Matrix.h \
    ../Aquifolium/include/Matrix_arma.h \
    ../Aquifolium/include/MetaModel.h \
    ../Aquifolium/include/NormalDist.h \
    ../Aquifolium/include/Object.h \
    ../Aquifolium/include/Quan.h \
    ../Aquifolium/include/QuanSet.h \
    ../Aquifolium/include/QuickSort.h \
    ../Aquifolium/include/StringOP.h \
    ../Aquifolium/include/System.h \
    ../Aquifolium/include/Vector.h \
    ../Aquifolium/include/Vector_arma.h \
    ../jsoncpp/include/json/allocator.h \
    ../jsoncpp/include/json/assertions.h \
    ../jsoncpp/include/json/autolink.h \
    ../jsoncpp/include/json/config.h \
    ../jsoncpp/include/json/features.h \
    ../jsoncpp/include/json/forwards.h \
    ../jsoncpp/include/json/json.h \
    ../jsoncpp/include/json/reader.h \
    ../jsoncpp/include/json/value.h \
    ../jsoncpp/include/json/version.h \
    ../jsoncpp/include/json/writer.h \
    ../jsoncpp/src/lib_json/json_tool.h \
    ../jsoncpp/src/lib_json/version.h.in \
    include/diagramviewer.h \
    include/edge.h \
    include/enums.h \
    include/GWidget.h \
    include/mProp.h \
    include/mProplist.h \
    include/node.h \
    include/utility_funcs.h \
    include/XString.h \
    include/PropList.h \
    include/PropListItem.h \
    include/PropModel.h \
    include/multiValues.h \
    include/logwindow.h \
    include/colorScheme.h \
    include/Command.h \
    include/treeitem.h \
    include/treemodel.h \
    include/ray.h \
    include/entity.h \
    include/plotWindow.h \
    include/process.h \
    include/gridWindow.h \
    include/results.h \
    include/helpWindow.h \
    include/CustomPlotZoom.h \
    include/qcustomplot.h \
    include/navigationList.h \
    include/variant.h \
    include/texfonts.h \
    include/texToHtml.h \
    include/delegate.h \
    include/UnitTextBox.h \
    include/UnitTextBox2.h \
    include/UnitTextBox3.h \
    include/folders.h \
    include/runtimeWindow.h \
    include/slndetailswindow.h \
    include/csvEditor.h \
    include/helpWindow.h \
    include/TableWidgetDateItem.h \
    include/expEditor.h \
    include/statusviewer.h \
    include/expressioneditor.h

FORMS += \
    forms/mainwindow.ui \
    forms/logwindow.ui \
    forms/plotwindow.ui \
    forms/gridWindow.ui \
    forms/helpWindow.ui \
    forms/runtimeWindow.ui \
    forms/slndetailswindow.ui \
    forms/csvEditor.ui \
    forms/helpWindow.ui

# LAPACK — Linear Algebra PACKage lib and include locations

win32 {

    LAPACK_INCLUDE = $$PWD/include
    #64 bits build
    contains(QMAKE_TARGET.arch, x86_64) {
        #debug
        CONFIG(debug, debug|release) {
            LAPACK_LIB_DIR = $$PWD/libs/lapack-blas_lib_win64/debug
            LIBS +=  -L$${LAPACK_LIB_DIR} -llapack_win64_MTd \
                    -lblas_win64_MTd
        }
        #release
        CONFIG(release, debug|release) {
            LAPACK_LIB_DIR = $$PWD/libs/lapack-blas_lib_win64/release
            LIBS +=  -L$${LAPACK_LIB_DIR} -llapack_win64_MT \
                    -lblas_win64_MT
        }
    }

    INCLUDEPATH += $${LAPACK_INCLUDE}

    DEFINES += ARMA_USE_LAPACK ARMA_USE_BLAS

}

linux {
    #sudo apt-get install libblas-dev liblapack-dev
     DEFINES += ARMA_USE_LAPACK ARMA_USE_BLAS
     LIBS += -larmadillo -llapack -lblas
}

