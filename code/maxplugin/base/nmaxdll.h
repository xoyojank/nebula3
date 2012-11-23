//------------------------------------------------------------------------------
//  nmaxdll.h
//
//  (C)2004 Johannes Kellner
//------------------------------------------------------------------------------
#ifndef N_MAXDLL_H
#define N_MAXDLL_H
//------------------------------------------------------------------------------
/**
    @class registeredClassDesc
    @ingroup NebulaMaxExport2Contrib

    This is a wrapper to generate all needed functions an exports to create
    a 3ds max plugin.
    This includes linkedClassDesc for the automatic count and publish of all
    classes you like to publish to max.
    And the macro DefineLinkedClassDesc for the easy create of ClassDesc's for
    the export.

    Code sample to register ClassDesc and define the all settings for the 3DS
    Max plugin:
    @code

// export plugin of the 3DS Max.
class nMaxExport2 : public SceneExport
{
public:
    ...
};

// This is all thing for your plugin. Doesn't it easy to use than appwizard huh? :)
static tRegisteredClassDesc<nMaxExport2> maxExportInterfaceClassDesc2(
    "nMaxExport2",         // class name
    N_MAX_EXPORT_CLASS_ID, // class ID
    SCENE_EXPORT_CLASS_ID, // super class ID
    1);//,                 // 1: public, 0: non-public
    //_T(""),              // category
    //"nebula2");          // scripter-visible name

    @endcode

    @note
    See 'nmax/inc/export2/nmaxexport2.h' file for an whole example.

    An instace of the nKernelServer will be created on DLL attach, and deleted 
    on DLL detach.
    Also a nMaxLoghandler is created and setted as the default loghandler for 
    the ks instance, but this LogHandler needes a pointer to the MaxInterface 
    as soon as possible.

    Another things you should know to use Nebula 3DS Max plugin toolkit is that
    you should define both of nMaxPluginInitialize() and nMaxPluginUninitialize()
    functions in your module.
    The functions provide a way for initialization and uninitialiazation of your
    plugin.
    If you do not define both of the functions, there will be the link error caused
    by the absent of that. 

    This file is licensed under the terms of the Nebula License. <br>
    (C) 2004 Johannes Kellner

    - 23-Jun-05 last updated by Kim, H.W.
*/

#include <Max.h>
#include <iparamb2.h>
#include <iparamm2.h>

// max5: use vc6, vc7 and vc7.1 (no need to use MAX_mem.h on)
// max6: use vc7 (default is ok) and vc7.1 (should download max_mem on discreet site)

// include max memory wrapper only on max6 (it will spit error out on max5)
// - 16-Jan-05 modified by Kim, H.W. no more need on 3dsmax7. 
//             (there will be Max_mem linkage error if we use 'Max_mem.h')
#if MAX_RELEASE >= 6000 && MAX_RELEASE < 6900 // 6900: 3dsmax7 alpha.
namespace MAXMem {
#include <MAX_mem.h> 
}
using namespace MAXMem;
#endif

//#include "util/narray.h"
//#include "base/nmaxloghandler.h"
//#include "kernel/nkernelserver.h"
#include <util/array.h>

class registeredClassDesc : public ClassDesc2
{
public:
    /// constructor
    registeredClassDesc(const char* theClassName,
                        Class_ID theClassID,
                        SClass_ID theSuperClassID,
                        int isPublic = 0,
                        const char* theCateory = "Nebula",
                        const char* internalName = "Nebula");
    /// destructor
    virtual ~registeredClassDesc() 
    { 
        /*must be empty, the array is deleted on dll unload*/ 
    };    
    
    /** from MAXSDK:
    This method returns a Boolean value. If the plug-in can be picked and 
    assigned by the user, as is usually the case, return TRUE. 
    Certain plug-ins may be used privately by other plug-ins implemented 
    in the same DLL and should not appear in lists for the user to choose from.
    These plug-ins would return FALSE.
    */
    virtual int	IsPublic() { return myIsPublic; }
	
    /** from MAXSDK:
    3ds max calls this method when it needs a pointer to a new instance of 
    the plug-in class.
    For example, if 3ds max is loading a file from disk containing a previously 
    used plug-in (procedural object, modifier, controller, etc.), it will call
    the plug-in's Create() method.
    The plug-in responds by allocating a new instance of its plug-in class. 
    The optional parameter 'bool loading = false' passed to Create() is a flag 
    indicating if the class being created is going to be loaded from a disk file.
    FIXME: this is not handled:
    If the flag is TRUE, the plug-in may not have to perform any initialization 
    of the object because the loading process will take care of it. 
    See in the MAXSDK the Advanced Topics section on Loading and Saving for 
    more information.
    */
    virtual void* Create(BOOL loading = FALSE) { return this->makeNewClass(); }

    /** from MAXSDK:
    This method returns the name of the class. This name appears in the button 
    for the plug-in in the 3ds max user interface.
    */
    virtual const TCHAR* ClassName() { return _T(myClassName); }

    /** from MAXSDK:
    This method returns a system-defined constant describing the class that this
    plug-in class was derived from.
    */
    virtual SClass_ID SuperClassID() { return mySuperClassID; }

