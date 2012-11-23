#-------------------------------------------------------------------------------
#   makeslnwin32.tcl
#
#   Generate VStudio project files for Win32 platform.
#
#   (C) 2005 Radon Labs GmbH
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
#   begin_project_win32
#   Starts a project definitin in the xml file.
#-------------------------------------------------------------------------------
proc begin_project_win32 { name guid } {
    print "<?xml version=\"1.0\" encoding=\"Windows-1252\"?>\n"
    
    begin "VisualStudioProject" 
        attr "ProjectType" "Visual C++"
        attr "Version" "9,00"
        attr "Name" $name
        attr "ProjectGUID" "\{$guid\}"
    close_head
    
    begin_noattrs "Platforms"
        begin "Platform"
            attr "Name" "Win32"
        end ""
    end "Platforms"
    
    begin_noattrs "ToolFiles"
        begin "ToolFile"
            attr "RelativePath" "..\\nidl.rules"
        end ""
    end "ToolFiles"
}

#-------------------------------------------------------------------------------
#   write_compiler_tool_win32 [project] [platform] [config]
#   Write the compiler tool settings.
#-------------------------------------------------------------------------------
proc write_compiler_tool_win32 { project platform config additionalIncPaths additionalDefines additionalOptions precompiledHeader} {
    global headerPrefix
    begin "Tool"
        attr "Name" "VCCLCompilerTool"
        if {"Maya_Debug" == $config} {
            attr "Optimization" "0"
            attr "AdditionalIncludeDirectories" "$headerPrefix;$additionalIncPaths;"
            attr "PreprocessorDefinitions" "__MAYA__;__WIN32__;WIN32;_DEBUG;NT_PLUGIN;_HAS_EXCEPTIONS=0;$additionalDefines" 
            attr "ExceptionHandling" "0"
            attr "BasicRuntimeChecks" "3"
            attr "RuntimeLibrary" "1"
            attr "BufferSecurityCheck" "true"
            attr "EnableFunctionLevelLinking" "false"
            attr "EnableEnhancedInstructionSet" "0"
            attr "RuntimeTypeInfo" "false"
            if {"" == $precompiledHeader} {
                attr "UsePrecompiledHeader" "0"
            } else {
                attr "UsePrecompiledHeader" "2"
                attr "PrecompiledHeaderThrough" "$precompiledHeader.h"
                attr "PrecompiledHeaderFile" "\$(OutDir)\\\$(ProjectName).pch"
            }
            attr "WarningLevel" "3"
            attr "WarnAsError" "true"
            attr "SuppressStartupBanner" "true"
            attr "Detect64BitPortabilityProblems" "false"
            attr "DebugInformationFormat" "4"
            attr "CallingConvention" "0"
            attr "CompileAs" "0"        
        } elseif {"Debug" == $config} {
            attr "AdditionalOptions" "$additionalOptions"
            attr "Optimization" "0"
            attr "AdditionalIncludeDirectories" "$headerPrefix;$additionalIncPaths;"
            attr "PreprocessorDefinitions" "__WIN32__;WIN32;_DEBUG;NT_PLUGIN;_HAS_EXCEPTIONS=0;$additionalDefines" 
            attr "StringPooling" "true"
            attr "MinimalRebuild" "true"
            attr "ExceptionHandling" "0"
            attr "BasicRuntimeChecks" "3"
            attr "RuntimeLibrary" "3"
            attr "BufferSecurityCheck" "true"
            attr "EnableFunctionLevelLinking" "true"
            attr "EnableEnhancedInstructionSet" "0"
            attr "FloatingPointModel" "2"
            attr "RuntimeTypeInfo" "false"
            if {"" == $precompiledHeader} {
                attr "UsePrecompiledHeader" "0"
            } else {
                attr "UsePrecompiledHeader" "2"
                attr "PrecompiledHeaderThrough" "$precompiledHeader.h"
                attr "PrecompiledHeaderFile" "\$(OutDir)\\\$(ProjectName).pch"
            }
            attr "WarningLevel" "3"
            attr "WarnAsError" "true"
            attr "SuppressStartupBanner" "true"
            attr "Detect64BitPortabilityProblems" "false"
            attr "DebugInformationFormat" "4"
            attr "CallingConvention" "0"
            attr "CompileAs" "0"
        } elseif {"Maya_Release" == $config} {
            attr "AdditionalOptions" "$additionalOptions"
            attr "Optimization" "2"
            attr "InlineFunctionExpansion" "1"
            attr "AdditionalIncludeDirectories" "$headerPrefix;$additionalIncPaths;"
            attr "PreprocessorDefinitions" "__MAYA__;__WIN32__;WIN32;NT_PLUGIN;_HAS_EXCEPTIONS=0;$additionalDefines" 
            attr "StringPooling" "true"
            attr "ExceptionHandling" "0"
            attr "WarningLevel" "3"
            attr "WarnAsError" "true"
            attr "SuppressStartupBanner" "true"
            attr "Detect64BitPortabilityProblems" "false"
            attr "RuntimeTypeInfo" "false"
            attr "DebugInformationFormat" "3"
            attr "CallingConvention" "0"
            attr "CompileAs" "0"
            if {"" == $precompiledHeader} {
                attr "UsePrecompiledHeader" "0"
            } else {
                attr "UsePrecompiledHeader" "2"
                attr "PrecompiledHeaderThrough" "$precompiledHeader.h"
                attr "PrecompiledHeaderFile" "\$(OutDir)\\\$(ProjectName).pch"
            }
        } else {
            # Release
            attr "AdditionalOptions" "$additionalOptions"
            attr "Optimization" "4"
            attr "InlineFunctionExpansion" "2"
            attr "EnableIntrinsicFunctions" "true"
            attr "FavorSizeOrSpeed" "1"
            attr "OmitFramePointers" "true"
            if {"Programming" == $config} {
                attr "WholeProgramOptimization" "false"
            } else {
                attr "WholeProgramOptimization" "true"
            }            
            attr "AdditionalIncludeDirectories" "$headerPrefix;$additionalIncPaths;"
            
            if {"Public_Build" == $config} {
                attr "PreprocessorDefinitions" "__WIN32__;WIN32;NT_PLUGIN;_HAS_EXCEPTIONS=0;PUBLIC_BUILD=1;$additionalDefines" 
            } elseif {"Securom" == $config} {
                attr "PreprocessorDefinitions" "__WIN32__;WIN32;NT_PLUGIN;_HAS_EXCEPTIONS=0;PUBLIC_BUILD=1;SECUROM=1;$additionalDefines"                    
            } else {
                attr "PreprocessorDefinitions" "__WIN32__;WIN32;NT_PLUGIN;_HAS_EXCEPTIONS=0;$additionalDefines" 
            }                
            attr "StringPooling" "true"
            attr "ExceptionHandling" "0"
            attr "RuntimeLibrary" "2"
            attr "BufferSecurityCheck" "false"
            attr "EnableFunctionLevelLinking" "true"
            attr "EnableEnhancedInstructionSet" "0"
            attr "FloatingPointModel" "2"
            attr "RuntimeTypeInfo" "false"            
            attr "WarningLevel" "3"
            attr "WarnAsError" "true"
            attr "SuppressStartupBanner" "true"
            attr "Detect64BitPortabilityProblems" "false"
            attr "DebugInformationFormat" "3"
            attr "CallingConvention" "0"
            attr "CompileAs" "0"
            if {"" == $precompiledHeader} {
                attr "UsePrecompiledHeader" "0"
            } else {
                attr "UsePrecompiledHeader" "2"
                attr "PrecompiledHeaderThrough" "$precompiledHeader.h"
                attr "PrecompiledHeaderFile" "\$(OutDir)\\\$(ProjectName).pch"
            }
        }            
    end ""
}

