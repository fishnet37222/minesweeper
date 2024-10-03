// Copyright (c) 2024 David A. Frischknecht
//
// SPDX-License-Identifier: Apache-2.0

#pragma once
#include <wx/wx.h>
#include "SevenSegmentDisplay.h"
#include "MineField.h"

class MainWindow final : public wxFrame
{
public:
	MainWindow();

private:
	wxBitmapButton* m_btnNewGame{};
	wxMenuBar* m_menuBar{};
	SevenSegmentDisplay* m_ssdMinesLeft{};
	SevenSegmentDisplay* m_ssdElapsedTime{};
	MineField* m_mineField{};

	void MenuBar_OnItemSelect(wxCommandEvent& event);
};