    /** from MAXSDK:
    This method must return the unique ID for the plug-in object. 
    A program is provided with the SDK to generate these ClassIDs.
    It is VERY important you use this program to create the ClassIDs for your 
    plug-ins.
    If you use one of the source code examples to create your plug-in, you MUST
    change the existing Class_ID. If you don't, you'll get a conflict. 
    If two ClassIDs conflict, the system will only load the first one it finds 
    (and will post a message when it attempts to load the second one noting that 
    there is a Class_ID conflict).
    */
    virtual Class_ID ClassID() { return myClassID; }

    /** from MAXSDK:
    The category is selected in the bottom-most drop down list in the create 
    branch of the command panel.
    If this is set to be an existing category (i.e. "Standard Primitives", 
    "Particle Systems", etc.) then the plug-in will appear in that category.
    Developers should NOT add to the categories provided by 3ds max 
    (see the note below).
    If the category doesn't yet exist then it is created.
    If the plug-in does not need to appear in the list, it may simply return 
    a null string as in _T("").
    Category() is also used for modifiers to classify them in the button sets 
    dialog.
    Important Note: The 3ds max architecture has a limit of 12 plug-ins per 
    category in the Create branch.
    To prevent a problem with too many plug-ins per category, developers should
    ALWAYS create a new Category for their plug-ins, rather than using one of 
    the existing ones used by the standard 3ds max plug-ins.
    Note that versions of 3ds max prior to release 1.2 would crash if there were 
    more than 12 buttons per category.
    */
    virtual const TCHAR* Category() { return _T(myCateory); }

    virtual const TCHAR* InternalName() { return _T(myInternalName); }

    HINSTANCE		HInstance() { return hInstance; }

private:
    /**
    This method will be overloaded automatic by the template 
    tRegisteredClassDesc<class TYPE> and will return a new inctance of the TYPE.
    */
    virtual void* makeNewClass() = 0;
    
    /// internal storage of the className
    const char* myClassName;
    /// internal storage of the class id
    Class_ID myClassID;
    /// internal storage of the superclass id
    SClass_ID mySuperClassID;
    /// internal storage of the isPublic flag
    int myIsPublic;
    /// internal storage of the category name
    const char* myCateory;
    /// parsable name (scripter-visible name)
    const char* myInternalName;
  
public:
    /// the shared array for all instances of this class to register self 
    /// in the contructor
	static Util::Array<registeredClassDesc*> array;
};

//------------------------------------------------------------------------------
/**
    template tRegisteredClassDesc: 
    - inherite from registeredClassDesc for the class TYPE
    - create the required method makeNewClass()
    
    A static instance of this template class must be created.
    For Example:
    static tRegisteredClassDesc<nMaxExport> nMaxExportClassDescInstance("nMaxExport", N_MAX_EXPORT_CLASS_ID, SCENE_EXPORT_CLASS_ID);
*/
template<class TYPE> class tRegisteredClassDesc : public registeredClassDesc
{
public:
    tRegisteredClassDesc(const char* theClassName, 
                         Class_ID theClassID, 
                         SClass_ID theSuperClassID, 
                         int isPublic = 0, 
                         const char* theCateory = "Nebula",
                         const char* internalName = "Nebula") :
        registeredClassDesc (theClassName, 
                             theClassID, 
                             theSuperClassID, 
                             isPublic, 
                             theCateory, 
                             internalName)
    { 
        //empty
    };

    virtual ~tRegisteredClassDesc() { /*empty*/ };

private:
    void* makeNewClass()
    { 
        return static_cast<void*>(new TYPE);
    }
};

///the description of the pluging
#ifndef LIB_DESCRIPTION
#define LIB_DESCRIPTION "Nebula3 - unspecified plugin - build: " __DATE__
#endif

///for what max version this plugin is compiled for
#ifndef LIB_MAX_VERSION
#define LIB_MAX_VERSION VERSION_3DSMAX
#endif

///can be loaded on demand - see CanAutoDefer() for more
#ifndef LIB_CANAUTODEFER
#define LIB_CANAUTODEFER 0
#endif

/// the dll entry funtion, called from the automatic added static init function
BOOL WINAPI DllMain(HINSTANCE hinstDLL, ULONG fdwReason, LPVOID lpvReserved);
/// return the i'th class description of the published classes to max
__declspec( dllexport ) ClassDesc* LibClassDesc(int i);
/// return the number of classes in this plugin that are published to max
__declspec( dllexport ) int LibNumberClasses();
/// return the desciption of this plugin
__declspec( dllexport ) const TCHAR* LibDescription();
/// return the max version this plugin is compiled for
__declspec( dllexport ) ULONG LibVersion();
/// return if plugin can be loaded on demand
__declspec( dllexport ) ULONG CanAutoDefer();

//-----------------------------------------------------------------------------
/**
    Do initializations when plugin startup. 
    One should define one's own nMaxPluginInitialize() function. 
    Add any initializations code which needed at the plugin startup time.
    (usually when the 3DS Max starup)

    @note
        This is called when a dll is loaded

    - May-05    kims     Added
*/
extern bool nMaxPluginInitialize();

//-----------------------------------------------------------------------------
/**
    Do Uninitializations when 3ds max is closed.
    One should define one's own nMaxPlugUninInitialize() function.
    Add any uninitializations code which needed at the plugin exit time.
    (usually when the 3DS Max exit)

    @note
        This is called when a dll is unloaded.

    - May-05    kims     Added
*/
extern void nMaxPluginUninitialize();

#endif