#-------------------------------------------------------------------------------
#   write_rc_tool_win32 [project] [platform] [config]
#   Write the resource compiler tool settings.
#-------------------------------------------------------------------------------
proc write_rc_tool_win32 { project platform config } {
    begin "Tool"
        attr "Name" "VCResourceCompilerTool"
        if {($config == "Debug") || ($config == "Maya_Debug")} {
            attr "PreprocessorDefinitions" "_DEBUG"
        } else {
            attr "PreprocessorDefinitions" "NDEBUG"
        }
        attr "Culture" "1033"
    end ""
}

#-------------------------------------------------------------------------------
#   write_exe_linker_tool_win32 [project] [platform] [config] [exe] [libs]
#   Write the linker tool settings for executables.
#-------------------------------------------------------------------------------
proc write_linker_tool_win32 { project platform config exe ext libs additionalLibPaths} {

    global binPrefix
    global vstudioPrefix

    begin "Tool"
        attr "Name" "VCLinkerTool"
        attr "AdditionalDependencies" $libs
        attr "LinkIncremental" "0"
        if {($ext == "dll") || ($ext == "mll")} {
            # create dll
            attr "ImportLibrary" "$binPrefix\\$exe.lib"
        }        
        attr "SuppressStartupBanner" "true"
        if {"vs8.0" == $vstudioPrefix} {
            attr "AdditionalLibraryDirectories" ".\\$platform\\$config,$binPrefix,..\\lib\\win32_vc_i386;$additionalLibPaths"
        } else {
            attr "AdditionalLibraryDirectories" ".\\$platform\\$config,$binPrefix,..\\lib\\win32_vc9_i386;$additionalLibPaths"
        }
        
        set fileId "."
        append fileId [string tolower $config]
        if {($fileId == ".release") || ($fileId == ".maya_release")} {
            set fileId ""
        }
        attr "OutputFile" "$binPrefix\\$exe$fileId.$ext"
		if {($ext == "exe")} {
			attr "GenerateManifest" "true"
		}
        attr "GenerateDebugInformation" "true"
        attr "ProgramDatabaseFile" "$binPrefix\\$exe$fileId.pdb"
        attr "GenerateMapFile" "true"
        attr "MapFileName" "$binPrefix\\$exe$fileId.map"
        attr "MapExports" "true"
        
        if {"Securom" == $config} {        
            attr "AdditionalOptions" "/export:SecuROM,@1"
        }            
        attr "TargetMachine" "1"
    end ""
}

