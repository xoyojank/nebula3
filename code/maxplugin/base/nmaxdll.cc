//------------------------------------------------------------------------------
//  nmaxdll.cc
//
//  (C)2004 Johannes Kellner
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "base/nmaxdll.h"
//#include "kernel/nwin32loghandler.h"
#include "core/coreserver.h"
#include "io/ioserver.h"
#include "io/logfileconsolehandler.h"

///the dllHandle
HINSTANCE hInstance;

///have we already initiated the custom controlls?
int controlsInit = FALSE;

/// Kernel server instance
//nKernelServer* kernelServer = 0;
Ptr<Core::CoreServer> coreServer;
Ptr<IO::IoServer> ioServer;

using namespace Util;
using namespace Core;
using namespace IO;
///the array where a 'unique' instances of the classDesc are registered
Util::Array<registeredClassDesc*> registeredClassDesc::array(1,1);

//------------------------------------------------------------------------------
/**
    To make plugin Classes like a ExporterClass to Max it's required:
    - to create a class description by inherit from ClassDesc
    - you must count the number of ClassDesc you want to publish to Max, returned in LibNumberClasses()
    - you must return a pointer to an instance of the right inherited ClassDesc in LibClassDesc(int i).

    The common solution for this task is:
    - create a inherited ClassDesc's for each Class and a static instance of this
    - count the number of classes you will to pulish to in the plugin and update
    the return value for LibNumberClasses()
    - update LibClassDesc(int i) to return the right instances adress

    Bads:
    - done as parts of the code, so all is static
    - request a special maxdll-header for every plugin you create
     with adapted functions (LibNumberClasses, LibClassDesc)
    - requires to change 3-4 parts in you code if you add or remove a class for the export
    to max
    - can not adjusted dynamic

    A 'better' solution:
    - the inherited ClassDecs's is created with the template
    tRegisteredClassDesc<class TYPE> and must only a static instance must be created.
    So you have only two lines code that defines all you need to know about this tasks
    - the linkedClassDesc is parrent for all the individual ClassDesc's created in step 1.
    All instances of linkedClassDesc share a nArray and in the Consturctoe they regiester
    themself in this nArray (Because all instances of them are defined static, the call to
    the contructor is done automatic while loading the dll, so all required information
    is pressent before the _any_ method can be called on this dll)
    The number of ClassDesc in this plugin so is automatic evaluated during the init of the static
    members and is equal to size of the nArray.
    - Because every linkedClassDesc push ther 'this'-pointer to the array it is easy to get the right
    adresses of the instance.

    Goods:
    - count and get the right instance adresses is done automatic
    - easy add or remove of classes you like to export to max
    - only one generic maxplugin dllheader for all plugins you like to create

    Bads:
    - in the moment there is a bug (only worked around, not fixed)
    all static variables are initiated two times,
    at differnt adresses. It's not clear if this is a bug of my code,
    or a bug in the linker or something else. If you have a idea about this please
    notify me, to inspect the bug see part2 of the constructor below.
    A workaround of this bug is that I compare the Class_ID of all instances and only register
    uniqes.
*/
registeredClassDesc::registeredClassDesc(
    const char* theClassName,
    Class_ID theClassID,
    SClass_ID theSuperClassID,
    int isPublic,
    const char* theCateory,
    const char* internalName)
: myClassName(theClassName),
  myClassID(theClassID),
  mySuperClassID(theSuperClassID),
  myIsPublic(isPublic),
  myCateory(theCateory),
  myInternalName (internalName)
{
    bool unique = true;

    //if (!this->array)
    //{
    //    /*a automatic init with the static keyword dons't work for this,
    //    because the order of the initialitze the static vars is not defined.
    //    So we can run in the case where the static implentation of the tRegisteredClassDesc
    //    calls this constructor but the static nArray isn't initiated.
    //    */
    //    this->array = n_new(nArray<registeredClassDesc*>(1,1));
    //}
    //else
    //{
    //    /*the next thing is that is seems that the init of the static var
    //    is done two times, don't ask me why...
    //    So we get 2 objects and adresses for the same implementation of the tRegisteredClassDesc.
    //    please send me a mail if you have a idea why this can happen
    //    */
    //    int i = 0;
    //    while (i < this->array->Size() && unique)
    //    {
    //        /*FIXME: this is just a WORKKAROUND/HACK,
    //        but I have no idea why the static vars are initated two times. Maybe this is cause by seperate thread attaches of this DLL.
    //        FIXME:
    //        The Class_ID is used for the compare to make shure that there
    //        are only unique linkedClassDescs, but I guess that the people tend to
    //        use the same Class_ID more than once... may we should add a warnig here.
    //        */
    //        registeredClassDesc* temp = this->array->At(i++);
    //        if (temp->ClassID() == this->ClassID())
    //            unique = false;
    //    }
    //}
	for (IndexT index = 0; index < array.Size();index++)
	{
		registeredClassDesc* temp = array[index];
		if (temp->ClassID() == this->ClassID())
		{
			unique = false;
			break;
		}
	}
    if (unique)
    {
        //add the elemnt because we don't have it in the list
        //this->array->PushBack(this); //add self to the linked list
		array.Append(this);
    }
}

