#-------------------------------------------------------------------------------
#   makexcode.tcl
#
#   Generate project files for XCode IDE.
#
#   (C) 2010 Radon Labs GmbH
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
#   begin_project_osx
#   Writes the start of a project file.
#-------------------------------------------------------------------------------
proc begin_project_osx { } {
    global cid
    
    puts $cid "// !$*UTF8*$!"
    puts $cid "\{"
    puts $cid "    archiveVersion = 1;"
    puts $cid "    classes = \{"
    puts $cid "    \};"
    puts $cid "    objectVersion = 45;"
    puts $cid "    objects = \{"
}

#-------------------------------------------------------------------------------
#   end_project_osx
#   Writes the end of a project file.
#-------------------------------------------------------------------------------
proc end_project_osx { sln_name } {
    global cid
    
    puts $cid "    \};"
    puts $cid "    rootObject = PROJECT$sln_name /* Project object */;"
    puts $cid "\}"    
}

#-------------------------------------------------------------------------------
#   begin_section
#   Writes the start section statement in a XCode project file
#   (just the comment).
#-------------------------------------------------------------------------------
proc begin_section { name } {
    global cid
    
    puts $cid "/* Begin $name section */"    
}

#-------------------------------------------------------------------------------
#   end_section
#   Writes the end section statement in a XCode project file
#   (just the comment).
#-------------------------------------------------------------------------------
proc end_section { name } {
    global cid
    
    puts $cid "/* End $name section */"
}

#-------------------------------------------------------------------------------
#   write_target_file_refs
#   Write the file ref objects for a target.
#-------------------------------------------------------------------------------
proc write_target_file_refs { sln_name tar_name } {
    global cid
    global tar
    global num_tars
    global mod
    global num_mods

    set t [findtargetbyname $tar_name]

    begin_section "File Refs: $tar_name"

    # get module list of target
    set t [findtargetbyname $tar_name]
    set module_list [get_dirmod $tar_name]
    
    for {set i 0} {$i < [llength $module_list]} {incr i} {
        set m [lindex $module_list $i]
        set file_list [gen_filelist $mod($m,headers) $mod($m,files) $mod($m,nidls)]
        for {set j 0} {$j < [llength $file_list]} {incr j} {
            set file [lindex $file_list $j]
            set file_name [lindex $file 0]
            set file_type [lindex $file 1]
            
            set filePath "../$tar($t,basedir)/$mod($m,dir)/$file_name"
            set fileId $sln_name/$tar_name/$mod($m,name)/$file_name
            puts $cid "        FILE$fileId = \{ isa = PBXFileReference; name = $file_name; path = $filePath; sourceTree = \"<group>\"; \};"
        }
    }
    
    # write the output file ref
    if {$tar($t,type) == "lib"} {
        set targetId $sln_name/$tar_name    
        set file_name "lib$tar_name.a"
        puts $cid "        LIB$targetId = \{ isa = PBXFileReference; explicitFileType = archive.ar; includeInIndex = 0; path = $file_name; sourceTree = \"BUILT_PRODUCTS_DIR\"; \};"
    } elseif {$tar($t,type) == "exe"} {
        set targetId $sln_name/$tar_name
        set file_name "$tar_name.app"
        puts $cid "        EXE$targetId = \{ isa = PBXFileReference; explicitFileType = wrapper.application; includeInIndex = 0; path = $file_name; sourceTree = \"BUILT_PRODUCTS_DIR\"; \};"
    
        # also need to add file refs for all frameworks
        for {set i 0} {$i < [llength $tar($t,frameworks_osx)]} {incr i} {
            set framework [lindex $tar($t,frameworks_osx) $i]
            puts $cid "        FRAMEWORK$targetId/$framework = \{isa = PBXFileReference; lastKnownFileType = wrapper.framework; name = $framework; path = System/Library/Frameworks/$framework; sourceTree = SDKROOT; \};"
        }
    }
    
    end_section "File Refs: $tar_name"
}