#-------------------------------------------------------------------------------
#   write_config_win32
#   Write a complete configuration fragment.
#-------------------------------------------------------------------------------
proc write_config_win32 { project platform config type additionalIncPaths additionalDefines additionalOptions additionalLibPaths libs } {
    global tar
    global num_tars
    global mod
    global num_mods

    set i [findtargetbyname $project]

    begin_config $project $platform $config $type
        if { $tar($i,preBuildStepCommandLine) != "" } {
            write_prebuildstep_tool $tar($i,preBuildStepCommandLine) $tar($i,preBuildStepDescription)
        } else {
            write_empty_tool "VCPreBuildEventTool"    
        }
        write_empty_tool "VCCustomBuildTool"
        write_empty_tool "VCXMLDataGeneratorTool"
        write_empty_tool "VCWebServiceProxyGeneratorTool"
        write_empty_tool "VCMIDLTool"
        write_compiler_tool_win32 $project $platform $config $additionalIncPaths $additionalDefines $additionalOptions $tar($i,precompiledheader)
        write_empty_tool "VCManagedResourceCompilerTool"
        write_rc_tool_win32 $project $platform $config
        write_empty_tool "VCPreLinkEventTool"
        if {($type == "lib") || ($type == "job")} {
            write_lib_tool $project $platform $config
        } else {
            write_linker_tool_win32 $project $platform $config $project $type $libs $additionalLibPaths
        }
        write_empty_tool "VCALinkTool"
        write_empty_tool "VCXDCMakeTool"
        write_empty_tool "VCBscMakeTool"
        write_empty_tool "VCFxCopTool"
        write_empty_tool "VCPostBuildEventTool"
    end_config
}    