//------------------------------------------------------------------------------
/**
    the dll entry funtion, this is called after init of the static vars
*/
BOOL WINAPI DllMain(HINSTANCE hinstDLL, ULONG fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH: //DLL - load
        {
            // Hang on to this DLL's instance handle.
            hInstance = hinstDLL;

            // Initialize the custom controls. This should be done only once.
            if (!controlsInit)
            {
                controlsInit = true;
                // Registering window classes for custom controls.
                // Previously every DLL had to initialize the window classes through a call to InitCustomControls()
                // in DllMain. This caused a substantial hit on the available system resources in Windows 9x
                // for every plug-in that was loaded. The classes are now globally registered for the whole process.
                // Plug-ins no longer have to call InitCustomControls() at startup - but it doesn't hurt since once
                // the classes has been registered they will not be registered again.
                InitCustomControls(hInstance);

                //InitCommonControls(); //win95... don't used
            }

            //create a new kernelserver if needed
            //if (kernelServer == 0)
            //{
            //    kernelServer = new nKernelServer();

            //    const char* plugDir = GetCOREInterface()->GetDir(APP_PLUGCFG_DIR);
            //    nLogHandler* logHandler = new nWin32LogHandler("nmaxtoolbox", plugDir);
            //    //logHandler->Open();

            //    //setup the new log handler
            //    kernelServer->SetLogHandler(logHandler);

            //    if (!nMaxPluginInitialize())
            //        return FALSE;
            //}

			// setup the core server
			coreServer = CoreServer::Create();
			coreServer->SetCompanyName(StringAtom("Dengwenyi@gmail.com"));
			coreServer->SetAppName(StringAtom("N3 Max Plugin"));
			coreServer->Open();

			// initialize IO subsystem
			ioServer = IoServer::Create();

			// setup a logfile console handler
			Ptr<LogFileConsoleHandler> logFileHandler = LogFileConsoleHandler::Create();
			Console::Instance()->AttachHandler(logFileHandler.upcast<ConsoleHandler>());

			if (!nMaxPluginInitialize())
				return FALSE;
        }
        break;
        case DLL_PROCESS_DETACH: //DLL - unload
        {
            nMaxPluginUninitialize();

			ioServer = 0;
			coreServer = 0;

            //cleanup the the list of classes here
            //if (registeredClassDesc::array != NULL)
            //{
            //    n_delete(registeredClassDesc::array);
            //}

            //if (kernelServer != 0)
            //{
            //    nWin32LogHandler* logHandler = static_cast<nWin32LogHandler*>(kernelServer->GetLogHandler());
            //    //logHandler->Close();
            //    delete logHandler;
            //    delete kernelServer;
            //}
        }
        break;
    }
    return (TRUE);
}

//------------------------------------------------------------------------------
/**
    give max the i'th class decription
*/
__declspec( dllexport ) ClassDesc*
LibClassDesc(int i)
{
	if( i < registeredClassDesc::array.Size())
		return registeredClassDesc::array[i];
    //if (registeredClassDesc::array)
    //    return registeredClassDesc::array->At(i);

    return 0;
}

//------------------------------------------------------------------------------
/**
    tell max the number classes, public to max in this dll
*/
__declspec( dllexport ) int
LibNumberClasses()
{
	return registeredClassDesc::array.Size();
    //if (registeredClassDesc::array)
    //    return registeredClassDesc::array->Size();

    //return 0;
}

//------------------------------------------------------------------------------
/**
    return desription of this dll
*/
__declspec( dllexport ) const TCHAR*
LibDescription()
{
    return LIB_DESCRIPTION;
}

//------------------------------------------------------------------------------
/**
    return version of max that plugin is
    compiled for
*/
__declspec( dllexport ) ULONG
LibVersion()
{
    return LIB_MAX_VERSION;
}

//------------------------------------------------------------------------------
/**
   from MAXSDK:
   It is possible to indicate to 3ds max that certain plug-ins should not be loaded at startup.
   Each deferred plug-in will be loaded when and if needed, which will happen when one or
   more of the 3ds max classes implemented by the plug-in is actually created.
   It might also happen when detailed information about the plug-in is requested.
   The principal benefit of deferral is that the Windows resources used by the plug-in
   will not be consumed unless there is some specific use or interrogation of the plug-in.
   Time taken at startup to load the plug-in, assign its virtual memory, etc. is also saved,
   or at least deferred.
   1 - can
   0 - can not
*/
__declspec( dllexport ) ULONG
CanAutoDefer()
{
    return LIB_CANAUTODEFER;
}
