//
// Created by croshaw on 11/25/24.
//

#include <QApplication>
#include "lexer.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    lexer mainWindow;
    mainWindow.show();
    return app.exec();
}