#-------------------------------------------------------------------------------
#   gen_lib_win32
#   Generate a static link lib project for VStudio9
#-------------------------------------------------------------------------------
proc gen_lib_win32 { filename solution_name name guid subtype configs} {

    global cid
    global tar
    global num_tars
    global mod
    global num_mods
    global vstudio9Prefix
    global indent_level    

    puts "-> VS9.0: $name.vcproj"
    
    set i [findtargetbyname $name]
    set platform $tar($i,platform)
    set additionalIncPaths [extract_libs $name incpath_win32]
    set additionalDefines [extract_libs $name defines]
    set additionalOptions [extract_libs $name additionaloptions]
    set additionalLibPaths [extract_libs $name libpath_win32]

    set module_list [get_dirmod $name]
    
    # open .vcproj file
    set cid [open $vstudio9Prefix/$filename w]
    set indent_level 0
    
    # write XML stuff
    begin_project_win32 $name $guid
        set writeFiles ""
        begin_configurations
        for {set i 0} {$i < [llength $configs]} {incr i} {  
            set configName [lindex $configs $i]        
            # note: subtype can be "lib", "nidl" or "job"
            write_config_win32 $name "Win32" $configName $subtype $additionalIncPaths $additionalDefines $additionalOptions $additionalLibPaths ""
            lappend writeFiles "$configName|Win32"
        }
        end_configurations
        write_files $writeFiles "VCCLCompilerTool" $name $module_list    
    end_project
    
    # close .vcproj file
    close $cid
}

#-------------------------------------------------------------------------------
#   gen_exe_win32
#   Generate an executable (or DLL) project for VStudio9
#-------------------------------------------------------------------------------
proc gen_exe_win32 { filename solution_name name guid ext configs } {

    global cid
    global tar
    global num_tars
    global mod
    global num_mods
    global vstudio9Prefix
    global indent_level

    puts "-> VS9.0: $name.vcproj"
    
    set component_list [gen_component_list $name]    
    set i [findtargetbyname $name]
    set platform $tar($i,platform)

    # FIXME: these are not used yet
    set additionalIncPaths [extract_libs $name incpath_win32]
    set additionalDefines [extract_libs $name defines]
    set additionalOptions [extract_libs $name additionaloptions]
    set additionalLibPaths [extract_libs $name libpath_win32]   
    
    set win32_libs [extract_libs $name libs_win32]    
    set module_list [get_dirmod $name]
    
    # open .vcproj file
    set cid [open $vstudio9Prefix/$filename w]
    set indent_level 0
    
    # write XML stuff
    begin_project_win32 $name $guid
        set writeFiles ""
        begin_configurations
            for {set i 0} {$i < [llength $configs]} {incr i} {  
                set configName [lindex $configs $i] 
                set libs [string tolower "libs_win32_$configName"]    
                set config_libs [extract_libs $name $libs]        
                write_config_win32 $name "Win32" $configName $ext $additionalIncPaths $additionalDefines $additionalOptions $additionalLibPaths "$win32_libs $config_libs"
                lappend writeFiles "$configName|Win32"
            }
        end_configurations
        write_files $writeFiles "VCCLCompilerTool" $name $module_list            
    end_project
    
    # close .vcproj file
    close $cid
}

