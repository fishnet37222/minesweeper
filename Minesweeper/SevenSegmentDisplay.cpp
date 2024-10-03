// Copyright (c) 2024 David A. Frischknecht
//
// SPDX-License-Identifier: Apache-2.0

#include "pch.h"
#include "SevenSegmentDisplay.h"
#include <wx/dcbuffer.h>
#include <wx/graphics.h>
#include <algorithm>

std::unordered_map<wchar_t, std::vector<SevenSegmentDisplay::Segment>> SevenSegmentDisplay::m_digitSegments
{
		{L' ', {}},
		{L'0', {TOP, TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT, BOTTOM}},
		{L'1', {TOP_RIGHT, BOTTOM_RIGHT}},
		{L'2', {TOP, TOP_RIGHT, MIDDLE, BOTTOM_LEFT, BOTTOM}},
		{L'3', {TOP, TOP_RIGHT, MIDDLE, BOTTOM_RIGHT, BOTTOM}},
		{L'4', {TOP_LEFT, MIDDLE, TOP_RIGHT, BOTTOM_RIGHT}},
		{L'5', {TOP, TOP_LEFT, MIDDLE, BOTTOM_RIGHT, BOTTOM}},
		{L'6', {TOP, TOP_LEFT, BOTTOM_LEFT, BOTTOM, BOTTOM_RIGHT, MIDDLE}},
		{L'7', {TOP, TOP_RIGHT, BOTTOM_RIGHT}},
		{L'8', {SEGMENT_LIST}},
		{L'9', {BOTTOM, BOTTOM_RIGHT, TOP_RIGHT, TOP, TOP_LEFT, MIDDLE}},
};

SevenSegmentDisplay::SevenSegmentDisplay(wxWindow* parent, wxWindowID id)
{
	wxWindowBase::SetBackgroundStyle(wxBG_STYLE_PAINT);
	wxControl::Create(parent, id, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE | wxBORDER_NONE);
	Bind(wxEVT_PAINT, &SevenSegmentDisplay::SevenSegmentDisplay_OnPaint, this);
	wxWindowBase::SetBackgroundColour(wxTheColourDatabase->Find(L"BLACK"));
	wxWindowBase::SetForegroundColour(wxTheColourDatabase->Find(L"YELLOW"));
}

