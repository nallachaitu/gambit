//
// $Source$
// $Revision$
// $Date$
//
// DESCRIPTION:
// Implementation of extensive form navigation window
//
// This file is part of Gambit
// Copyright (c) 2002, The Gambit Project
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif  // WX_PRECOMP
#include "efgnavigate.h"

EfgNavigateWindow::EfgNavigateWindow(gbtGameDocument *p_game,
				     wxWindow *p_parent)
  : wxGrid(p_parent, -1, wxDefaultPosition, wxDefaultSize),
    gbtGameView(p_game)
{
  CreateGrid(10, 1);
  SetEditable(false);
  SetDefaultCellAlignment(wxCENTER, wxCENTER);

  SetLabelValue(wxVERTICAL, "Node label", 0);
  SetLabelValue(wxVERTICAL, "Pr(node reached)", 1);
  SetLabelValue(wxVERTICAL, "Node value", 2);
  SetLabelValue(wxVERTICAL, "Information set", 3);
  SetLabelValue(wxVERTICAL, "Pr(infoset reached)", 4);
  SetLabelValue(wxVERTICAL, "Belief", 5);
  SetLabelValue(wxVERTICAL, "Information set value", 6);
  SetLabelValue(wxVERTICAL, "Incoming action label", 7);
  SetLabelValue(wxVERTICAL, "Pr(incoming action)", 8);
  SetLabelValue(wxVERTICAL, "Incoming action value", 9);

  SetLabelSize(wxHORIZONTAL, 0);
  SetLabelSize(wxVERTICAL, 150);
  AdjustScrollbars();
  Show(true);
}

void EfgNavigateWindow::OnUpdate(gbtGameView *)
{
  Node *cursor = m_game->m_cursor;
  
  if (!cursor) { // no data available
    for (int i = 0; i < GetRows(); i++) { 
      SetCellValue("", i, 0);
    }
    return;
  }

  // if we got here, the node is valid.
  try {
    SetCellValue((char *) cursor->GetName(), 0, 0);
    SetCellValue((char *) m_game->GetRealizProb(cursor), 1, 0);
    SetCellValue((char *) m_game->GetNodeValue(cursor), 2, 0);

    gText tmpstr;
  
    if (!cursor->GetPlayer()) {
      tmpstr = "TERMINAL";
    }
    else {
      if (cursor->GetPlayer()->IsChance())
	tmpstr = "CHANCE";
      else
	tmpstr = ("(" + ToText(cursor->GetPlayer()->GetNumber()) + "," +
		  ToText(cursor->GetInfoset()->GetNumber()) + ")");
    }
	  
    SetCellValue((char *) tmpstr, 3, 0);
    SetCellValue((char *) m_game->GetInfosetProb(cursor), 4, 0);
    SetCellValue((char *) m_game->GetBeliefProb(cursor), 5, 0);
    SetCellValue((char *) m_game->GetInfosetValue(cursor), 6, 0);
	
    Node *p = cursor->GetParent();

    if (p) {
      int branch = 0;
      for (branch = 1; p->GetChild(branch) != cursor; branch++);

      SetCellValue((char *) cursor->GetAction()->GetName(), 7, 0);
      SetCellValue((char *) m_game->GetActionProb(p, branch), 8, 0);
      SetCellValue((char *) m_game->GetActionValue(p, branch), 9, 0);
    }
    else {
      SetCellValue("N/A (root)", 7, 0);
      SetCellValue("1", 8, 0);
      SetCellValue("N/A", 9, 0);
    }
  }	
  catch (gException &) { }
}

