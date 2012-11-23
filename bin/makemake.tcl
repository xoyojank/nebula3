#--------------------------------------------------------------------
#   makemake.tcl
#   ============
#   Tcl procs to convert Nebula package definitions into Makefiles
#   and Visual Studio workspace files.
#
#   Nebula package files define 'modules' and 'targets'. A module
#   is a collection of source files that belong together.
#   A target is a collection of modules, and specifies how the
#   modules should be compiled into a binary. This can be a
#   generic dll, a class package dll, or an executable.
#
#   (C) 2000 A.Weissflog
#
#   17-May-00   floh    added support for header files
#   07-Nov-00   floh    new target type 'mll' for maya plugins
#   01-Feb-01   floh    support for setdoxycfg keyword
#   24-Jul-02   floh    added Xbox support
#   31-Jul-03   floh    removed N_EXPORT macros from generated package files
#--------------------------------------------------------------------

global includePath
set headerPrefix "../inc/"

#--------------------------------------------------------------------
#   structs
#   =======
#   mod(i,name)         - name of module
#   mod(i,dir)          - directory where source files reside
#   mod(i,files)        - list of source files for module 'name'
#   mod(i,headers)      - list of header files for module 'name'
#   mod(i,objs)         - object file names for win32 (generated)
#   mod(i,srcs)         - source file names for win32 (generated)
#   mod(i,hdrs)         - header file names
#   mod(i,nidls)        - interface definition file
#   mod(i,deps,src)     - dependencies for 'src' (generated)
#   mod(i,type)         - "cpplib", "clib" -> these sources are just c 
#                         file collections
#
#   tar(i,name)                 - name of target
#   tar(i,basedir)              - optional base directory
#   tar(i,precompiledheader)    - optional name of precompiled header
#   tar(i,platform)             - one of 'all|win32|unix|xbox'
#   tar(i,type)                 - one of 'dll|package|exe|mll|lib'
#   tar(i,mods)                 - module list for target 'name'
#   tar(i,depends)              - list of other targets, which this target depends on
#   tar(i,defines)              - list of extra #defines
#   tar(i,additionaloptions)    - list of additional options
#   tar(i,libs_win32)           - general win32 specific link libs
#   tar(i,libs_unix)            - unix specific link libs
#   tar(i,libs_osx)             - OSX specific link libs
#   tar(i,libs_xbox360)         - xbox specific link libs
#   tar(i,libs_ps3)             - ps3 specific link libs
#   tar(i,libs_win32_debug)     - debug/win32 specific link libs 
#   tar(i,libs_win32_release)   - release/win32 specific link libs
#   tar(i,libs_xbox360_debug)      - debug/xbox specific link libs
#   tar(i,libs_xbox360_release)    - release/xbox specific link libs
#   tar(i,libs_ps3_debug)          - debug/ps3 specific link libs
#   tar(i,libs_ps3_release)        - release/ps3 specific link libs
#   tar(i,libs_wii_debug)       - debug/wii specific link libs
#   tar(i,libs_wii_release)     - release/wii specific link libs
#   tar(i,rsrc_win32)           - win32 specific resource files
#   tar(i,frameworks_osx)       - OSX specific frameworks
#   tar(i,doxycfg)              - location of Doxygen config file
#   tar(i,incpath_win32)            - additional include paths for Win32
#   tar(i,libpath_win32)            - additional lib paths for Win32
#   tar(i,preBuildStepCommandLine)  - optional pre build step commandline
#   tar(i,preBuildStepDescription)  - optional pre build step description
#
#   sln(i,name)     - name of solution
#   sln(i,targets)  - list of targets in the solution
#--------------------------------------------------------------------

global mod
global tar
global sln
global num_mods
global num_tars
global num_slns

global GroupFilter
set GroupFilter 0

set num_mods 0
set num_tars 0
set num_slns 0

#--------------------------------------------------------------------
#   procs
#   =====
#   beginmodule $mod_name
#       setdir $subdir
#       setfiles   $file $file $file...
#       setheaders $file $file $file...
#       setver $file
#   endmodule
#   begintarget $name
#       setmods $module $module $module...
#       setdepend $target $target...
#       setlibs_win32 $lib $lib...
#       setlibs_unix  $lib $lib...
#       setlibs_osx   $lib $lib...
#       setframeworks_osx $lib $lib...
#   endtarget
#--------------------------------------------------------------------

#====================================================================
#   UTILITY PROCEDURES
#====================================================================

#--------------------------------------------------------------------
#   addtolist $var $list
#   04-Mar-00   floh    created
#--------------------------------------------------------------------
proc addtolist {var list} {
    upvar $var v
    for {set i 0} {$i < [llength $list]} {incr i} {
        append v [lindex $list $i] " "
    }
}