#-------------------------------------------------------------------------------
#   write_target_buildfile_refs
#   Write the build file objects for a target.
#-------------------------------------------------------------------------------
proc write_target_buildfile_refs { sln_name tar_name } {
    global cid
    global tar
    global num_tars
    global mod
    global num_mods

    set t [findtargetbyname $tar_name]

    # write source files

    begin_section "Build Files: $tar_name"
    # get module list of target
    set t [findtargetbyname $tar_name]
    set module_list [get_dirmod $tar_name]
    
    for {set i 0} {$i < [llength $module_list]} {incr i} {
        set m [lindex $module_list $i]
        set file_list [gen_filelist $mod($m,files) "" ""]
        for {set j 0} {$j < [llength $file_list]} {incr j} {
            set file [lindex $file_list $j]
            set file_name [lindex $file 0]
            set file_type [lindex $file 1]
            
            set fileId $sln_name/$tar_name/$mod($m,name)/$file_name
            puts $cid "        BUILDFILE$fileId = \{ isa = PBXBuildFile; fileRef = FILE$fileId; \};"
        }
    }
    
    # write framework files
    if {$tar($t,type) == "exe"} {
        set targetId $sln_name/$tar_name
        for {set i 0} {$i < [llength $tar($t,frameworks_osx)]} {incr i} {
            set framework [lindex $tar($t,frameworks_osx) $i]
            puts $cid "        BUILDFRAMEWORK$targetId/$framework = \{isa = PBXBuildFile; fileRef = FRAMEWORK$targetId/$framework; \};"
        }
    }

    end_section "Build Files: $tar_name"
}

#-------------------------------------------------------------------------------
#   write_target_file_groups
#   Write file groups for one target.
#-------------------------------------------------------------------------------
proc write_target_file_groups { sln_name tar_name } {
    global cid
    global tar
    global num_tars
    global mod
    global num_mods

    begin_section "File Groups: $tar_name"

    # get module list of target
    set module_list [get_dirmod $tar_name]
    
    for {set i 0} {$i < [llength $module_list]} {incr i} {
        set m [lindex $module_list $i]
        
        # generate a unique group name and a human-readable name
        # NOTE: only use last part of path for a child-module
        # (e.g. core/debug -> debug)
        set groupName "$sln_name/$tar_name/$mod($m,name)"
        set displayedGroupName $mod($m,name)
        if {[llength [split $displayedGroupName "/\\"]] > 1} {
            set displayedGroupName [lindex [split $displayedGroupName "/\\"] 1]
        }
        
        # write group block
        puts $cid "        GROUP$groupName = \{"
        puts $cid "            isa = PBXGroup;"
        puts $cid "            children = ("

        # write sub groups of this group, these are groups
        # after this group which are made of several path components,
        # and where the first component is identical with the
        # current group name
        for {set j [expr $i + 1]} {$j < [llength $module_list]} {incr j} {
            set mm [lindex $module_list $j]
            set tokens [split $mod($mm,name) "/\\"]
            if {([llength $tokens] > 1) && ([lindex $tokens 0] == $mod($m,name))} {
                # found a child group
                set childGroupName "GROUP$sln_name/$tar_name/$mod($mm,name)"
                puts $cid "                $childGroupName,"
            } else {
                break
            }
        }		

        # write files in group 
        set file_list [gen_filelist $mod($m,headers) $mod($m,files) $mod($m,nidls)]
        for {set j 0} {$j < [llength $file_list]} {incr j} {
            set file [lindex $file_list $j]
            set file_name [lindex $file 0]
            set file_type [lindex $file 1]

            set fileId "FILE$groupName/$file_name"
            puts $cid "                $fileId,"
        }
        puts $cid "            );"
        puts $cid "            name = $displayedGroupName;"
        puts $cid "            sourceTree = \"<group>\";"
        puts $cid "        \};"
    }
    
    end_section "File Groups: $tar_name"
} 

#-------------------------------------------------------------------------------
#   write_target_framework_group
#   Write a group with refs to all linked frameworks for a target.
#-------------------------------------------------------------------------------
proc write_target_framework_group { sln_name tar_name } {
    global cid
    global tar
    global num_tars
    global mod
    global num_mods

    set t [findtargetbyname $tar_name]
    begin_section "Target Frameworks Group: $tar_name"
    set targetId $sln_name/$tar_name
    set groupName "$targetId"
    
    # write group block
    puts $cid "        FRAMEWORKGROUP$groupName = \{"
    puts $cid "            isa = PBXGroup;"
    puts $cid "            children = ("
    
    for {set i 0} {$i < [llength $tar($t,frameworks_osx)]} {incr i} {
        set framework [lindex $tar($t,frameworks_osx) $i]
        puts $cid "                FRAMEWORK$targetId/$framework,"
    }
    
    puts $cid "            );"
    puts $cid "            name = Frameworks;"
    puts $cid "            sourceTree = \"<group>\";"
    puts $cid "        \};"
    
    end_section "Target Frameworks Group: $tar_name"
}

