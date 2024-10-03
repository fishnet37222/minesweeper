// Copyright (c) 2024 David A. Frischknecht
//
// SPDX-License-Identifier: Apache-2.0

#include "pch.h"
#include "MainWindow.h"
#include "AboutDialog.h"

#include "bitmaps/smile-1.xpm"
#include "bitmaps/smile-2.xpm"
#include "bitmaps/smile-3.xpm"
#include "bitmaps/smile-4.xpm"

enum class MenuId
{
	New = wxID_HIGHEST + 1,
	Beginner,
	Intermediate,
	Expert,
	Custom,
	BestTimes,
	Exit,
	HowToPlay,
	About
};

MainWindow::MainWindow() : wxFrame(nullptr, wxID_ANY, L"Minesweeper", wxDefaultPosition, wxDefaultSize,
	wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))
{
	wxFrame::SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_FRAMEBK));
	wxTopLevelWindowMSW::SetIcons(wxIconBundle(L"APP_ICON", nullptr));

	using enum MenuId;
	m_menuBar = new wxMenuBar();
	auto* mnuGame = new wxMenu();
	mnuGame->Append(static_cast<int>(New), L"&New\tF2");
	mnuGame->AppendSeparator();
	mnuGame->AppendCheckItem(static_cast<int>(Beginner), L"&Beginner");
	mnuGame->AppendCheckItem(static_cast<int>(Intermediate), L"&Intermediate");
	mnuGame->AppendCheckItem(static_cast<int>(Expert), L"&Expert");
	mnuGame->AppendCheckItem(static_cast<int>(Custom), L"&Custom...");
	mnuGame->AppendSeparator();
	mnuGame->Append(static_cast<int>(BestTimes), L"Best &Times");
	mnuGame->AppendSeparator();
	mnuGame->Append(static_cast<int>(Exit), L"E&xit");
	m_menuBar->Append(mnuGame, L"&Game");
	auto* mnuHelp = new wxMenu();
	mnuHelp->Append(static_cast<int>(HowToPlay), L"How To Play");
	mnuHelp->AppendSeparator();
	mnuHelp->Append(static_cast<int>(About), L"&About Minesweeper");
	m_menuBar->Append(mnuHelp, L"&Help");
	wxFrame::SetMenuBar(m_menuBar);
	m_menuBar->Bind(wxEVT_MENU, &MainWindow::MenuBar_OnItemSelect, this);

	auto* szrMainOuter = new wxBoxSizer(wxHORIZONTAL);
	szrMainOuter->AddSpacer(12);
	auto* szrMainInner = new wxBoxSizer(wxVERTICAL);
	szrMainInner->AddSpacer(5);
	auto* szrTop = new wxBoxSizer(wxHORIZONTAL);
	m_ssdMinesLeft = new SevenSegmentDisplay(this);
	szrTop->Add(m_ssdMinesLeft, wxSizerFlags(0).CenterVertical());
	szrTop->AddSpacer(5);
	szrTop->AddStretchSpacer(1);
	m_btnNewGame = new wxBitmapButton(this, wxID_ANY, wxBitmapBundle::FromBitmap(wxBitmap(smile_1_xpm)));
	szrTop->Add(m_btnNewGame, wxSizerFlags(0).CenterVertical());
	szrTop->AddStretchSpacer(1);
	szrTop->AddSpacer(5);
	m_ssdElapsedTime = new SevenSegmentDisplay(this);
	szrTop->Add(m_ssdElapsedTime, wxSizerFlags(0).CenterVertical());
	szrMainInner->Add(szrTop, wxSizerFlags(0).Expand());
	szrMainInner->AddSpacer(5);
	m_mineField = new MineField(this);
	szrMainInner->Add(m_mineField, wxSizerFlags(0).CenterHorizontal());
	szrMainInner->AddSpacer(12);
	szrMainOuter->Add(szrMainInner, wxSizerFlags(1).Expand());
	szrMainOuter->AddSpacer(12);
	SetSizerAndFit(szrMainOuter);

	CenterOnScreen();
}

void MainWindow::MenuBar_OnItemSelect(wxCommandEvent& event)
{
	const auto menuId = static_cast<MenuId>(event.GetId());

	using enum MenuId;
	switch (menuId)
	{
		case New:
		case Beginner:
		case Intermediate:
		case Expert:
		case Custom:
		case BestTimes:
			break;

		case Exit:
		{
			Close();
			break;
		}

		case HowToPlay:
			break;

		case About:
		{
			AboutDialog dialog(this);
			dialog.ShowModal();
			break;
		}
	}
}