#--------------------------------------------------------------------
#   findmodbyname $name
#   Find module index by name.
#   04-Mar-00   floh    created
#--------------------------------------------------------------------
proc findmodbyname {name} {
    global mod
    global num_mods
    for {set i 0} {$i < $num_mods} {incr i} {
        if {$name == $mod($i,name)} {
            return $i
        }
    }
    puts "ERROR: module '$name' not defined!"
    exit
}

#--------------------------------------------------------------------
#   findtargetbyname $name
#   Find target index by name.
#   04-Mar-00   floh    created
#--------------------------------------------------------------------
proc findtargetbyname {name} {
    global tar
    global num_tars
    for {set i 0} {$i < $num_tars} {incr i} {
        if {$name == $tar($i,name)} {
            return $i
        }
    }
    puts "ERROR: target '$name' not defined!"
    exit
}

#--------------------------------------------------------------------
#   findsolutionbyname $name
#   Find solution index by name.
#   04-Mar-00   floh    created
#--------------------------------------------------------------------
proc findsolutionbyname {name} {
    global sln
    global num_slns
    for {set i 0} {$i < $num_slns} {incr i} {
        if {$name == $sln($i,name)} {
            return $i
        }
    }
    puts "ERROR: solution '$name' not defined!"
    exit
}

#--------------------------------------------------------------------
#   gen_filelists $module
#   Generate source and object file name lists for a given module.
#   04-Mar-00   floh    created
#   11-Mar-00   floh    + generate dependencies for files
#   20-Mar-00   floh    + global variable 'global_gendeps' used to
#                         turn dependency-generation on/off
#   17-May-00   floh    + support for header files
#--------------------------------------------------------------------
proc gen_filelists {module} {
    global mod
    global num_mods
    global headerPrefix

    set i [findmodbyname $module]

    set num_files [llength $mod($i,files)]

    # generate object file lists
    set mod($i,objs) ""
    for {set j 0} {$j < $num_files} {incr j} {
        lappend mod($i,objs) "$mod($i,dir)/[lindex $mod($i,files) $j]\$(OBJ)"
    }

    # generate source file lists
    set mod($i,srcs) ""   
    for {set j 0} {$j < $num_files} {incr j} {
        set cur_src "$mod($i,dir)/[lindex $mod($i,files) $j].cc"
        lappend mod($i,srcs) $cur_src
    }

    # generate source file lists
    set mod($i,srcs) ""   
    for {set j 0} {$j < $num_files} {incr j} {
        set cur_src "$mod($i,dir)/[lindex $mod($i,files) $j].cpp"
        lappend mod($i,srcs) $cur_src
    }

    # append header files to 'hdrs'
    set mod($i,hdrs) ""
    set num_headers [llength $mod($i,headers)]
    for {set j 0} {$j < $num_headers} {incr j} {
        set cur_hdr "$mod($i,dir)/[lindex $mod($i,headers) $j].h"
        lappend mod($i,hdrs) $cur_hdr
    }
}

#====================================================================
#   .PAK FILE PARSING PROCEDURES
#====================================================================

#--------------------------------------------------------------------
#   import_target "path" "name" "depends"
#   Define an external project.
#--------------------------------------------------------------------
proc import_target {path solution name depends} {
    global num_tars
    global tar
        
    set tar($num_tars,name) $name
    set tar($num_tars,solution) $solution
    set tar($num_tars,type) "import"
    set tar($num_tars,import_path) $path
    set tar($num_tars,depends) $depends
    
    incr num_tars
}

#--------------------------------------------------------------------
#   beginmodule $name
#   Start definition of a module.
#   04-Mar-00   floh    created
#--------------------------------------------------------------------
proc beginmodule {name} {
    global num_mods
    global mod
    set mod($num_mods,name)         $name
    set mod($num_mods,dir)          ""
    set mod($num_mods,files)        ""
    set mod($num_mods,headers)      ""
    set mod($num_mods,nidls)        ""
    set mod($num_mods,type)         ""
}

#--------------------------------------------------------------------
#   setdir $subdir
#   Specify path where module files live.
#   04-Mar-00   floh    created
#--------------------------------------------------------------------
proc setdir {subdir} {
    global mod
    global num_mods
    set mod($num_mods,dir) $subdir
}

#--------------------------------------------------------------------
#   setfiles $files
#   04-Mar-00   floh    created
#--------------------------------------------------------------------
proc setfiles {file_list} {
    global mod
    global num_mods
    addtolist mod($num_mods,files) $file_list
}

#--------------------------------------------------------------------
#   setmodtype type
#   24-Jul-02   floh    created
#--------------------------------------------------------------------
proc setmodtype {type} {
    global mod
    global num_mods
    set mod($num_mods,type) $type
}    

