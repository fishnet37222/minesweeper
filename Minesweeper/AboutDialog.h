// Copyright (c) 2024 David A. Frischknecht
//
// SPDX-License-Identifier: Apache-2.0

#pragma once
#include <wx/wx.h>
#include <wx/webview.h>

class AboutDialog final : public wxDialog
{
public:
	[[maybe_unused]] explicit AboutDialog(wxWindow* parent);

private:
	wxWebView* m_wvLicense{};

	void WebView_OnNavigating(wxWebViewEvent& event);
};
