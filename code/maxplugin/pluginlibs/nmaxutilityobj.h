//------------------------------------------------------------------------------
//  nmaxutilityobj.h
//
//  (c)2004 Kim, Hyoun Woo
//------------------------------------------------------------------------------
#ifndef N_MAXUTILITYOBJ_H
#define N_MAXUTILITYOBJ_H
//------------------------------------------------------------------------------
/**
    @class nMaxUtilityObj
    @ingroup NebulaMaxExport2Contrib

    @brief Nebula2 utility object plugin.
*/
#include "Max.h"
#include "notify.h"
#include "utilapi.h"
#include "istdplug.h"
#include "base/nmaxdll.h"

#define N_MAX_UTILITY_CLASS_ID    Class_ID(0x697f7c56, 0x159066d5)

#define ROLLUP_EXPORTOPTIONS_OPEN (1<<0)
//#define ROLLUP__OPTN (1<<0)
//#define ROLLUP__OPTN (1<<0)

//------------------------------------------------------------------------------
class nMaxUtilityObj : public UtilityObj, public IParamArray
{
public:
    nMaxUtilityObj();
    virtual ~nMaxUtilityObj();

    /// @name 3dsmax functions should be defined.
    /// @{
    void BeginEditParams(Interface* intf, IUtil* iutil);
    void EndEditParams(Interface* intf, IUtil* iutil);
    void DeleteThis() { /*Do not delete this utilityobj*/ }
    /// @}

protected:
    static IParamMap* optionParamMap;

    int flags;

};
//------------------------------------------------------------------------------
// ClassDesc2 defination for 'nMaxExpose' class
static tRegisteredClassDesc<nMaxUtilityObj> maxUtilityObjInterfaceClassDesc2(
    "Nebula2",      // class name
    N_MAX_UTILITY_CLASS_ID, // class ID
    UTILITY_CLASS_ID, // super class ID
    1);//,                 // 1: public, 0: non-public
    //_T(""),              // category
    //"nebula2");          // scripter-visible name

#endif
