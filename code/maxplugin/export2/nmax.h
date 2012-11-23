//-----------------------------------------------------------------------------
//  nmax.h
//
//  (c)2004 Kim, Hyoun Woo
//-----------------------------------------------------------------------------
#ifndef N_MAX_H
#define N_MAX_H

//-----------------------------------------------------------------------------
// CAUTION : DO NOT INCLUDE ANY NEBULA SPECIFIC HEADERS IN THIS FILE.
//-----------------------------------------------------------------------------

#include <Max.h>
#include <simpobj.h>
#include <modstack.h>
#include <stdmat.h>
#include <shaders.h>
#include <decomp.h>
//#include <bmmlib.h>
//#include <guplib.h>

// now, Character Studio entirely included in 3dsmax7
// for other version of 3dsmax, specify include path of character studio
// in 'project > directory' page of 'tool > option' menu.
#if (MAX_RELEASE >= 6900)   // 3dsmax R7 or R7 alpha
    #include <cs/bipexp.h>
    #include <cs/phyexp.h>
#else
    #include <bipexp.h>
    #include <phyexp.h>    // for other version
#endif

// This should be included before 'iskin.h' to use it.
#include <iparamm2.h>      
#include <iskin.h>
#include <plugapi.h>
#include <ikctrl.h>
#include <iiksys.h>
#include <notetrck.h>

#if MAX_RELEASE >= 6000
    #include <IDXMaterial.h>
#endif

#if MAX_RELEASE >= 4000
#include <custattrib.h>
#include <icustattribcontainer.h>
#endif

// copyed from stdmat2.h
// IDs for all the ParamBlocks and their parameters.  One block UI per rollout.
enum { std2_shader, std2_extended, std2_sampling, std_maps, std2_dynamics, };  // pblock IDs
// std2_shader param IDs
enum 
{ 
    std2_shader_type, std2_wire, std2_two_sided, std2_face_map, std2_faceted,
    std2_shader_by_name,  // virtual param for accessing shader type by name
};
// std2_extended param IDs
enum 
{ 
    std2_opacity_type, std2_opacity, std2_filter_color, std2_ep_filter_map,
    std2_falloff_type, std2_falloff_amnt, 
    std2_ior,
    std2_wire_size, std2_wire_units,
    std2_apply_refl_dimming, std2_dim_lvl, std2_refl_lvl,
    std2_translucent_blur, std2_ep_translucent_blur_map,
    std2_translucent_color, std2_ep_translucent_color_map,
};

// std2_sampling param IDs
enum 
{ 
    std2_ssampler, std2_ssampler_qual, std2_ssampler_enable, 
    std2_ssampler_adapt_on, std2_ssampler_adapt_threshold, std2_ssampler_advanced,
    std2_ssampler_subsample_tex_on, std2_ssampler_by_name, 
    std2_ssampler_param0, std2_ssampler_param1, std2_ssampler_useglobal
};
// std_maps param IDs
enum 
{
    std2_map_enables, std2_maps, std2_map_amnts, std2_mp_ad_texlock, 
};
// std2_dynamics param IDs
enum 
{
    std2_bounce, std2_static_friction, std2_sliding_friction,
};


// paramblock2 block and parameter IDs for the standard shaders
// NB these are duplicated in shaders/stdShaders.cpp...
enum { shdr_params, };
// shdr_params param IDs
enum 
{ 
    shdr_ambient, shdr_diffuse, shdr_specular,
    shdr_ad_texlock, shdr_ad_lock, shdr_ds_lock, 
    shdr_use_self_illum_color, shdr_self_illum_amnt, shdr_self_illum_color, 
    shdr_spec_lvl, shdr_glossiness, shdr_soften,
};

#define SECONDSPERTICK (1.0f / 4800.0f)

//-----------------------------------------------------------------------------
// default value of assigns and paths
#define N_MAXEXPORT_INIFILE    "nmaxtoolbox.ini"

#define N_MAXEXPORT_BINARY_PATH    "home:bin/win32d/"

#define N_MAXEXPORT_ANIMS_ASSIGN    "home:export/anims/"
#define N_MAXEXPORT_GFXLIB_ASSIGN   "home:export/gfxlib/"
#define N_MAXEXPORT_GUI_ASSIGN      "home:export/"
#define N_MAXEXPORT_LIGHTS_ASSIGN   "home:export/"
#define N_MAXEXPORT_MESHES_ASSIGN   "home:export/meshes/"
#define N_MAXEXPORT_SHADERS_ASSIGN  "home:data/shaders/"
#define N_MAXEXPORT_TEXTURES_ASSIGN "home:export/textures/"

#define N_MAXEXPORT_ANIMS_PATH    "anims:"
#define N_MAXEXPORT_GFXLIB_PATH   "gfxlib:"
#define N_MAXEXPORT_GUI_PATH      "gui:"
#define N_MAXEXPORT_LIGHTS_PATH   "lights:"
#define N_MAXEXPORT_MESHES_PATH   "meshes:"
#define N_MAXEXPORT_SHADERS_PATH  "shaders:"
#define N_MAXEXPORT_TEXTURES_PATH "textures:"

//-----------------------------------------------------------------------------
// verbose level of log message
enum Verbose {
    Error           = 0,
    Warning         = 1,
    Low             = 2,
    Medium          = 3,
    High            = 4,
    NumVerboseLevel = 5
};

//-----------------------------------------------------------------------------
// function declaration for exposing it to max script.
int ExportScene(const TCHAR* name, Interface* inf, INode* inode, int previewMode);

// @name IPC Functions
// @{
bool nChangeShaderParameter(const char* toolkitShaderName, const char* shaderHandling, 
                            const char* paramID, const char* value);
bool nDoHotLoading(const char* objPath);
bool nConnectIpc(const char* host, const char* port);
void nDisConnectIpc();
bool nSendMsgIpc(const char* msg);
bool nIsConnectedIpc();
// @}

#endif
