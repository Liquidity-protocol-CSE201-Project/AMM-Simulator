QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets charts

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AccountListWidgetItem.cpp \
    MetricsTable.cpp \
    MintDialog.cpp \
    ProvideDialog.cpp \
    TokenListWidgetItem.cpp \
    WalletListWidgetItem.cpp \
    main.cpp \
    MainWindow.cpp \
    src/Utilities/Account.cpp\
    src/Utilities/Initializer.cpp\
    src/Utilities/Operation.cpp\
    src/Utilities/PoolInterface.cpp\
    src/Utilities/Token.cpp

HEADERS += \
    AccountListWidgetItem.h \
    MainWindow.h \
    MetricsTable.h \
    MintDialog.h \
    ProvideDialog.h \
    TokenListWidgetItem.h \
    WalletListWidgetItem.h \
    src/Utilities/Utilities.hpp \
    src/Protocols/BalancerPool.hpp \
    src/Protocols/ConstantSum.hpp \
    src/Protocols/UniswapV2Pool.hpp \
    src/Protocols/UniswapV3Pool.hpp

FORMS += \
    AccountListWidgetItem.ui \
    MainWindow.ui \
    MetricsTable.ui \
    MintDialog.ui \
    ProvideDialog.ui \
    TokenListWidgetItem.ui \
    WalletListWidgetItem.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