#--------------------------------------------------------------------
#   setheaders $files
#   17-May-00   floh    created
#--------------------------------------------------------------------
proc setheaders {file_list} {
    global mod
    global num_mods
    addtolist mod($num_mods,headers) $file_list
}

#--------------------------------------------------------------------
#   setnidls $files
#   01-Oct-06   floh    created
#--------------------------------------------------------------------
proc setnidls {file_list} {
    global mod
    global num_mods
    addtolist mod($num_mods,nidls) $file_list
}

#--------------------------------------------------------------------
#   endmodule
#   04-Mar-00   floh    created
#--------------------------------------------------------------------
proc endmodule { } {
    global mod
    global num_mods

    # generate file lists for module
    set name $mod($num_mods,name)
    incr num_mods
    gen_filelists $name
}

#--------------------------------------------------------------------
#   dumpmodules
#   Print data of all defined modules.
#   04-Mar-00   floh    created
#--------------------------------------------------------------------
proc dumpmodules { } {
    global mod
    global num_mods
    puts "\n\n======================================================="
    puts " MODULES"
    puts " "
    for {set i 0} {$i < $num_mods} {incr i} {
        puts "-------------------------------------------------------"
        puts " module:      $mod($i,name)"
        puts " dir:         $mod($i,dir)"
        puts " files:       $mod($i,files)"
        puts " headers:     $mod($i,headers)"
    }
}

#--------------------------------------------------------------------
#   begintarget $name
#   04-Mar-00   floh    created
#--------------------------------------------------------------------
proc begintarget {name} {
    global num_tars
    global tar
    set tar($num_tars,name)                 $name
    set tar($num_tars,basedir)              ""
    set tar($num_tars,precompiledheader)    ""
    set tar($num_tars,platform)             "all"
    set tar($num_tars,type)                 ""
    set tar($num_tars,mods)                 ""
    set tar($num_tars,depends)              ""
    
    set tar($num_tars,libs_win32)           ""
    set tar($num_tars,libs_win32_debug)     ""
    set tar($num_tars,libs_win32_release)   ""
    set tar($num_tars,libs_win32_programming) ""
    set tar($num_tars,libs_win32_public_build) ""
    set tar($num_tars,libs_win32_securom)   ""
    set tar($num_tars,libs_win32_maya_debug)   ""
    set tar($num_tars,libs_win32_maya_release)   ""
    
    set tar($num_tars,libs_xbox360_debug)   ""
    set tar($num_tars,libs_xbox360_release) ""
    set tar($num_tars,libs_xbox360_programming) ""
    set tar($num_tars,libs_xbox360_public_build) ""

    set tar($num_tars,libs_ps3_debug)   ""
    set tar($num_tars,libs_ps3_release) ""
    set tar($num_tars,libs_ps3_programming) ""
    set tar($num_tars,libs_ps3_public_build) ""
    set tar($num_tars,libs_ps3_distribute_assert) ""
    
    set tar($num_tars,libs_wii_debug)       ""
    set tar($num_tars,libs_wii_release)     ""
    set tar($num_tars,libs_wii_programming) ""
    
    set tar($num_tars,libs_unix)            ""
    set tar($num_tars,libs_osx)             ""
    set tar($num_tars,libs_xbox360)         ""
    set tar($num_tars,libs_ps3)             ""
    set tar($num_tars,rsrc_win32)           ""
    set tar($num_tars,frameworks_osx)       ""
    set tar($num_tars,doxycfg)              ""
    set tar($num_tars,doxytitle)            ""
    set tar($num_tars,incpath_win32)        ""
    set tar($num_tars,incpath_osx)          ""
    set tar($num_tars,libpath_win32)        ""
    set tar($num_tars,defines)              ""
    set tar($num_tars,additionaloptions)             ""
    set tar($num_tars,preBuildStepCommandLine) ""
    set tar($num_tars,preBuildStepDescription) ""
    set tar($num_tars,import_path) "."
}

#--------------------------------------------------------------------
#   setplatform $platform
#   09-Mar-00   floh    created
#--------------------------------------------------------------------
proc setplatform {platform} {
    global num_tars
    global tar
    set tar($num_tars,platform) $platform
}

#--------------------------------------------------------------------
#   settype $type
#   04-Mar-00   floh    created
#--------------------------------------------------------------------
proc settype {type} {
    global num_tars
    global tar
    set tar($num_tars,type) $type
}

#--------------------------------------------------------------------
#   setbasedir $basedir
#   26-Jan-07   floh    created
#--------------------------------------------------------------------
proc setbasedir {basedir} {
    global num_tars
    global tar
    set tar($num_tars,basedir) $basedir
}

