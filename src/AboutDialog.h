/* -*- Mode: C++; coding: utf-8; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4 -*- */
/* AboutDialog.h
 * This file is part of LibreScribe.
 *
 * LibreScribe is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * LibreScribe is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LibreScribe.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __LIBRESCRIBE_ABOUT_DIALOG_H__
#define __LIBRESCRIBE_ABOUT_DIALOG_H__

#ifndef WX_PRECOMP
	//(*HeadersPCH(AboutDialog)
#   include <wx/dialog.h>
#   include <wx/sizer.h>
#   include <wx/stattext.h>
#   include <wx/statbmp.h>
	//*)
#endif
//(*Headers(AboutDialog)
#include <wx/statline.h>
//*)

class AboutDialog: public wxDialog
{
	public:

		AboutDialog(wxWindow* parent,wxWindowID id=wxID_ANY);
		virtual ~AboutDialog();

		//(*Declarations(AboutDialog)
		wxStaticText* libreScribeVersionLabel;
		wxStaticBitmap* libreScribeLogoBitmap;
		wxStaticText* descriptionText;
		wxStaticLine* aboutDialogSeparator;
		//*)

	protected:

		//(*Identifiers(AboutDialog)
		static const long idLogoBitmap;
		static const long idLibreScribeVersionLabel;
		static const long idAboutDialogSeparator;
		static const long idDescriptionText;
		//*)

	private:

		//(*Handlers(AboutDialog)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif // __LIBRESCRIBE_ABOUT_DIALOG_H__

