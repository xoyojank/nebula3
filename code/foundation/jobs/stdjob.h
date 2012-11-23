#pragma once
//------------------------------------------------------------------------------
/**
    @file jobs/stdjob.h
    
    Standard header for job functions.
    
    (C) 2009 Radon Labs GmbH
*/
#define __NEBULA3_JOB__ (1)
#include "core/config.h"
#if __WIN32__
#include "core/win32/precompiled.h"
#include "jobs/jobfunccontext.h"
#elif __XBOX360__
#include "core/xbox360/precompiled.h"
#include "jobs/jobfunccontext.h"
#elif __WII__
#include "core/wii/precompiled.h"
#include "jobs/jobfunccontext.h"
#elif __PS3__
#include <cell/spurs/job_context.h>
#include <spu_intrinsics.h>
#include <spu_mfcio.h>
#include <spu_printf.h>
#include <cell/spurs/job_queue.h>
#include "jobs/jobfunccontext.h"
#include "jobs/ps3/ps3jobfuncwrapper.h"
#include "jobs/ps3/ps3spuconfig.h"
#else
#error "Job functions not supported on this platform!"
#endif    

#if !__PS3__
#define __ImplementSpursJob(func)
#endif


