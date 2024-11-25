//
// Created by croshaw on 11/25/24.
//

// You may need to build the project (run Qt uic code generator) to get "ui_lexer.h" resolved

#include "lexer.h"
#include "ui_lexer.h"


lexer::lexer(QWidget *parent) :
        QWidget(parent), ui(new Ui::lexer) {
    ui->setupUi(this);
}

lexer::~lexer() {
    delete ui;
}
