//
// Created by croshaw on 12/2/24.
//
#include "call_frame.hpp"


cui::call_frame::call_frame() : wxFrame(NULL, wxID_ANY, "CIDE") {
    SetupFrame();

//    auto *richTextCtrl = new line_number_panel(
//            this, wxID_ANY, wxEmptyString, wxDefaultPosition,
//            wxDefaultSize, wxVSCROLL | wxHSCROLL | wxBORDER_NONE | wxWANTS_CHARS, wxDefaultValidator,
//            wxTextCtrlNameStr);


    Bind(wxEVT_MENU, &call_frame::OnExit, this, wxID_EXIT);
}

void cui::call_frame::OnExit(wxCommandEvent &event) {
    Close(true);
}

void cui::call_frame::SetupFrame() {
    SetupMenu();
    SetupLeftBar();
    SetupMain();

//    mainPanel->Add(leftToolBar);

}

void cui::call_frame::SetupLeftBar() {
    leftToolBar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxSize(20, -1),
                                wxTB_NOICONS | wxTB_VERTICAL | wxTB_HORZ_TEXT | wxTB_FLAT);
    leftToolBar->AddTool(wxID_HIGHEST + 1, "TableView", wxBitmapBundle());
    leftToolBar->AddTool(wxID_HIGHEST + 2, "TreeView", wxBitmapBundle());
    leftToolBar->AddTool(wxID_HIGHEST + 3, "Terminal", wxBitmapBundle());
    leftToolBar->Realize();
    SetToolBar(leftToolBar);

    Bind(wxEVT_TOOL, &call_frame::OnLeftToolClicked, this, wxID_HIGHEST + 1);
    Bind(wxEVT_TOOL, &call_frame::OnLeftToolClicked, this, wxID_HIGHEST + 2);
    Bind(wxEVT_TOOL, &call_frame::OnLeftToolClicked, this, wxID_HIGHEST + 3);
}

void cui::call_frame::SetupMenu() {
    menuFile = new wxMenu();
    menuFile->Append(ID_OPEN_FILE, "&Open file...\tCtrl-O",
                     "Open file to edit them");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    auto *menuRun = new wxMenu();
    menuRun->Append(ID_RUN_PROG, "&Run...\tCtrl-R",
                    "Run program");
//    menuRun->Append(ID_OPEN_FILE + 2, "&Debug...\tF5",
//                    "Debug program");

    auto *menuBar = new wxMenuBar();
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuRun, "&Run");

    Bind(wxEVT_MENU, &call_frame::OnMenuItemClick, this);
    SetMenuBar(menuBar);
}

void cui::call_frame::SetupMain() {
    SetupCodeView();

    terminalView = new wxStyledTextCtrl(
            this, wxID_ANY, wxDefaultPosition,
            wxDefaultSize, wxVSCROLL | wxHSCROLL | wxBORDER_NONE | wxWANTS_CHARS,
            wxTextCtrlNameStr);

    tableView = new wxRichTextCtrl(
            this, wxID_ANY, wxEmptyString, wxDefaultPosition,
            wxDefaultSize, wxVSCROLL | wxHSCROLL | wxBORDER_NONE | wxWANTS_CHARS, wxDefaultValidator,
            wxTextCtrlNameStr);
    treeView = new wxRichTextCtrl(
            this, wxID_ANY, wxEmptyString, wxDefaultPosition,
            wxDefaultSize, wxVSCROLL | wxHSCROLL | wxBORDER_NONE | wxWANTS_CHARS, wxDefaultValidator,
            wxTextCtrlNameStr);

    wxFlexGridSizer *mainSizer = new wxFlexGridSizer(2, 1, 5, 5);
    mainSizer->AddGrowableRow(0);
    mainSizer->AddGrowableCol(0);

    wxFlexGridSizer *secondSizer = new wxFlexGridSizer(1, 3, 5, 5);
    secondSizer->AddGrowableCol(1);
    secondSizer->AddGrowableRow(0);

    secondSizer->Add(tableView, 1, wxEXPAND, 5);
    secondSizer->Add(codeView, 1, wxEXPAND, 5);
    secondSizer->Add(treeView, 1, wxEXPAND, 5);
    secondSizer->SetItemMinSize(tableView, TABLE_VIEW_WIDTH, -1);
    secondSizer->SetItemMinSize(treeView, TREE_VIEW_WIDTH, -1);

    mainSizer->Add(secondSizer, 1, wxEXPAND | wxALL, 5);
    mainSizer->Add(terminalView, 1, wxEXPAND, 5);
    mainSizer->SetItemMinSize(terminalView, -1, TERMINAL_HEIGHT);
    tableView->Hide();
    treeView->Hide();
    SetSizerAndFit(mainSizer);
}

