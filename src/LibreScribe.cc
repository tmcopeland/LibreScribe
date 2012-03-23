/* -*- Mode: C++; coding: utf-8; indent-tabs-mode: nil; c-basic-offset: 4; tab-width: 4 -*- */
/* LibreScribe.cc
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

#include "LibreScribe.h"
#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

//(*AppHeaders
#include "GUIFrame.h"
#include <wx/image.h>
//*)
//#include <iostream>

IMPLEMENT_APP(LibreScribe);

bool LibreScribe::OnInit() {
/*    for (int i = 0; i < this->argc; i++)
    {
        if (this->argv[i] == "--help")
        {
            std::cout << this->argv[0] << " version "
                      << LibreScribe_VERSION_MAJOR << " "
                      << LibreScribe_VERSION_MINOR << std::endl;
            
            return 0;
        }
    }/* Maybe we'll use CMake later... */
    
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if (wxsOK)
    {
        GUIFrame* Frame = new GUIFrame(0);
        Frame->Show();
        SetTopWindow(Frame);
    }
    //*)
    return wxsOK;
}