#-------------------------------------------------------------------------------
#   write_target_main_group
#   Writes the main group for a target.
#-------------------------------------------------------------------------------
proc write_target_main_group { sln_name tar_name } {
    global cid
    global tar
    global num_tars
    global mod
    global num_mods
    
    begin_section "Target Main Group: $tar_name"

    # get module list of target
    set module_list [get_dirmod $tar_name]

    set groupName "$sln_name/$tar_name"

    # write group block
    puts $cid "        GROUP$groupName = \{"
    puts $cid "            isa = PBXGroup;"
    puts $cid "            children = ("

    # get module list of target
    set module_list [get_dirmod $tar_name]
    
    for {set i 0} {$i < [llength $module_list]} {incr i} {
        set m [lindex $module_list $i]
        
        # ignore sub modules
        if {[llength [split $mod($m,name) "/\\"]] == 1} {        
            # generate a unique group name
            set groupName "$sln_name/$tar_name/$mod($m,name)"
            puts $cid "                GROUP$groupName,"
        }
    }
    
    # if the target is an exe, write a group with all frameworks
    set t [findtargetbyname $tar_name]
    if {$tar($t,type) == "exe"} {
        puts $cid "                FRAMEWORKGROUP$sln_name/$tar_name,"
    }
    puts $cid "            );"
    puts $cid "            name = $tar_name;"
    puts $cid "            sourceTree = \"<group>\";"
    puts $cid "        \};"
    
    end_section "Target Main Group: $tar_name"
}

#-------------------------------------------------------------------------------
#   write_project_main_group
#   Writes the project main group.
#-------------------------------------------------------------------------------
proc write_project_main_group { sln_name } {
    global cid
    global tar
    global num_tars
    global sln
    global num_slns

    begin_section "Project Main Group: $sln_name"

    set s [findsolutionbyname $sln_name]
    
    set groupName "$sln_name"
    
    # write group block
    puts $cid "        GROUP$groupName = \{"
    puts $cid "            isa = PBXGroup;"
    puts $cid "            children = ("

    # get target list of solution
    set target_list $sln($s,targets)
    for {set i 0} {$i < [llength $target_list]} {incr i} {
        set tarName [lindex $target_list $i]
        puts $cid "                GROUP$sln_name/$tarName,"
    }
    puts $cid "                PRODUCTS$sln_name,"
    puts $cid "            );"
    puts $cid "            name = $sln_name;"
    puts $cid "            sourceTree = \"<group>\";"
    puts $cid "        \};"

    end_section "Project Main Group: $sln_name"
}

#-------------------------------------------------------------------------------
#   write_project_products_group
#   Writes the products group for the project.
#-------------------------------------------------------------------------------
proc write_project_products_group { sln_name } {
    global cid
    global tar
    global num_tars
    global sln
    global num_slns

    set s [findsolutionbyname $sln_name]
    set target_list $sln($s,targets)

    puts $cid "        PRODUCTS$sln_name = \{"
    puts $cid "            isa = PBXGroup;"
    puts $cid "            children = ("
    for {set i 0} {$i < [llength $target_list]} {incr i} {
        set tar_name [lindex $target_list $i]
        set t [findtargetbyname $tar_name]
        if {($tar($t,type) == "lib")} {
            set targetId $sln_name/$tar_name
            puts $cid "                LIB$targetId,"
        }
    }
    puts $cid "            );"
    puts $cid "            name = Products;"
    puts $cid "            sourceTree = \"<group>\";"
    puts $cid "        \};"
}