#--------------------------------------------------------------------
#   gen_solution9_win32
#   Generate complete solution file.
#--------------------------------------------------------------------
proc gen_solution9_win32 { sln_name configs } {
    global tar
    global num_tars
    global sln
    global num_slns
    global vstudio9Prefix
    
    set s [findsolutionbyname $sln_name]   
    puts "=> VS9.0: $sln($s,name).sln"  
    file mkdir $vstudio9Prefix

    # get all targets in the solution
    set target_list $sln($s,targets)

    # write .sln file
    set sln_cid [open $vstudio9Prefix/$sln($s,name).sln w]

    puts $sln_cid "Microsoft Visual Studio Solution File, Format Version 10.00"
    puts $sln_cid "# Visual Studio 2008"
    
    # first, generate guids for each target
    writetargetguids $sln_name

    # count the different module types in the solution (NIDLs, Libs, Targets)
    set numNIDLs 0
    set numLibs 0
    set numTargets 0
    set numImports 0
    set numJobs 0    
    for {set i 0} {$i < [llength $target_list]} {incr i} {
        set tname [lindex $target_list $i]
        set t [findtargetbyname $tname]
        if {$tar($t,type) == "nidl"} {
            incr numNIDLs
        } elseif {$tar($t,type) == "lib"} {
            incr numLibs
        } elseif {$tar($t,type) == "import"} {
            incr numImports        
        } elseif {$tar($t,type) == "job"} {
            incr numJobs            
        } else {
            incr numTargets
        }                    
    }

    # build toplevel folders for each project type
    if {$numNIDLs > 0} {
        puts $sln_cid "Project(\"{2150E333-8FDC-42A3-9474-1A3956D46DE8}\") = \"NIDLs\", \"NIDLs\", \"{798777e9-b38c-4974-bee3-b1cd45f6a1e6}\""
        puts $sln_cid "EndProject"
    }
    if {$numLibs > 0} {
        puts $sln_cid "Project(\"{2150E333-8FDC-42A3-9474-1A3956D46DE8}\") = \"Libs\", \"Libs\", \"{832A854F-2030-4EA4-86FE-EDEEC017E78E}\""
        puts $sln_cid "EndProject"
    }
    if {$numImports > 0} {
        puts $sln_cid "Project(\"{2150E333-8FDC-42A3-9474-1A3956D46DE8}\") = \"Imports\", \"Imports\", \"{366466BC-0D1F-4F03-B10B-B800B81D0A93}\""
        puts $sln_cid "EndProject"
    }
    if {$numTargets > 0} {
        puts $sln_cid "Project(\"{2150E333-8FDC-42A3-9474-1A3956D46DE8}\") = \"Exes\", \"Exes\", \"{3B2AD919-B940-473D-A9CC-62428540DEC5}\""
        puts $sln_cid "EndProject"
    }
    if {$numJobs > 0} {
        puts $sln_cid "Project(\"{2150E333-8FDC-42A3-9474-1A3956D46DE8}\") = \"Jobs\", \"Jobs\", \"{28B9CEF2-57A6-4ee5-8235-2014936D9FE7}\""
        puts $sln_cid "EndProject"
    }
    
    # for each internal project...
    for {set i 0} {$i < [llength $target_list]} {incr i} {
        set t [findtargetbyname [lindex $target_list $i]]

        # vcproj filename
        set vcproj_filename "$tar($t,name).vcproj"

        # write project file
        if {$tar($t,type) == "dll"} {
            gen_exe_win32 $vcproj_filename $sln($s,name) $tar($t,name) $tar($t,uuid) "dll" $configs
        } elseif {$tar($t,type) == "exe"} {
            gen_exe_win32 $vcproj_filename $sln($s,name) $tar($t,name) $tar($t,uuid) "exe" $configs
        } elseif {$tar($t,type) == "mll"} {
            gen_exe_win32 $vcproj_filename $sln($s,name) $tar($t,name) $tar($t,uuid) "mll" $configs
        } elseif {($tar($t,type) == "lib") || ($tar($t,type) == "nidl") || ($tar($t,type) == "job")} {
            gen_lib_win32 $vcproj_filename $sln($s,name) $tar($t,name) $tar($t,uuid) $tar($t,type) $configs
        } elseif {$tar($t,type) == "import"} {
            set vcproj_filename "$tar($t,import_path)\\$vstudio9Prefix\\$tar($t,name).vcproj"
        } else {
            puts "ERROR: UNKNOWN TARGET TYPE '$tar($t,type)'"
            exit
        }
        
        puts $sln_cid "Project(\"{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}\") = \"$tar($t,name)\", \"$vcproj_filename\", \"{$tar($t,uuid)}\""
        
        # project dependencies
        set num_deps [llength $tar($t,depends)]
        if {$num_deps > 0} {
            puts $sln_cid "\tProjectSection(ProjectDependencies) = postProject"
            for {set k 0} {$k < $num_deps} {incr k} {
                set dtar [lindex $tar($t,depends) $k]
                set d [findtargetbyname $dtar]
                puts $sln_cid "\t\t\{$tar($d,uuid)\} = \{$tar($d,uuid)\}"
            }
            puts $sln_cid "\tEndProjectSection"
        }
        puts $sln_cid "EndProject"
    }
    
    # global settings
    puts $sln_cid "Global"
    puts $sln_cid "\tGlobalSection(SolutionConfigurationPlatforms) = preSolution"
    for {set j 0} {$j < [llength $configs]} {incr j} {  
            set configName [lindex $configs $j]
        puts $sln_cid "\t\t$configName|Win32 = $configName|Win32"
    }
    puts $sln_cid "\tEndGlobalSection"    
    puts $sln_cid "\tGlobalSection(ProjectConfigurationPlatforms) = postSolution"
    for {set i 0} {$i < [llength $target_list]} {incr i} {
        set t [findtargetbyname [lindex $target_list $i]]
        for {set j 0} {$j < [llength $configs]} {incr j} {  
            set configName [lindex $configs $j]      
            puts $sln_cid "\t\t{$tar($t,uuid)}.$configName|Win32.ActiveCfg = $configName|Win32"
            puts $sln_cid "\t\t{$tar($t,uuid)}.$configName|Win32.Build.0 = $configName|Win32"                
        }
    }
    puts $sln_cid "\tEndGlobalSection"
    puts $sln_cid "\tGlobalSection(SolutionProperties) = preSolution"
    puts $sln_cid "\t\tHideSolutionNode = FALSE"
    puts $sln_cid "\tEndGlobalSection"

    # write nested projects
    puts $sln_cid "\tGlobalSection(NestedProjects) = preSolution"
    for {set i 0} {$i < [llength $target_list]} {incr i} {
        set tname [lindex $target_list $i]
        set t [findtargetbyname $tname]
        if {$tar($t,type) == "nidl"} {
            # note: the NIDLs group is hardwired
            puts $sln_cid "\t\t{$tar($t,uuid)} = {798777e9-b38c-4974-bee3-b1cd45f6a1e6}"
        } elseif {$tar($t,type) == "job"} {
            # note: the Job group GUID is hardwired
            puts $sln_cid "\t\t{$tar($t,uuid)} = {28B9CEF2-57A6-4ee5-8235-2014936D9FE7}"                        
        } elseif {$tar($t,type) == "lib"} {
            # note: the Lib group GUID is hardwired
            puts $sln_cid "\t\t{$tar($t,uuid)} = {832A854F-2030-4EA4-86FE-EDEEC017E78E}"
        } elseif {$tar($t,type) == "import"} {
            puts $sln_cid "\t\t{$tar($t,uuid)} = {366466BC-0D1F-4F03-B10B-B800B81D0A93}"
        } else {
            # note: the Target group GUID is hardwired
            puts $sln_cid "\t\t{$tar($t,uuid)} = {3B2AD919-B940-473D-A9CC-62428540DEC5}"
        }                    
    }
    puts $sln_cid "\tEndGlobalSection"
    puts $sln_cid "EndGlobal"
    close $sln_cid
}

#--------------------------------------------------------------------
#   gen_allsolutions9_win32
#   Generate all solutions 
#--------------------------------------------------------------------
proc gen_allsolutions9_win32 { {configs {"Debug" "Release" "Maya_Debug" "Maya_Release"}} } {
    global sln
    global num_slns
    global tar
    global num_tars
    global vstudio9Prefix
    global vstudioPrefix
    
    # set visual studio path prefix
    set vstudioPrefix $vstudio9Prefix
    
    # then generate solutions
    for {set i 0} {$i < $num_slns} {incr i} {
        
        if {$sln($i,platform) == "win32"} {

            puts ">>> $sln($i,name)"

            # first generate UUIDs for each target    
            for {set j 0} {$j < $num_tars} {incr j} {
                if {$tar($j,type) != "import"} {
                    set tar($j,uuid) [buildtargetguid $tar($j,name)]
                } else {
                    set tar($j,uuid) [readtargetguid $tar($j,name)]
                }        
            }
            gen_solution9_win32 $sln($i,name) $configs   
        }            
    }
}

