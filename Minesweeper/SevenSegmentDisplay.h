// Copyright (c) 2024 David A. Frischknecht
//
// SPDX-License-Identifier: Apache-2.0

#pragma once
#include <wx/wx.h>
#include <unordered_map>
#include <vector>

class SevenSegmentDisplay final : public wxControl
{
public:
	explicit SevenSegmentDisplay(wxWindow* parent, wxWindowID id = wxID_ANY);
	[[nodiscard]] int GetValue() const {return m_value;}
	void SetValue(int value);
	[[nodiscard]] int GetDigitSpacing() const {return m_digitSpacing;}
	void SetDigitSpacing(int digitSpacing);
	[[nodiscard]] int GetDigitCount() const {return m_digitCount;}
	void SetDigitCount(int digitCount);
	[[nodiscard]] int GetSegmentThickness() const {return m_segmentThickness;}
	void SetSegmentThickness(int segmentThickness);
	[[nodiscard]] wxSize GetDigitSize() const {return m_digitSize;}
	void SetDigitSize(wxSize digitSize);
	[[nodiscard]] bool GetLeadingZerosVisible() const {return m_leadingZerosVisible;}
	void SetLeadingZerosVisible(bool leadingZerosVisible);
	
private:
#define SEGMENT_LIST TOP,TOP_LEFT,TOP_RIGHT,MIDDLE,BOTTOM_LEFT,BOTTOM_RIGHT,BOTTOM
	enum Segment {SEGMENT_LIST};
	static std::unordered_map<wchar_t, std::vector<Segment>> m_digitSegments;
	
	int m_value{0};
	int m_digitSpacing{5};
	int m_digitCount{3};
	int m_segmentThickness{3};
	wxSize m_digitSize{15,30};
	bool m_leadingZerosVisible{false};
	
	void SevenSegmentDisplay_OnPaint(wxPaintEvent& event);
	[[nodiscard]] wxSize DoGetBestClientSize() const override;
};