void cui::call_frame::OnLeftToolClicked(wxCommandEvent &event) {
    switch (event.GetId()) {
        case wxID_HIGHEST + 1:
            tableView->Show(!tableView->IsShown());
            break;
        case wxID_HIGHEST + 2:
            treeView->Show(!treeView->IsShown());
            break;
        case wxID_HIGHEST + 3:
            terminalView->Show(!terminalView->IsShown());
            break;
        default:
            wxMessageBox("Unknown Button Clicked", "Error");
            return;
    }
    Layout();
}

void cui::call_frame::SetupCodeView() {
    codeView = new wxNotebook(this, wxID_ANY);
    wxPanel *panel1 = new wxPanel(codeView, wxID_ANY);
    auto *test = new codeRichTextCtrl(
            panel1, wxID_ANY, wxEmptyString, wxDefaultPosition,
            wxDefaultSize, wxVSCROLL | wxHSCROLL | wxBORDER_NONE | wxWANTS_CHARS, wxDefaultValidator,
            wxTextCtrlNameStr);
    currentTab = test;
    auto *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(test, 1, wxEXPAND | wxALL, 0);
    panel1->SetSizer(sizer);
    codeView->AddPage(panel1, "Untitled");
}

void cui::call_frame::OnMenuItemClick(wxEvent &event) {
    switch (event.GetId()) {
        case ID_OPEN_FILE:
            std::cout << "OpenFile\n";
            break;
        case ID_RUN_PROG: {
            terminalView->ClearAll();
            std::streambuf *originalCoutBuffer = std::cout.rdbuf();
            std::streambuf *originalCerrBuffer = std::cerr.rdbuf();
            try {
                if (!currentTab) {
                    wxLogError("RichTextCtrl not found in the current sizer.");
                    return;
                }

                // Получаем текст из richTextCtrl
                wxString code = currentTab->GetValue();
                // Создаем объект настроек
                call::settings set = call::DEFAULT;
                std::string source = code.ToStdString();
                // Запускаем процесс
                call::call callProcessor(source, set);
                richTextCtrlBuffer *_coutBuffer = new richTextCtrlBuffer(terminalView,
                                                                         *wxBLACK);
                richTextCtrlBuffer *_cerrBuffer = new richTextCtrlBuffer(terminalView,
                                                                         *wxRED);

                // Создаём потоки с этими буферами
                std::ostream *_coutStream = new std::ostream(_coutBuffer);
                std::ostream *_cerrStream = new std::ostream(_cerrBuffer);

                // Перенаправляем std::cout и std::cerr
                std::cout.rdbuf(_coutStream->rdbuf());
                std::cerr.rdbuf(_cerrStream->rdbuf());
                callProcessor.run();
                std::cout << "Process finished with exit code 0\n";
                delete _coutStream;
                delete _cerrStream;
                delete _coutBuffer;
                delete _cerrBuffer;
            } catch (const std::exception &ex) {
                wxLogError("An error occurred: %s", ex.what());
            }
            std::cout.rdbuf(originalCoutBuffer);
            std::cerr.rdbuf(originalCerrBuffer);
            break;
        }
        default:
            std::cout << "Menu Error";
    }
}
