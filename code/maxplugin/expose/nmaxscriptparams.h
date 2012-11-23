//------------------------------------------------------------------------------
//  nmaxscriptparams.h
//
//  (C)2004 Kim, Hyoun Woo.
//------------------------------------------------------------------------------
#ifndef N_MAXSCRIPTPARAMS_H
#define N_MAXSCRIPTPARAMS_H

#include "expose/nmaxscriptobjattr.h"

void AddFloatParam(const nObjectAttribute::Command*, nCmd*, nArg*, nString&, nString&, nString&);
void AddVector2Param(const nObjectAttribute::Command*, nCmd*, nString&, nString&, nString&);
void AddVector3Param(const nObjectAttribute::Command*, nCmd*, nString&, nString&, nString&);
void AddVector4Param(const nObjectAttribute::Command*, nCmd*, nString&, nString&, nString&);
void AddBoolParam(const nObjectAttribute::Command*, nCmd*, nArg*, nString&, nString&, nString&);
void AddRGBParam(const nObjectAttribute::Command*, nCmd*, nString&, nString&, nString&);
void AddRGBAParam(const nObjectAttribute::Command*,nCmd*, nString&, nString&, nString&);
void AddStringParam(const nObjectAttribute::Command*, nCmd*, nArg*, nString&, nString&, nString&);
void AddIntParam(const nObjectAttribute::Command*, nCmd*, nArg*, nString&, nString&,nString&);

#endif

