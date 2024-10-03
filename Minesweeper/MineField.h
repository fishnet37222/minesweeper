// Copyright (c) 2024 David A. Frischknecht
//
// SPDX-License-Identifier: Apache-2.0

#pragma once
#include <wx/wx.h>

class MineField final : public wxControl
{
public:
	explicit MineField(wxWindow* parent, wxWindowID id = wxID_ANY);
	[[maybe_unused]] [[nodiscard]] wxSize GetCellSize() const {return m_cellSize;}
	[[maybe_unused]] void SetCellSize(wxSize cellSize);
	[[maybe_unused]] [[nodiscard]] wxSize GetGridSize() const {return m_gridSize;}
	
	[[maybe_unused]] void SetGridSize(wxSize gridSize);
	
private:
	wxSize m_cellSize{20, 20};
	wxSize m_gridSize{9, 9};
	
	void MineField_OnPaint([[maybe_unused]] wxPaintEvent& event);
	[[nodiscard]] wxSize DoGetBestClientSize() const override;
};
