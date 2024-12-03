//
// Created by croshaw on 12/2/24.
//

#ifndef CALL_UI_CODE_RICH_TEXT_CTRL_HPP
#define CALL_UI_CODE_RICH_TEXT_CTRL_HPP

#include <wx/richtext/richtextctrl.h>
#include <map>

namespace cui {
    class codeRichTextCtrl : public wxRichTextCtrl {
    public:
        const std::map<char, char> PAIRS = {{'(',  ')'},
                                            {'[',  ']'},
                                            {'{',  '}'},
                                            {'"',  '"'},
                                            {'\'', '\''},};

        codeRichTextCtrl(wxWindow *parent, wxWindowID id = -1, const wxString &value = wxEmptyString,
                         const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize,
                         long style = wxRE_MULTILINE, const wxValidator &validator = wxDefaultValidator,
                         const wxString &name = wxASCII_STR(wxTextCtrlNameStr))
                : wxRichTextCtrl(parent, id, value, pos, size, style, validator, name) {
            Bind(wxEVT_KEY_DOWN, &codeRichTextCtrl::OnKeyDown, this);
            Bind(wxEVT_TEXT, &codeRichTextCtrl::OnTextChanged, this);
        }

    private:
        bool shouldInsertBracket;
//        std::vector<wxString> suggestions;

        void OnKeyDown(wxKeyEvent &event) {
            long pos = GetInsertionPoint();
            wxString currentText = GetValue();
            if (event.GetKeyCode() == wxKeyCode::WXK_BACK) {
                if (pos == 0) {
                    event.Skip();
                    return;
                }
                for (const auto &item: PAIRS) {
                    if (currentText[pos - 1] == item.first && currentText[pos] == item.second) {
                        SetValue((pos >= 2 ? currentText.substr(0, pos - 1) : "") +
                                 (currentText.size() > pos + 1 ? currentText.substr(pos + 1) : ""));
                        SetInsertionPoint(pos - 1);
                        return;
                    }
                }
            }
            auto ch = event.GetUnicodeKey();
            for (const auto &item: PAIRS) {
                if (item.first == ch) {
                    shouldInsertBracket = true;
                    break;
                } else if (item.second == ch) {
                    if (currentText[pos] == ch) {
                        SetInsertionPoint(pos + 1);
                        return;
                    }
                }
            }
            event.Skip();
        }

        void OnTextChanged(wxCommandEvent &event) {
            if (!shouldInsertBracket) {
                return;
            }
            shouldInsertBracket = false;
            long pos = GetInsertionPoint();
            wxString currentText = GetValue();

            if (pos > 0) {
                auto ch = currentText[pos - 1];
                if (PAIRS.contains(ch)) {
                    ChangeValue(currentText.substr(0, pos) + PAIRS.at(ch) + currentText.substr(pos));
                    SetInsertionPoint(pos);
                }
            }
        }
    };
}


#endif //CALL_UI_CODE_RICH_TEXT_CTRL_HPP