#--------------------------------------------------------------------
#   setprecompiledheader $header
#--------------------------------------------------------------------
proc setprecompiledheader {header} {
    global num_tars
    global tar
    set tar($num_tars,precompiledheader) $header
}

#--------------------------------------------------------------------
#   setmods $modules
#   04-Mar-00   floh    created
#--------------------------------------------------------------------
proc setmods {mod_list} {
    global num_tars
    global tar
    set tar($num_tars,mods) ""
    for {set i 0} {$i < [llength $mod_list]} {incr i} {
        lappend tar($num_tars,mods) [lindex $mod_list $i] 
    }
}

#--------------------------------------------------------------------
#   setclusters $clusters
#--------------------------------------------------------------------
proc setclusters {cluster_list} {
    global home
    global num_tars
    global tar
        
    for {set i 0} {$i < [llength $cluster_list]} {incr i} {
        set cluster [lindex $cluster_list $i]
        set headers ""
        set sources ""
                
        # get list of header files
        set full_headers [glob -nocomplain -directory ./$tar($num_tars,basedir)/$cluster *.h]
        for {set j 0} {$j < [llength $full_headers]} {incr j} {
            lappend headers [file tail [lindex $full_headers $j]]
        }

        # get list of source files
        set full_sources [glob -nocomplain -directory ./$tar($num_tars,basedir)/$cluster *.cpp]
        for {set j 0} {$j < [llength $full_sources]} {incr j} {
            lappend sources [file tail [lindex $full_sources $j]]
        }
        # get list of source files
        set full_sources [glob -nocomplain -directory ./$tar($num_tars,basedir)/$cluster *.cc]
        for {set j 0} {$j < [llength $full_sources]} {incr j} {
            lappend sources [file tail [lindex $full_sources $j]]
        }
        set full_sources [glob -nocomplain -directory ./$tar($num_tars,basedir)/$cluster *.c]
        for {set j 0} {$j < [llength $full_sources]} {incr j} {
            lappend sources [file tail [lindex $full_sources $j]]
        }
        set full_sources [glob -nocomplain -directory ./$tar($num_tars,basedir)/$cluster *.m]
        for {set j 0} {$j < [llength $full_sources]} {incr j} {
            lappend sources [file tail [lindex $full_sources $j]]
        }
        set full_sources [glob -nocomplain -directory ./$tar($num_tars,basedir)/$cluster *.mm]
        for {set j 0} {$j < [llength $full_sources]} {incr j} {
            lappend sources [file tail [lindex $full_sources $j]]
        }

        # Make module from cluster
        beginmodule $cluster
            setmodtype cpplib
            setdir $cluster
            setfiles $sources
            setheaders $headers
        endmodule
    }
    
    # Set modules
    setmods $cluster_list
}

#--------------------------------------------------------------------
#   setclibclusters $clusters
#--------------------------------------------------------------------
proc setclibclusters {cluster_list} {
    global home
    global num_tars
    global tar
    
    for {set i 0} {$i < [llength $cluster_list]} {incr i} {
        set cluster [lindex $cluster_list $i]
        set headers ""
        set sources ""
                
        # get list of header files
        set full_headers [glob -nocomplain -directory ./$tar($num_tars,basedir)/$cluster *.h]
        for {set j 0} {$j < [llength $full_headers]} {incr j} {
            lappend headers [file tail [lindex $full_headers $j]]
        }

        # get list of source files
        set full_sources [glob -nocomplain -directory ./$tar($num_tars,basedir)/$cluster *.c]
        for {set j 0} {$j < [llength $full_sources]} {incr j} {
            lappend sources [file tail [lindex $full_sources $j]]
        }
        set full_sources [glob -nocomplain -directory ./$tar($num_tars,basedir)/$cluster *.cpp]
        for {set j 0} {$j < [llength $full_sources]} {incr j} {
            lappend sources [file tail [lindex $full_sources $j]]
        }
        set full_sources [glob -nocomplain -directory ./$tar($num_tars,basedir)/$cluster *.cc]
        for {set j 0} {$j < [llength $full_sources]} {incr j} {
            lappend sources [file tail [lindex $full_sources $j]]
        }
        set full_sources [glob -nocomplain -directory ./$tar($num_tars,basedir)/$cluster *.m]
        for {set j 0} {$j < [llength $full_sources]} {incr j} {
            lappend sources [file tail [lindex $full_sources $j]]
        }
        set full_sources [glob -nocomplain -directory ./$tar($num_tars,basedir)/$cluster *.mm]
        for {set j 0} {$j < [llength $full_sources]} {incr j} {
            lappend sources [file tail [lindex $full_sources $j]]
        }

        # get list of nidl files and add their target files to sources and headers
        set full_nidls [glob -nocomplain -directory ./$tar($num_tars,basedir)/$cluster *.nidl]
        for {set j 0} {$j < [llength $full_nidls]} {incr j} {
            set file [file tail [lindex $full_nidls $j]]
            if {-1 == [lsearch $headers $file]} {
                lappend headers $file
            }
            if {-1 == [lsearch $sources $file]} {
                lappend sources $file
            }
        }
                
        # Make module from cluster
        beginmodule $cluster
            setmodtype clib
            setdir $cluster
            setfiles $sources
            setheaders $headers
        endmodule
    }
    
    # Set modules
    setmods $cluster_list
}