void SevenSegmentDisplay::SevenSegmentDisplay_OnPaint([[maybe_unused]] wxPaintEvent& event)
{
	const wxAutoBufferedPaintDC dc{ this };
	auto* gc = wxGraphicsContext::Create(dc);
	gc->SetAntialiasMode(wxANTIALIAS_DEFAULT);

	gc->SetBrush(GetParent()->GetBackgroundColour());
	gc->SetPen(GetParent()->GetBackgroundColour());

	gc->DrawRectangle(0, 0, GetClientSize().GetWidth(), GetClientSize().GetHeight());

	const auto litColor = GetForegroundColour();
	const auto unlitColor = litColor.ChangeLightness(25);

	gc->SetBrush(GetBackgroundColour());
	gc->SetPen(GetBackgroundColour());

	gc->DrawRoundedRectangle(0, 0, GetClientSize().GetWidth() - 1, GetClientSize().GetHeight() - 1, 4);

	auto valueString = std::to_wstring(m_value);
	if (valueString.length() < m_digitCount)
	{
		valueString = std::wstring(m_digitCount - valueString.length(), m_leadingZerosVisible ? L'0' : L' ') + valueString;
	}
	if (valueString.length() > m_digitCount)
	{
		valueString = valueString.substr(valueString.length() - m_digitCount - 1);
	}

	std::vector<std::tuple<wxGraphicsPath, bool>> paths{};
	const auto halfHeight = m_digitSize.GetHeight() / 2;

	for (auto i = 0; i < m_digitCount; i++)
	{
		const auto digit = valueString[i];
		const auto& segments = m_digitSegments[digit];
		const auto digitOriginX = m_digitSpacing + ((m_digitSize.GetWidth() + m_digitSpacing) * i);
		const auto digitOriginY = m_digitSpacing;

		const auto segmentList = { SEGMENT_LIST };
		for (const auto segment : segmentList)
		{
			auto path = gc->CreatePath();

			switch (segment)
			{
				case TOP:
				{
					path.MoveToPoint(digitOriginX + 1, digitOriginY);
					path.AddLineToPoint(digitOriginX + m_digitSize.GetWidth() - 1, digitOriginY);
					path.AddLineToPoint(digitOriginX + m_digitSize.GetWidth() - m_segmentThickness - 1, digitOriginY + m_segmentThickness);
					path.AddLineToPoint(digitOriginX + m_segmentThickness + 1, digitOriginY + m_segmentThickness);
					path.CloseSubpath();
					break;
				}

				case TOP_LEFT:
				{
					path.MoveToPoint(digitOriginX, digitOriginY + 1);
					path.AddLineToPoint(digitOriginX + m_segmentThickness, digitOriginY + m_segmentThickness + 1);
					path.AddLineToPoint(digitOriginX + m_segmentThickness, digitOriginY + halfHeight - m_segmentThickness - 1);
					path.AddLineToPoint(digitOriginX, digitOriginY + halfHeight - 1);
					path.CloseSubpath();
					break;
				}

				case TOP_RIGHT:
				{
					path.MoveToPoint(digitOriginX + m_digitSize.GetWidth(), digitOriginY + 1);
					path.AddLineToPoint(digitOriginX + m_digitSize.GetWidth() - m_segmentThickness, digitOriginY + m_segmentThickness + 1);
					path.AddLineToPoint(digitOriginX + m_digitSize.GetWidth() - m_segmentThickness, digitOriginY + halfHeight - m_segmentThickness - 1);
					path.AddLineToPoint(digitOriginX + m_digitSize.GetWidth(), digitOriginY + halfHeight - 1);
					path.CloseSubpath();
					break;
				}

				case MIDDLE:
				{
					path.MoveToPoint(digitOriginX + 1, digitOriginY + halfHeight);
					path.AddLineToPoint(digitOriginX + m_segmentThickness, digitOriginY + halfHeight - m_segmentThickness + 1);
					path.AddLineToPoint(digitOriginX + m_digitSize.GetWidth() - m_segmentThickness, digitOriginY + halfHeight - m_segmentThickness + 1);
					path.AddLineToPoint(digitOriginX + m_digitSize.GetWidth() - 1, digitOriginY + halfHeight);
					path.AddLineToPoint(digitOriginX + m_digitSize.GetWidth() - m_segmentThickness, digitOriginY + halfHeight + m_segmentThickness - 1);
					path.AddLineToPoint(digitOriginX + m_segmentThickness, digitOriginY + halfHeight + m_segmentThickness - 1);
					path.CloseSubpath();
					break;
				}

				case BOTTOM_LEFT:
				{
					path.MoveToPoint(digitOriginX, digitOriginY + halfHeight + 1);
					path.AddLineToPoint(digitOriginX + m_segmentThickness, digitOriginY + halfHeight + m_segmentThickness + 1);
					path.AddLineToPoint(digitOriginX + m_segmentThickness, digitOriginY + m_digitSize.GetHeight() - m_segmentThickness - 1);
					path.AddLineToPoint(digitOriginX, digitOriginY + m_digitSize.GetHeight() - 1);
					path.CloseSubpath();
					break;
				}

				case BOTTOM_RIGHT:
				{
					path.MoveToPoint(digitOriginX + m_digitSize.GetWidth(), digitOriginY + halfHeight + 1);
					path.AddLineToPoint(digitOriginX + m_digitSize.GetWidth() - m_segmentThickness, digitOriginY + halfHeight + m_segmentThickness + 1);
					path.AddLineToPoint(digitOriginX + m_digitSize.GetWidth() - m_segmentThickness, digitOriginY + m_digitSize.GetHeight() - m_segmentThickness - 1);
					path.AddLineToPoint(digitOriginX + m_digitSize.GetWidth(), digitOriginY + m_digitSize.GetHeight() - 1);
					path.CloseSubpath();
					break;
				}

				case BOTTOM:
				{
					path.MoveToPoint(digitOriginX + 1, digitOriginY + m_digitSize.GetHeight());
					path.AddLineToPoint(digitOriginX + m_digitSize.GetWidth() - 1, digitOriginY + m_digitSize.GetHeight());
					path.AddLineToPoint(digitOriginX + m_digitSize.GetWidth() - m_segmentThickness - 1, digitOriginY + m_digitSize.GetHeight() - m_segmentThickness);
					path.AddLineToPoint(digitOriginX + m_segmentThickness + 1, digitOriginY + m_digitSize.GetHeight() - m_segmentThickness);
					path.CloseSubpath();
					break;
				}

				default: break;
			}

			paths.emplace_back(path, std::ranges::find(segments.begin(), segments.end(), segment) != segments.end());
		}
	}

	for (const auto& [path, lit] : paths)
	{
		const auto color = lit ? litColor : unlitColor;
		gc->SetBrush(color);
		gc->SetPen(color);
		gc->DrawPath(path);
	}

	delete gc;
}

wxSize SevenSegmentDisplay::DoGetBestClientSize() const
{
	const auto width = (m_digitSize.GetWidth() + m_digitSpacing) * m_digitCount + m_digitSpacing;
	const auto height = m_digitSpacing * 2 + m_digitSize.GetHeight();
	return { width, height };
}

void SevenSegmentDisplay::SetValue(int value)
{
	if (m_value == value) return;

	m_value = value;
	wxWindow::Refresh();
}

void SevenSegmentDisplay::SetDigitSpacing(int digitSpacing)
{
	if (m_digitSpacing == digitSpacing) return;

	m_digitSpacing = digitSpacing;
	InvalidateBestSize();
	wxWindow::Refresh();
}

void SevenSegmentDisplay::SetDigitCount(int digitCount)
{
	if (m_digitCount == digitCount) return;

	m_digitCount = digitCount;
	InvalidateBestSize();
	wxWindow::Refresh();
}

void SevenSegmentDisplay::SetSegmentThickness(int segmentThickness)
{
	if (m_segmentThickness == segmentThickness) return;

	m_segmentThickness = segmentThickness;
	wxWindow::Refresh();
}

void SevenSegmentDisplay::SetDigitSize(wxSize digitSize)
{
	if (m_digitSize == digitSize) return;

	m_digitSize = digitSize;
	InvalidateBestSize();
	wxWindow::Refresh();
}

void SevenSegmentDisplay::SetLeadingZerosVisible(bool leadingZerosVisible)
{
	if (m_leadingZerosVisible == leadingZerosVisible) return;

	m_leadingZerosVisible = leadingZerosVisible;
	wxWindow::Refresh();
}
