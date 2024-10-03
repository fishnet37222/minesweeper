// Copyright (c) 2024 David A. Frischknecht
//
// SPDX-License-Identifier: Apache-2.0

#include "pch.h"
#include "MineField.h"
#include <wx/dcbuffer.h>
#include <wx/graphics.h>

MineField::MineField(wxWindow* parent, wxWindowID id)
{
	wxControl::SetBackgroundStyle(wxBG_STYLE_PAINT);
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE | wxBORDER_NONE);
	wxControl::SetBackgroundColour(wxTheColourDatabase->Find(L"LIGHT GREY"));
	Bind(wxEVT_PAINT, &MineField::MineField_OnPaint, this);
}

[[maybe_unused]] void MineField::SetCellSize(wxSize cellSize)
{
	if (m_cellSize == cellSize) return;

	m_cellSize = cellSize;
	InvalidateBestSize();
	Refresh();
}

[[maybe_unused]] void MineField::SetGridSize(wxSize gridSize)
{
	if (m_gridSize == gridSize) return;

	m_gridSize = gridSize;
	InvalidateBestSize();
	Refresh();
}

void MineField::MineField_OnPaint([[maybe_unused]] wxPaintEvent& event)
{
	const wxAutoBufferedPaintDC dc(this);
	auto* gc = wxGraphicsContext::Create(dc);
	gc->SetAntialiasMode(wxANTIALIAS_DEFAULT);

	gc->SetBrush(GetBackgroundColour());
	gc->SetPen(GetBackgroundColour());

	gc->DrawRectangle(0, 0, GetClientSize().GetWidth(), GetClientSize().GetHeight());

	const auto highlightColor = GetBackgroundColour().ChangeLightness(150);
	const auto shadowColor = GetBackgroundColour().ChangeLightness(50);

	for (auto gridY = 0; gridY < m_gridSize.GetHeight(); gridY++)
	{
		const auto cellOriginY = m_cellSize.GetHeight() * gridY;
		for (auto gridX = 0; gridX < m_gridSize.GetWidth(); gridX++)
		{
			const auto cellOriginX = m_cellSize.GetWidth() * gridX;

			for (auto i = 0; i < 3; i++)
			{
				gc->SetPen(highlightColor);
				gc->StrokeLine(cellOriginX + i, cellOriginY + i, cellOriginX + m_cellSize.GetWidth() - i, cellOriginY + i);
				gc->StrokeLine(cellOriginX + i, cellOriginY + i, cellOriginX + i, cellOriginY + m_cellSize.GetHeight() - i);

				gc->SetPen(shadowColor);
				gc->StrokeLine(cellOriginX + m_cellSize.GetWidth() - i, cellOriginY + i,
					cellOriginX + m_cellSize.GetWidth() - i, cellOriginY + m_cellSize.GetHeight() - i);
				gc->StrokeLine(cellOriginX + i, cellOriginY + m_cellSize.GetHeight() - i,
					cellOriginX + m_cellSize.GetWidth() - i, cellOriginY + m_cellSize.GetHeight() - i);
			}
		}
	}

	delete gc;
}

wxSize MineField::DoGetBestClientSize() const
{
	return { m_cellSize.GetWidth() * m_gridSize.GetWidth() + 1, m_cellSize.GetHeight() * m_gridSize.GetHeight() + 1 };
}