#--------------------------------------------------------------------
#   setnidlclusters $clusters
#--------------------------------------------------------------------
proc setnidlclusters {cluster_list} {
    global home
    global num_tars
    global tar
    
    set nidl_clusters {}
    for {set i 0} {$i < [llength $cluster_list]} {incr i} {
        set cluster [lindex $cluster_list $i]
        set nidls ""
                
        # get list of nidl files
        set full_nidls [glob -nocomplain -directory ./$tar($num_tars,basedir)/$cluster *.nidl]
        for {set j 0} {$j < [llength $full_nidls]} {incr j} {
            set file [file tail [lindex $full_nidls $j]]
            lappend nidls $file
        }
                
        # make module from cluster
        beginmodule nidl_$cluster
            setmodtype cpplib
            setdir $cluster
            setnidls $nidls
        endmodule
        lappend nidl_clusters nidl_$cluster
    }
    
    # Set modules
    setmods $nidl_clusters
}

#--------------------------------------------------------------------
#   setjobfiles $clusters
#--------------------------------------------------------------------
proc setjobfiles { dir file_list } {
    global home
    global num_tars
    global tar
    
    set srcFiles ""
    set hdrFiles ""
    for {set i 0} {$i < [llength $file_list]} {incr i} {
        set curFile [lindex $file_list $i]
        
        # extract source files from file list
        if {[file extension $curFile] == ".cpp"} {
            lappend srcFiles $curFile
        }
        if {[file extension $curFile] == ".cc"} {
            lappend srcFiles $curFile
        }
        if {[file extension $curFile] == ".h"} {
            lappend hdrFiles $curFile
        }      
    }

    # make module from cluster
    beginmodule job_$tar($num_tars,name)
        setmodtype cpplib
        setdir $dir
        setfiles $srcFiles
        setheaders $hdrFiles
    endmodule
    
    # Set modules
    setmods job_$tar($num_tars,name)
}

#--------------------------------------------------------------------
#   setdepends $depends
#   04-Mar-00   floh    created
#--------------------------------------------------------------------
proc setdepends {depends} {
    global num_tars
    global tar
    set tar($num_tars,depends) $depends
}

#--------------------------------------------------------------------
#   setdoxycfg $filename
#   01-Feb-01   floh    created
#--------------------------------------------------------------------
proc setdoxycfg {filename} {
    global num_tars
    global tar
    set tar($num_tars,doxycfg)   $filename
}

#--------------------------------------------------------------------
#   setprebuildstep $commandline $description
#--------------------------------------------------------------------
proc setprebuildstep {commandline description} {
    global num_tars
    global tar
    set tar($num_tars,preBuildStepCommandLine) $commandline
    set tar($num_tars,preBuildStepDescription) $description
}

#--------------------------------------------------------------------
#   setincpath_win32 $paths
#   01-Jul-04   floh    created
#--------------------------------------------------------------------
proc setincpath_win32 {paths} {
    global num_tars
    global tar
    set tar($num_tars,incpath_win32) $paths
}

#--------------------------------------------------------------------
#   setincpath_osx $paths
#   OSX inc paths are delimited by spaces!
#   20-Feb-10   floh    created
#--------------------------------------------------------------------
proc setincpath_osx {paths} {
    global num_tars
    global tar
    set tar($num_tars,incpath_osx) $paths
}

#--------------------------------------------------------------------
#   setlibpath_win32 $paths
#   01-Jul-04   floh    created
#--------------------------------------------------------------------
proc setlibpath_win32 {paths} {
    global num_tars
    global tar
    set tar($num_tars,libpath_win32) $paths
}

#--------------------------------------------------------------------
#   setdefines $libs
#   04-Mar-00   floh    created
#--------------------------------------------------------------------
proc setdefines {defs} {
    global num_tars
    global tar
    set tar($num_tars,defines) $defs
}

#--------------------------------------------------------------------
#   setadditionaloptions $additionaloptions
#   07-Oct-17   johannes    created
#--------------------------------------------------------------------
proc setadditionaloptions {addopt} {
    global num_tars
    global tar
    set tar($num_tars,additionaloptions) $addopt
}

