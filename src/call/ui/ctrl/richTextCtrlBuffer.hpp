//
// Created by croshaw on 12/2/24.
//

#ifndef CALL_UI_STREAM_RICH_TEXT_CTRL_HPP
#define CALL_UI_STREAM_RICH_TEXT_CTRL_HPP

//#include <wx/richtext/richtextctrl.h>
#include <wx/stc/stc.h>

class richTextCtrlBuffer : public std::streambuf {
public:
    explicit richTextCtrlBuffer(wxStyledTextCtrl *richTextCtrl, const wxColour &textColor = *wxBLACK) : _richTextCtrl(
            richTextCtrl), _textColor(textColor) {}

protected:
    // Перехватываем символы, записанные в поток
    virtual int overflow(int c) override {
        if (c != EOF) {
            _buffer += static_cast<char>(c);
            if (c == '\n') {
                flushBuffer();
            }
        }
        return c;
    }

    virtual int sync() override {
        flushBuffer();
        return 0;
    }

private:
    std::string _buffer;
    wxStyledTextCtrl *_richTextCtrl;
    wxColour _textColor;

    void flushBuffer() {
        if (!_buffer.empty()) {
            int style = 1; // Индекс стиля текста
            _richTextCtrl->StyleSetForeground(style, _textColor); // Устанавливаем цвет текста

            // Добавляем текст и применяем стиль
            int startPos = _richTextCtrl->GetLength();
            _richTextCtrl->AppendText(_buffer); // Добавляем текст
            _richTextCtrl->StartStyling(startPos); // Указываем начальную позицию и маску стиля
            _richTextCtrl->SetStyling(_buffer.size(), style); // Применяем стиль к добавленному тексту

            _buffer.clear(); // Очищаем буфер
        }
    }
};


#endif //CALL_UI_STREAM_RICH_TEXT_CTRL_HPP
