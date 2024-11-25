//
// Created by croshaw on 11/25/24.
//

#ifndef CALL_UI_LEXER_H
#define CALL_UI_LEXER_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class lexer; }
QT_END_NAMESPACE

class lexer : public QWidget {
Q_OBJECT

public:
    explicit lexer(QWidget *parent = nullptr);

    ~lexer() override;

private:
    Ui::lexer *ui;
};


#endif //CALL_UI_LEXER_H