#--------------------------------------------------------------------
#   setlibs_win32 $libs
#   04-Mar-00   floh    created
#--------------------------------------------------------------------
proc setlibs_win32 {libs} {
    global num_tars
    global tar
    addtolist tar($num_tars,libs_win32) $libs
}

#--------------------------------------------------------------------
#   setlibs_win32_debug $libs
#   31-Jan-03   floh    created
#--------------------------------------------------------------------
proc setlibs_win32_debug {libs} {
    global num_tars
    global tar
    addtolist tar($num_tars,libs_win32_debug) $libs
}

#--------------------------------------------------------------------
#   setlibs_win32_release $libs
#   31-Jan-03   floh    created
#--------------------------------------------------------------------
proc setlibs_win32_release {libs} {
    global num_tars
    global tar
    addtolist tar($num_tars,libs_win32_release) $libs
}

#--------------------------------------------------------------------
#   setlibs_win32_programming $libs
#   31-Jan-03   floh    created
#--------------------------------------------------------------------
proc setlibs_win32_programming {libs} {
    global num_tars
    global tar
    addtolist tar($num_tars,libs_win32_programming) $libs
}

#--------------------------------------------------------------------
#   setlibs_win32_public_build $libs
#   31-Jan-03   floh    created
#--------------------------------------------------------------------
proc setlibs_win32_public_build {libs} {
    global num_tars
    global tar
    addtolist tar($num_tars,libs_win32_public_build) $libs
}

#--------------------------------------------------------------------
#   setlibs_win32_securom $libs
#   31-Jan-03   floh    created
#--------------------------------------------------------------------
proc setlibs_win32_securom {libs} {
    global num_tars
    global tar
    addtolist tar($num_tars,libs_win32_securom) $libs
}

#--------------------------------------------------------------------
#   setlibs_win32_maya_release $libs
#   31-Jan-03   floh    created
#--------------------------------------------------------------------
proc setlibs_win32_maya_release {libs} {
    global num_tars
    global tar
    addtolist tar($num_tars,libs_win32_maya_release) $libs
}

#--------------------------------------------------------------------
#   setlibs_win32_maya_release $libs
#   31-Jan-03   floh    created
#--------------------------------------------------------------------
proc setlibs_win32_maya_debug {libs} {
    global num_tars
    global tar
    addtolist tar($num_tars,libs_win32_maya_debug) $libs
}

#--------------------------------------------------------------------
#   setlibs_unix $libs
#   04-Mar-00   floh    created
#--------------------------------------------------------------------
proc setlibs_unix {libs} {
    global num_tars
    global tar
    addtolist tar($num_tars,libs_unix) $libs
}

#--------------------------------------------------------------------
#   setlibs_osx $libs
#   04-Mar-00   floh    created
#--------------------------------------------------------------------
proc setlibs_osx {libs} {
    global num_tars
    global tar
    addtolist tar($num_tars,libs_osx) $libs
}

#--------------------------------------------------------------------
#   setframeworks_osx $libs
#   04-Mar-00   floh    created
#--------------------------------------------------------------------
proc setframeworks_osx {libs} {
    global num_tars
    global tar
    addtolist tar($num_tars,frameworks_osx) $libs
}

#--------------------------------------------------------------------
#   setlibs_xbox360 $libs
#   18-Dec-02   floh    created
#--------------------------------------------------------------------
proc setlibs_xbox360 {libs} {
    global num_tars
    global tar
    addtolist tar($num_tars,libs_xbox360) $libs
}

#--------------------------------------------------------------------
#   setlibs_xbox360_debug $libs
#   31-Jan-03   floh    created
#--------------------------------------------------------------------
proc setlibs_xbox360_debug {libs} {
    global num_tars
    global tar
    addtolist tar($num_tars,libs_xbox360_debug) $libs
}

#--------------------------------------------------------------------
#   setlibs_xbox360_release $libs
#   31-Jan-03   floh    created
#--------------------------------------------------------------------
proc setlibs_xbox360_release {libs} {
    global num_tars
    global tar
    addtolist tar($num_tars,libs_xbox360_release) $libs
}

#--------------------------------------------------------------------
#   setlibs_xbox360_public_build $libs
#   31-Jan-03   floh    created
#--------------------------------------------------------------------
proc setlibs_xbox360_public_build {libs} {
    global num_tars
    global tar
    addtolist tar($num_tars,libs_xbox360_public_build) $libs
}

#--------------------------------------------------------------------
#   setlibs_ps3 $libs
#--------------------------------------------------------------------
proc setlibs_ps3 {libs} {
    global num_tars
    global tar
    addtolist tar($num_tars,libs_ps3) $libs
}