#-------------------------------------------------------------------------------
#   write_project_root
#   Write the project's root object.
#-------------------------------------------------------------------------------
proc write_project_root_object { sln_name } {
    global cid
    global tar
    global num_tars
    global sln
    global num_slns
    
    set s [findsolutionbyname $sln_name]
    set target_list $sln($s,targets)
    
    begin_section "PBXProject"
    
    puts $cid "        PROJECT$sln_name /* Project object */ = \{"
    puts $cid "            isa = PBXProject;"
    puts $cid "            buildConfigurationList = PROJBUILDCONFIGLIST$sln_name;"
    puts $cid "            compatibilityVersion = \"Xcode 3.1\";"
    puts $cid "            hasScannedForEncodings = 1;"
    puts $cid "            mainGroup = GROUP$sln_name;"
    puts $cid "            productRefGroup = PRODUCTS$sln_name;"
    puts $cid "            projectDirPath = \"\";"
    puts $cid "            projectRoot = \"\";"
    puts $cid "            targets = ("
    for {set i 0} {$i < [llength $target_list]} {incr i} {
        set tar_name [lindex $target_list $i]
        set t [findtargetbyname $tar_name]
        if {($tar($t,type) == "lib") || ($tar($t,type) == "exe")} {
            set targetId $sln_name/$tar_name
            puts $cid "                TARGET$targetId,"
        }
    }
    puts $cid "            );"
    puts $cid "        \};"

    end_section "PBXProject"
}

#-------------------------------------------------------------------------------
#   write_target_buildconfigurations
#   Write build config list for a target.
#-------------------------------------------------------------------------------
proc write_target_buildconfigurationlist { sln_name tar_name } {
    global tar
    global num_tars
    global sln
    global num_slns
    global cid

    set t [findtargetbyname $tar_name]
    set targetId $sln_name/$tar_name
    
    # first write configurations
    set sectionName "BuildConfigurations: $tar_name (lib)"
    begin_section $sectionName
    
    # Lib Debug Config
    puts $cid "        BUILDCONFIGDEBUG$targetId /* Debug */ = \{"
    puts $cid "            isa = XCBuildConfiguration;"
    puts $cid "            buildSettings = \{"
    puts $cid "                ALWAYS_SEARCH_USER_PATHS = NO;"
    puts $cid "                ARCHS = \"\$(ARCHS_STANDARD_32_64_BIT)\";"
    puts $cid "                COPY_PHASE_STRIP = NO;"
    puts $cid "                GCC_DYNAMIC_NO_PIC = NO;"
    puts $cid "                GCC_ENABLE_CPP_EXCEPTIONS = NO;"
    puts $cid "                GCC_ENABLE_CPP_RTTI = NO;"
    puts $cid "                GCC_ENABLE_FIX_AND_CONTINUE = YES;"
    puts $cid "                GCC_MODEL_TUNING = G5;"
    puts $cid "                GCC_OPTIMIZATION_LEVEL = 0;"
    puts $cid "                GCC_PRECOMPILE_PREFIX_HEADER = NO;"
    puts $cid "                GCC_PREPROCESSOR_DEFINITIONS = __OSX__;"
    puts $cid "                HEADER_SEARCH_PATHS = $tar($t,incpath_osx);"
    puts $cid "                INSTALL_PATH = /usr/local/lib;"
    puts $cid "                ONLY_ACTIVE_ARCH = YES;"
    puts $cid "                PRODUCT_NAME = $tar_name;"
    puts $cid "            \};"
    puts $cid "            name = Debug;"
    puts $cid "        \};"
    
    # Lib Release Config
    puts $cid "        BUILDCONFIGRELEASE$targetId /* Release */ = \{"
    puts $cid "            isa = XCBuildConfiguration;"
    puts $cid "            buildSettings = \{"
    puts $cid "                ALWAYS_SEARCH_USER_PATHS = NO;"
    puts $cid "                ARCHS = \"\$(ARCHS_STANDARD_32_BIT)\";"
    puts $cid "                DEBUG_INFORMATION_FORMAT = \"dwarf-with-dsym\";"
    puts $cid "                GCC_ENABLE_CPP_EXCEPTIONS = NO;"
    puts $cid "                GCC_ENABLE_CPP_RTTI = NO;"
    puts $cid "                GCC_MODEL_TUNING = G5;"
    puts $cid "                GCC_PRECOMPILE_PREFIX_HEADER = NO;"
    puts $cid "                GCC_PREPROCESSOR_DEFINITIONS = __OSX__;"
    puts $cid "                HEADER_SEARCH_PATHS = $tar($t,incpath_osx);"
    puts $cid "                INSTALL_PATH = /usr/local/lib;"
    puts $cid "                PRODUCT_NAME = $tar_name;"
    puts $cid "            \};"
    puts $cid "            name = Release;"
    puts $cid "        \};"
    end_section $sectionName
    
    # write config list for libs
    set sectionName "BuildConfigurationList: $tar_name (lib)"
    begin_section $sectionName

    puts $cid "        BUILDCONFIGLIST$targetId = \{"
    puts $cid "            isa = XCConfigurationList;"
    puts $cid "            buildConfigurations = ("
    puts $cid "                BUILDCONFIGDEBUG$targetId,"
    puts $cid "                BUILDCONFIGRELEASE$targetId,"
    puts $cid "            );"
    puts $cid "            defaultConfigurationIsVisible = 0;"
    puts $cid "            defaultConfigurationName = Release;"
    puts $cid "        \};"
    
    end_section $sectionName
}

