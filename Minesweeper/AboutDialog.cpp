// Copyright (c) 2024 David A. Frischknecht
//
// SPDX-License-Identifier: Apache-2.0

#include "pch.h"
#include "AboutDialog.h"

#include "LICENSE-2.0-html.h"

[[maybe_unused]] AboutDialog::AboutDialog(wxWindow* parent) : wxDialog(parent, wxID_ANY, L"About Minesweeper")
{
	wxDialog::SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_FRAMEBK));

	auto* szrMainOuter = new wxBoxSizer(wxHORIZONTAL);
	szrMainOuter->AddSpacer(12);
	auto* szrMainInner = new wxBoxSizer(wxVERTICAL);
	szrMainInner->AddSpacer(12);
	szrMainInner->Add(new wxStaticText(this, wxID_ANY, L"Minesweeper 1.0"), wxSizerFlags(0).CenterHorizontal());
	szrMainInner->AddSpacer(5);
	szrMainInner->Add(new wxStaticText(this, wxID_ANY, L"Copyright \u00A9 2024, David A. Frischknecht"),
		wxSizerFlags(0).CenterHorizontal());
	szrMainInner->AddSpacer(5);
	m_wvLicense = wxWebView::New(this, wxID_ANY);
	m_wvLicense->SetPage(license_html, wxEmptyString);
	m_wvLicense->SetMinSize({ 600, 300 });
	m_wvLicense->Bind(wxEVT_WEBVIEW_NAVIGATING, &AboutDialog::WebView_OnNavigating, this);
	szrMainInner->Add(m_wvLicense, wxSizerFlags(1).Expand());
	szrMainInner->AddSpacer(5);
	auto* szrButtons = CreateButtonSizer(wxCLOSE);
	szrMainInner->Add(szrButtons, wxSizerFlags(0).Expand());
	szrMainInner->AddSpacer(12);
	szrMainOuter->Add(szrMainInner, wxSizerFlags(1).Expand());
	szrMainOuter->AddSpacer(12);
	SetSizerAndFit(szrMainOuter);

	CenterOnParent();
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void AboutDialog::WebView_OnNavigating(wxWebViewEvent& event)
{
	if (event.GetURL().StartsWith(L"data:"))
	{
		event.Skip();
		return;
	}

	event.Veto();
	wxLaunchDefaultBrowser(event.GetURL());
}