#--------------------------------------------------------------------
#   setlibs_ps3_debug $libs
#--------------------------------------------------------------------
proc setlibs_ps3_debug {libs} {
    global num_tars
    global tar
    addtolist tar($num_tars,libs_ps3_debug) $libs
}

#--------------------------------------------------------------------
#   setlibs_ps3_release $libs
#--------------------------------------------------------------------
proc setlibs_ps3_release {libs} {
    global num_tars
    global tar
    addtolist tar($num_tars,libs_ps3_release) $libs
}

#--------------------------------------------------------------------
#   setlibs_ps3_public_build $libs
#--------------------------------------------------------------------
proc setlibs_ps3_public_build {libs} {
    global num_tars
    global tar
    addtolist tar($num_tars,libs_ps3_public_build) $libs
}

#--------------------------------------------------------------------
#   setlibs_ps3_distribute_assert $libs
#--------------------------------------------------------------------
proc setlibs_ps3_distribute_assert {libs} {
    global num_tars
    global tar
    addtolist tar($num_tars,libs_ps3_distribute_assert) $libs
}

#--------------------------------------------------------------------
#   setrsrc_win32 $libs
#   04-Mar-00   floh    created
#--------------------------------------------------------------------
proc setrsrc_win32 {file_list} {
    global num_tars
    global tar
    addtolist tar($num_tars,rsrc_win32) $file_list
}

#--------------------------------------------------------------------
#   setlibs_wii_debug $libs
#   11-Jan-07   M.Mielitz    created
#--------------------------------------------------------------------
proc setlibs_wii_debug {libs} {
    global num_tars
    global tar
    addtolist tar($num_tars,libs_wii_debug) $libs
}

#--------------------------------------------------------------------
#   setlibs_wii_debug $libs
#   11-Jan-07   M.Mielitz    created
#--------------------------------------------------------------------
proc setlibs_wii_release {libs} {
    global num_tars
    global tar
    addtolist tar($num_tars,libs_wii_release) $libs
}

#--------------------------------------------------------------------
#   endtarget
#   04-Mar-00   floh    created
#--------------------------------------------------------------------
proc endtarget { } {
    global num_tars
    incr num_tars
}

#--------------------------------------------------------------------
#   dumptargets
#   Print data of all defined targets.
#   04-Mar-00   floh    created
#--------------------------------------------------------------------
proc dumptargets { } {
    global tar
    global num_tars
    puts "\n\n======================================================="
    puts " TARGETS"
    puts " "
    for {set i 0} {$i < $num_tars} {incr i} {
        puts "-------------------------------------------------------"
        puts " target:              $tar($i,name)"
        puts " type:                $tar($i,type)"
        puts " mods:                $tar($i,mods)"
        puts " depends:             $tar($i,depends)"
        puts " libs_win32:          $tar($i,libs_win32)"
        puts " libs_unix:           $tar($i,libs_unix)"
        puts " libs_osx:            $tar($i,libs_osx)"
        puts " frameworks_osx:      $tar($i,frameworks_osx)"
        puts " rsrc_win32:          $tar($i,rsrc_win32)"
        puts " incpath_win32:       $tar($i,incpath_win32)"
        puts " libpath_win32:       $tar($i,libpath_win32)"    
        puts " defines:             $tar($i,defines)"    
        puts " additionaloptions:   $tar($i,additionaloptions)"  
    }
}

#====================================================================
#   SOLUTION FUNCTIONS
#====================================================================

#--------------------------------------------------------------------
#   beginsolution
#   Begin a new solution definition.
#--------------------------------------------------------------------
proc beginsolution {name platform} {
    global num_slns
    global sln
    set sln($num_slns,name) $name
    set sln($num_slns,targets) ""
    set sln($num_slns,platform) $platform    
}

#--------------------------------------------------------------------
#   settargets
#   Define targets in solution.
#--------------------------------------------------------------------
proc settargets {targets} {
    global num_slns
    global sln
    set sln($num_slns,targets) $targets
}

#--------------------------------------------------------------------
#   endsolution
#   Finish a solution definition.
#--------------------------------------------------------------------
proc endsolution {} {
    global num_slns
    incr num_slns
}

#====================================================================
#   PACKAGE HELPER FUNCTIONS
#====================================================================

#--------------------------------------------------------------------
#   gen_component_list
#   Generate a list of dependent components from a target dependency list.
#   The target itself will be included at the head of the list.
#--------------------------------------------------------------------
proc gen_component_list {target} {
global tar
global num_tars

    set component_list $target
    set t [findtargetbyname $target]
    for {set i 0} {$i < [llength $tar($t,depends)]} {incr i} {
        lappend component_list [lindex $tar($t,depends) $i]
    }
    return $component_list
}