#-------------------------------------------------------------------------------
#   write_target_dependencies
#   Writes the dependendency objects for the given target.
#-------------------------------------------------------------------------------
proc write_target_dependencies { sln_name tar_name } {
    global tar
    global num_tars
    global sln
    global num_slns
    global cid

    set targetId $sln_name/$tar_name

    set sectionName "Target Dependencies: $tar_name"
    begin_section $sectionName

    # for each dependency...
    set t [findtargetbyname $tar_name]
    for {set i 0} {$i < [llength $tar($t,depends)]} {incr i} {
        set dep [lindex $tar($t,depends) $i]
        set depId $targetId/$dep        
        
        # write PBXContainerItemProxy object
        puts $cid "        TARGETPROXY$depId = \{"
        puts $cid "            isa = PBXContainerItemProxy;"
        puts $cid "            containerPortal = PROJECT$sln_name;"
        puts $cid "            proxyType = 1;"
        puts $cid "            remoteGlobalIDString = TARGET$sln_name/$dep;"
        puts $cid "            remoteInfo = $dep;"
        puts $cid "        \};"        
        
        # write PBXTargetDependency object
        puts $cid "        TARGETDEPENDENCY$depId = \{"
        puts $cid "            isa = PBXTargetDependency;"
        puts $cid "            target = TARGET$sln_name/$dep;"
        puts $cid "            targetProxy = TARGETPROXY$depId;"
        puts $cid "        \};"
    
    }
    end_section $sectionName
}

#-------------------------------------------------------------------------------
#   write_project_buildconfigurationlist
#   Write the build configurations for the project.
#-------------------------------------------------------------------------------
proc write_project_buildconfigurationlist { sln_name } {
    global tar
    global num_tars
    global sln
    global num_slns
    global cid

    set sectionName "Project Build Configuration: $sln_name"
    begin_section $sectionName
    
    # project wide Debug config settings
    puts $cid "        PROJBUILDCONFIGDEBUG$sln_name /* Debug */ = \{"
    puts $cid "            isa = XCBuildConfiguration;"
    puts $cid "            buildSettings = \{"
    puts $cid "                ARCHS = \"\$(ARCHS_STANDARD_32_64_BIT)\";"
    puts $cid "                GCC_C_LANGUAGE_STANDARD = gnu99;"
    puts $cid "                GCC_OPTIMIZATION_LEVEL = 0;"
    puts $cid "                GCC_WARN_ABOUT_RETURN_TYPE = YES;"
    puts $cid "                GCC_WARN_UNUSED_VARIABLE = YES;"
    puts $cid "                ONLY_ACTIVE_ARCH = YES;"
    puts $cid "                SDKROOT = macosx10.6;"
    puts $cid "            \};"
    puts $cid "            name = Debug;"
    puts $cid "        \};"
    
    # project wide Release config settings
    puts $cid "        PROJBUILDCONFIGRELEASE$sln_name /* Release */ = \{"
    puts $cid "            isa = XCBuildConfiguration;"
    puts $cid "            buildSettings = \{"
    puts $cid "                ARCHS = \"\$(ARCHS_STANDARD_32_64_BIT)\";"
    puts $cid "                GCC_C_LANGUAGE_STANDARD = gnu99;"
    puts $cid "                GCC_WARN_ABOUT_RETURN_TYPE = YES;"
    puts $cid "                GCC_WARN_UNUSED_VARIABLE = YES;"
    puts $cid "                PREBINDING = NO;"
    puts $cid "                SDKROOT = macosx10.6;"
    puts $cid "            \};"
    puts $cid "            name = Release;"
    puts $cid "        \};"
    
    # project wide build configuration list
    puts $cid "        PROJBUILDCONFIGLIST$sln_name = \{"
    puts $cid "            isa = XCConfigurationList;"
    puts $cid "            buildConfigurations = ("
    puts $cid "                PROJBUILDCONFIGDEBUG$sln_name,"
    puts $cid "                PROJBUILDCONFIGRELEASE$sln_name,"
    puts $cid "            );"
    puts $cid "            defaultConfigurationVisible = 0;"
    puts $cid "            defaultConfigirationName = Release;"
    puts $cid "        \};"
    
    end_section $sectionName
}

