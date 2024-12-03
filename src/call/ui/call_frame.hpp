//
// Created by croshaw on 12/2/24.
//

#ifndef CALL_UI_CALL_FRAME_HPP
#define CALL_UI_CALL_FRAME_HPP

#include <wx/wxprec.h>

#ifndef WX_PRECOMP

#include <wx/wx.h>

#endif

#include <wx/richtext/richtextctrl.h>
#include <wx/notebook.h>
#include "ctrl/codeRichTextCtrl.hpp"
#include "ctrl/richTextCtrlBuffer.hpp"
#include "call/core/call.hpp"
#include <wx/stc/stc.h>

namespace cui {
    enum {
        ID_OPEN_FILE,
        ID_RUN_PROG,
        ID_DEBUG_PROG
    };

    class call_frame : public wxFrame {
    public:
        call_frame();

    private:
        const int TERMINAL_HEIGHT = 200;
        const int TABLE_VIEW_WIDTH = 250;
        const int TREE_VIEW_WIDTH = 250;

        wxRichTextCtrl *currentTab;

        wxMenu *menuFile;
        wxToolBar *leftToolBar;
        wxRichTextCtrl *tableView;
        wxNotebook *codeView;
        wxRichTextCtrl *treeView;
        wxStyledTextCtrl *terminalView;

        void SetupFrame();

        void SetupMenu();

        void SetupLeftBar();

        void SetupMain();

        void SetupCodeView();

        void SetupTableView();

        void SetupTreeView();

        void OnLeftToolClicked(wxCommandEvent &event);

        void OnExit(wxCommandEvent &event);

        void OnMenuItemClick(wxEvent &event);
    };
}


#endif //CALL_UI_CALL_FRAME_HPP
