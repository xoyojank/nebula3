//-----------------------------------------------------------------------------
//  nmaxmaterial_ui.h
//
//  (c)2004 Kim, Hyoun Woo
//-----------------------------------------------------------------------------
#ifndef N_MAXMATERIAL_UI_H
#define N_MAXMATERIAL_UI_H

#include "tinyxml/tinyxml.h"

nString AddSpinner(const nString &shdName, const nString &shaderHandler, bool isFloat, TiXmlElement* elemParam);
nString AddCheckBox(const nString &shdName, const nString &shaderHandler, TiXmlElement* elemParam);
nString AddDropdownList(const nString &shdName, const nString &shaderHandler, TiXmlElement* elemParam);
nString AddColorPicker(TiXmlElement* elemParam);
nString AddMapButton(TiXmlElement* elemParam, nString defaultValue = "");
nString AddVector4Spinner(const nString &shdName, const nString &shaderHandler, TiXmlElement* elemParam);
nString AddLabel(const nString &uiname, const nString &caption, int across = 1, bool addDot = false);
nString AddSetDirDlg(TiXmlElement* elemParam);
nString AddEnvelopeCurve(const nString &shdName, const nString &shaderHandler, TiXmlElement* elemParam);


nString GetDefault(TiXmlElement* elemParam);

#define USE_ACTIVEX_ENVELOPECURVE_CTRL 1

#endif