#-------------------------------------------------------------------------------
#   write_target_buildphases
#   Write the build phases for a target.
#-------------------------------------------------------------------------------
proc write_target_buildphases { sln_name tar_name } {
    global tar
    global num_tars
    global sln
    global num_slns
    global mod
    global num_mods
    global cid

    set t [findtargetbyname $tar_name]
    set targetId $sln_name/$tar_name
    set module_list [get_dirmod $tar_name]

    set sectionName "SourcesBuildPhase: $tar_name"
    begin_section $sectionName
   
    # compile sources build-phase
    puts $cid "        BUILDPHASESOURCES$targetId = \{"
    puts $cid "            isa = PBXSourcesBuildPhase;"
    puts $cid "            buildActionMask = 2147483647;"
    puts $cid "            files = ("
    
    for {set i 0} {$i < [llength $module_list]} {incr i} {
        set m [lindex $module_list $i]
        set file_list [gen_filelist $mod($m,files) "" ""]
        for {set j 0} {$j < [llength $file_list]} {incr j} {
            set file [lindex $file_list $j]
            set file_name [lindex $file 0]
            set file_type [lindex $file 1]            
            set fileId $sln_name/$tar_name/$mod($m,name)/$file_name
            puts $cid "                BUILDFILE$fileId,"
        }
    }
    puts $cid "            );"
    puts $cid "            runOnlyForDeploymentPostprocessing = 0;"            
    puts $cid "        \};"    
    
    # linker build-phase
    if {$tar($t,type) == "exe"} {
        puts $cid "        BUILDPHASEFRAMEWORKS$targetId = \{"
        puts $cid "            isa = PBXFrameworksBuildPhase;"
        puts $cid "            buildActionMask = 2147483647;"
        puts $cid "            files = ("
        
        for {set i 0} {$i < [llength $tar($t,frameworks_osx)]} {incr i} {
            set framework [lindex $tar($t,frameworks_osx) $i]
            puts $cid "                BUILDFRAMEWORK$targetId/$framework,"
        }
        
        puts $cid "            );"
        puts $cid "            runOnlyForDeploymentPostprocessing = 0;"
        puts $cid "        \};"
    }
    
    end_section $sectionName
}

#-------------------------------------------------------------------------------
#   write_lib_target
#   Write the target block for a static link lib.
#-------------------------------------------------------------------------------
proc write_lib_target { sln_name tar_name } {
    global tar
    global num_tars
    global sln
    global num_slns
    global cid

    set t [findtargetbyname $tar_name]

    set sectionName "PBXNativeTarget: $tar_name (lib)"
    set targetId $sln_name/$tar_name
    
    begin_section $sectionName
    
    puts $cid "        TARGET$targetId = \{"
    puts $cid "            isa = PBXNativeTarget;"
    puts $cid "            buildConfigurationList = BUILDCONFIGLIST$targetId;"
    puts $cid "            buildPhases = ("
    puts $cid "                BUILDPHASESOURCES$targetId,"
    puts $cid "            );"
    puts $cid "            buildRules = ("
    puts $cid "            );"
    puts $cid "            dependencies = ("

    # write dependencies
    set t [findtargetbyname $tar_name]
    for {set i 0} {$i < [llength $tar($t,depends)]} {incr i} {
        set dep [lindex $tar($t,depends) $i]
        set depId $targetId/$dep        
        puts $cid "                    TARGETDEPENDENCY$depId,"
    }                

    puts $cid "            );"
    puts $cid "            name = $tar_name;"
    puts $cid "            productName = $tar_name;"
    puts $cid "            productReference = LIB$targetId;"
    puts $cid "            productType = \"com.apple.product-type.library.static\";"
    puts $cid "        \};"
    
    end_section $sectionName
}

