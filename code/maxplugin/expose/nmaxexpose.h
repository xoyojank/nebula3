#ifndef N_MAXSCRIPT_H
#define N_MAXSCRIPT_H

//-----------------------------------------------------------------------------
/**
    nmaxexpose.h

    (C) 2004 Kim, Hyoun Woo.

    nMaxExpose is licensed under the terms of the Nebula License.
*/

#include "base/nmaxdll.h"

#define N_MAX_SCRIPT_CLASS_ID       Class_ID(0x352333, 0x57664) 
#define N_MAX_SCRIPT_INTERFACE_ID   Interface_ID(0x352323, 0x55664) 

//-----------------------------------------------------------------------------
/**
    @class INebulaInterface
    @ingroup NMaxExposeContrib

    @brief Base class for function publishing system of Nebula.

    This class is interface class which provide public interfaces of published
    functions. All member functions are declared by pure virtual functions.
*/
class INebulaInterface : public FPStaticInterface
{
    virtual TCHAR* New (TCHAR* className, TCHAR* objectName) = 0;
    virtual BOOL Delete (TCHAR* objectName) = 0;
    virtual Tab<TCHAR*> Dir() = 0;
    virtual TCHAR* Sel (TCHAR* objectName) = 0;
    virtual TCHAR* Lookup (TCHAR* objectName) = 0;
    virtual BOOL Exists (TCHAR* objectName) = 0;
    virtual void NPrint (TCHAR* msg) = 0;
    virtual Tab<FPValue*>* Call(TCHAR* command, Tab<FPValue*> *val) = 0;
    virtual Tab<FPValue*>* CallV(TCHAR* command) = 0;

};

//-----------------------------------------------------------------------------
/**
    @class nMaxExpose
    @ingroup NMaxExposeContrib

    @brief Class for 3dsmax Function Publishing of Nebula.
*/
class nMaxExpose : public ReferenceTarget//, public INebulaInterface
{
public:
    nMaxExpose() {}

    void DeleteThis() { delete this; }

    int IsKeyable() { return 0; }

    FPInterfaceDesc* GetDesc();

    RefResult NotifyRefChanged(Interval i, RefTargetHandle rth, PartID& pi,RefMessage rm)
    {
        return REF_SUCCEED;
    }

    BaseInterface* GetInterface(Interface_ID id) 
    { 
        if (id == N_MAX_SCRIPT_INTERFACE_ID) 
            return (INebulaInterface*)this; 
        else 
            return ReferenceTarget::GetInterface(id); 
    }

};

// ClassDesc2 defination for 'nMaxExpose' class
static tRegisteredClassDesc<nMaxExpose> maxScriptInterfaceClassDesc2(
    "nMaxExpose",          // class name
    N_MAX_SCRIPT_CLASS_ID, // class ID
    REF_TARGET_CLASS_ID,   // super class ID
    1,                     // 1: public, 0: non-public
    _T(""),                // category
    "nebula2");            // scripter-visible name

#endif /*N_MAXSCRIPT_H*/