#--------------------------------------------------------------------
#   get_dirmod
#   Return list of module indices, sorted by the module's directory.
#   09-Mar-00
#--------------------------------------------------------------------
proc get_dirmod {name} {
    global tar
    global num_tars
    global mod
    global num_mods

    set t [findtargetbyname $name]

    set dirmod_list ""
    for {set i 0} {$i < [llength $tar($t,mods)]} {incr i} {
        set m [findmodbyname [lindex $tar($t,mods) $i]]
        set dm "$m $mod($m,dir) $mod($m,name)"
        lappend dirmod_list $dm
    }

    set dirmod_list [lsort -index 1 [lsort -index 2 $dirmod_list]]
    set mod_list ""
    for {set i 0} {$i < [llength $dirmod_list]} {incr i} {
        lappend mod_list [lindex [lindex $dirmod_list $i] 0]
    }
    return $mod_list
}

#--------------------------------------------------------------------
# generate a sorted file list wher the elemnts have this structure:
# {filename type(header|source|nidl)} {...}
#--------------------------------------------------------------------
proc gen_filelist { header_list source_list nidl_list } {
    set filelist {}
    for {set i 0} {$i < [llength $header_list] } {incr i} {
        lappend filelist "[lindex $header_list $i] header"
    }
    for {set i 0} {$i < [llength $source_list] } {incr i} {
        lappend filelist "[lindex $source_list $i] source"
    }
    for {set i 0} {$i < [llength $nidl_list] } {incr i} {
        lappend filelist "[lindex $nidl_list $i] nidl"
    }
    set filelist [lsort $filelist]    
    return $filelist
}

#--------------------------------------------------------------------
#   extract_libs
#   Extract the link libs for a given target.
#--------------------------------------------------------------------
proc extract_libs {targetName libType} {
    global tar
    
    # puts "extract_libs: targetName=$targetName libType=$libType"    
    set i [findtargetbyname $targetName]
    
    set libList ""
    for {set j 0} {$j < [llength $tar($i,$libType)]} {incr j} {
        append libList "[lindex $tar($i,$libType) $j] "
    }
    # puts "extract_libs: result=$libList"    
    return $libList
}

#--------------------------------------------------------------------
#   writetargetguids
#   For each target, writes a small file with it's guid.
#--------------------------------------------------------------------
proc writetargetguids {solution_name} {
    global tar
    global num_tars
    global sln
    global vstudioPrefix

    set s [findsolutionbyname $solution_name]   
    set target_list $sln($s,targets)
    for {set i 0} {$i < [llength $target_list]} {incr i} {
        set t [findtargetbyname [lindex $target_list $i]]
        if {$tar($t,type) != "import"} {
            set cid [open "$vstudioPrefix/$tar($t,name).guid" w]
            puts $cid $tar($t,uuid)
            close $cid
        }            
    }
}

#--------------------------------------------------------------------
#   readtargetguids
#   Reads a specific target guid, this is for imported targets.
#--------------------------------------------------------------------
proc readtargetguid {target_name} {
    global tar
    global vstudioPrefix
    
    set t [findtargetbyname $target_name]
    set path "$tar($t,import_path)\\$vstudioPrefix\\$tar($t,name).guid"
    set cid [open $path r]
    set result [gets $cid]
    
    close $cid
    return $result
}

#--------------------------------------------------------------------
#   updatetargetguid
#   Reads a specific target guid or build a new one
#--------------------------------------------------------------------
proc buildtargetguid {target_name} {
    global tar
    global vstudioPrefix
    
    set t [findtargetbyname $target_name]
    set path "$tar($t,import_path)\\$vstudioPrefix\\$tar($t,name).guid"
    
    set result ""
    if [file exists $path] {
        set cid [open $path r]
        set result [gets $cid]
    } else {
        set result [string toupper [exec uuidgen]] 
    } 
    return $result
}

#====================================================================
#   Include generator functions for GNU Make and Visual Studio
#   Workspace files.
#====================================================================
puts "*** Building VisualStudio.NET solution"
source ../bin/makeslnbase.tcl
source ../bin/makeslnwin32.tcl
if {[file exists ../bin/makeslnxbox360.tcl]} {
    puts "*** Building XBox360 solution files"
    source ../bin/makeslnxbox360.tcl
}
if {[file exists ../bin/makeslnps3.tcl]} {
    puts "*** Building PS3 solution files"
    source ../bin/makeslnps3.tcl
}
if {[file exists ../bin/makeslnwii.tcl]} {
    puts "*** Building CodeWarrior XML solution files"
    source ../bin/makeslnwii.tcl
}
if {[file exists ../bin/makexcode.tcl]} {
    puts "*** Building OSX solution files"
    source ../bin/makexcode.tcl
}

#--------------------------------------------------------------------
#   EOF
#--------------------------------------------------------------------