#-------------------------------------------------------------------------------
#   write_exe_target
#   Write the target block for an executable.
#-------------------------------------------------------------------------------
proc write_exe_target { sln_name tar_name } {
    global tar
    global num_tars
    global sln
    global num_slns
    global cid

    set t [findtargetbyname $tar_name]

    set sectionName "PBXNativeTarget: $tar_name (app)"
    set targetId $sln_name/$tar_name
    
    begin_section $sectionName
    
    puts $cid "        TARGET$targetId = \{"
    puts $cid "            isa = PBXNativeTarget;"
    puts $cid "            buildConfigurationList = BUILDCONFIGLIST$targetId;"
    puts $cid "            buildPhases = ("
    puts $cid "                BUILDPHASESOURCES$targetId,"
    puts $cid "                BUILDPHASEFRAMEWORKS$targetId,"
    puts $cid "                );"
    puts $cid "                buildRules = ("
    puts $cid "                );"
    puts $cid "                dependencies = ("

    # write dependencies
    set t [findtargetbyname $tar_name]
    for {set i 0} {$i < [llength $tar($t,depends)]} {incr i} {
        set dep [lindex $tar($t,depends) $i]
        set depId $targetId/$dep        
        puts $cid "                    TARGETDEPENDENCY$depId,"
    }                

    puts $cid "                );"
    puts $cid "                name = $tar_name;"
    puts $cid "                productName = $tar_name;"
    puts $cid "                productReference = EXE$targetId;"
    puts $cid "                productType = \"com.apple.product-type.application\";"
    puts $cid "            \};"

    end_section $sectionName
}

#-------------------------------------------------------------------------------
#   gen_solution_xcode
#   Generate a single solution project for XCode
#-------------------------------------------------------------------------------
proc gen_solution_xcode { sln_name } {
    global tar
    global num_tars
    global sln
    global num_slns
    global vstudioPrefix
    global cid

    # first, generate guids for each target
    writetargetguids $sln_name
    
    set s [findsolutionbyname $sln_name]
    set projPath "$vstudioPrefix/$sln($s,name).xcodeproj"
    set projFile "$projPath/project.pbxproj"
    puts "=> XCode: $projPath"
    file mkdir $projPath
    
    # open the project file and write header
    set cid [open $projFile w]
    begin_project_osx
    
    # get all targets in solution
    set target_list $sln($s,targets)
    for {set i 0} {$i < [llength $target_list]} {incr i} {
        set t [findtargetbyname [lindex $target_list $i]]
        
        # write target file refs
        write_target_file_refs $sln_name $tar($t,name)
        
        # write target build file refs
        write_target_buildfile_refs $sln_name $tar($t,name)
        
        # write the target file groups
        write_target_file_groups $sln_name $tar($t,name)
        
        # write the target main group
        write_target_main_group $sln_name $tar($t,name)
        
        # write the actual target
        if {($tar($t,type) == "lib") || ($tar($t,type) == "exe")} {

            if {$tar($t,type) == "exe"} {
                write_target_framework_group $sln_name $tar($t,name)
            }

            write_target_dependencies $sln_name $tar($t,name)           
            write_target_buildconfigurationlist $sln_name $tar($t,name)
            write_target_buildphases $sln_name $tar($t,name)
        
            if {$tar($t,type) == "lib"} {
                write_lib_target $sln_name $tar($t,name)
            } elseif {$tar($t,type) == "exe"} {
                write_exe_target $sln_name $tar($t,name)
            }
        }
    }
    
    # write the products group for this project
    write_project_products_group $sln_name
    
    # write the projects main group
    write_project_main_group $sln_name
    
    # write the project-wide configuration list
    write_project_buildconfigurationlist $sln_name
    
    # write the project root object
    write_project_root_object $sln_name
        
    # close project file
    end_project_osx $sln_name
    close $cid
}

#-------------------------------------------------------------------------------
#   gen_allsolutions_xcode
#   Generate all solutions as XCode projects.
#-------------------------------------------------------------------------------
proc gen_allsolutions_xcode { } {
    global sln
    global num_slns
    global tar
    global num_tars
    global vstudioPrefix
    
    set vstudioPrefix "xcode"
    
    for {set i 0} {$i < $num_slns} {incr i} {
    
        if {$sln($i,platform) == "osx"} {
            # first generate UUIDs for each target    
            puts "> Build GUIDs for solution $sln($i,name)"
            for {set j 0} {$j < $num_tars} {incr j} {
                if {$tar($j,type) != "import"} {
                    set tar($j,uuid) [buildtargetguid $tar($j,name)]
                } else {
                    set tar($j,uuid) [readtargetguid $tar($j,name)]
                }        
            }
            puts ">>> $sln($i,name)"
            gen_solution_xcode $sln($i,name)
        }
    }
}
