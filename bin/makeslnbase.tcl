#-------------------------------------------------------------------------------
#   makeslnbase.tcl
#
#   General functions for building VStudio project files.
#
#   (C) 2007 Radon Labs GmbH
#-------------------------------------------------------------------------------
set headerPrefix ".."
set sourcePrefix ".."
set vstudioPrefix "Invalid"
set vstudio9Prefix "vs9.0"
set cWarriorPrefix "cw"
set binPrefix "..\\..\\bin\\win32"
set genDirGroups 1
set indent_level 0

#-------------------------------------------------------------------------------
#   begin_noattrs_withtext [name]
#   Begin an xml node without attributes (so close it immediately) without a "\n" after the tag.
#-------------------------------------------------------------------------------
proc begin_noattrs_withtext { name } {
    global indent_level
    indent
    print "<$name>"
    incr indent_level
}

#-------------------------------------------------------------------------------
#   print_tagtext
#   Print a line to the xml file, without a "\n" after the text.
#-------------------------------------------------------------------------------
proc print_tagtext { str } {
    global cid
    puts -nonewline $cid $str
}

#-------------------------------------------------------------------------------
#   end_aftertext [name]
#   End and xml node, without an indent before the tag.
#-------------------------------------------------------------------------------
proc end_aftertext { name } {
    global indent_level
    incr indent_level -1
    print "</$name>\n"
}

#-------------------------------------------------------------------------------
#   print
#   Print a line to the xml file.
#-------------------------------------------------------------------------------
proc print { str } {
    global indent_level 
    global cid
    puts -nonewline $cid $str
}

#-------------------------------------------------------------------------------
#   indent
#   Print indent tabs.
#-------------------------------------------------------------------------------
proc indent { } {
    global indent_level 
    global cid   
    for {set i 0} {$i < $indent_level} {incr i} {
        puts -nonewline $cid "\t"
    }
}

#-------------------------------------------------------------------------------
#   begin [name]
#   Begin an xml node.
#-------------------------------------------------------------------------------
proc begin { name } {
    global indent_level
    indent
    print "<$name\n"
    incr indent_level
}

#-------------------------------------------------------------------------------
#   begin_noattrs [name]
#   Begin an xml node without attributes (so close it immediately).
#-------------------------------------------------------------------------------
proc begin_noattrs { name } {
    global indent_level
    indent
    print "<$name>\n"
    incr indent_level
}

#-------------------------------------------------------------------------------
#   close_head
#   Closes an xml node header.
#-------------------------------------------------------------------------------
proc close_head {} {
    indent
    print ">\n"
}

#-------------------------------------------------------------------------------
#   end [name]
#   End and xml node. If name is empty, a short cut close statement "/>"
#   will be written, otherwise a fulle "</name>".
#-------------------------------------------------------------------------------
proc end { name } {
    global indent_level
    incr indent_level -1
    indent
    if {"" == $name} {
        print "/>\n"
    } else {        
        print "</$name>\n"
    }
}

#-------------------------------------------------------------------------------
#   attr name value
#   Write an XML attribute.
#-------------------------------------------------------------------------------
proc attr { name value } {
    indent
    print "$name=\"$value\"\n"
}

#-------------------------------------------------------------------------------
#   end_project
#   Ends a project definitin in the xml file.
#-------------------------------------------------------------------------------
proc end_project { } {
    begin_noattrs "Globals"
    end "Globals"
    end "VisualStudioProject"
}

#-------------------------------------------------------------------------------
#   begin_configurations
#   Begins the configs block.
#-------------------------------------------------------------------------------
proc begin_configurations {} {
    begin_noattrs "Configurations"
}

#------------------------------------------------------------------------------
#   end_configurations
#   Ends the configs block.
#-------------------------------------------------------------------------------
proc end_configurations {} {
    end "Configurations"
    begin_noattrs "References"
    end "References"
}

#-------------------------------------------------------------------------------
#   begin_config [project] [platform] [config] [type=lib|exe|dll]
#   Begins a single configuration for a static link library.
#-------------------------------------------------------------------------------
proc begin_config { project platform config type } {
    begin "Configuration"
        attr "Name" "$config|$platform"
        attr "OutputDirectory" ".\\$platform\\$config"
        attr "IntermediateDirectory" ".\\$platform\\$config\\$project" 
        if {($type == "lib") || ($type == "job")} {        
            attr "ConfigurationType" "4"
        } elseif {$type == "nidl"} {        
            attr "ConfigurationType" "10"
        } elseif {$type == "exe"} {
            attr "ConfigurationType" "1"
        } elseif {($type == "dll") || ($type == "mll")} {
            attr "ConfigurationType" "2"
        }
        attr "UseOfMFC" "0"
        attr "ATLMinimizesCRunTimeLibraryUsage" "false"
        if {($config != "Debug") && 
            ($config != "Programming") && 
            ($config != "Maya_Debug") && 
            ($config != "Maya_Release") &&
            ($config != "Profile") &&
            ($config != "Profile_FastCap") &&
            ($config != "Release_NoLTCG")} {
			attr "WholeProgramOptimization" "1"
        } else {
			attr "WholeProgramOptimization" "0"
		}
    close_head
}

#-------------------------------------------------------------------------------
#   end_config
#   Ends a single configuration.
#-------------------------------------------------------------------------------
proc end_config {} {
    end "Configuration"
}

#-------------------------------------------------------------------------------
#   write_empty_tool [name]
#   Writes a generic, empty tool section.
#-------------------------------------------------------------------------------
proc write_empty_tool { name } {
    begin "Tool"
        attr "Name" $name
    end ""
}

#-------------------------------------------------------------------------------
#   write_prebuildstep_tool [command line] [description]
#   Write a pre build tool.
#-------------------------------------------------------------------------------
proc write_prebuildstep_tool { commandLine description } {
    begin "Tool"
        attr "Name" "VCPreBuildEventTool"
        attr "Description" "$description"
        attr "CommandLine" "$commandLine"
    end ""
}

#-------------------------------------------------------------------------------
#   write_files [platform] [target] [module_list]
#   Writes the file list to the current project. 
#   FIXME: support for nodirgroups
#-------------------------------------------------------------------------------
proc write_files { configs compilerTool target module_list } {

    global mod
    global num_mods
    global tar
    global headerPrefix
    global sourcePrefix
    global genDirGroups

    if {[llength $module_list] > 0} {
        begin_noattrs "Files"
            
            # add optional resource files
            set t [findtargetbyname $target]
            for {set j 0} {$j < [llength $tar($t,rsrc_win32)]} {incr j} {
                begin "File"
                    attr "RelativePath" "[lindex $tar($t,rsrc_win32) $j]"
                    close_head
                end "File"
            }
            
            # add optional stdneb.cc file
            if {"" != $tar($t,precompiledheader)} {
                begin "File"
                    attr "RelativePath" "$sourcePrefix\\$tar($t,basedir)\\$tar($t,precompiledheader).cc"
                    close_head
                    for {set k 0} {$k < [llength $configs]} {incr k} {
                        set config [lindex $configs $k]
                        begin "FileConfiguration"
                            attr "Name" $config
                            close_head
                            begin "Tool"
                                attr "Name" $compilerTool
                                attr "UsePrecompiledHeader" "1"
                            end ""
                        end "FileConfiguration"   
                    }
                end "File"
            }
            
            # add source and header files
            set cur_dir "none"
            set subLevels 0
            for {set k 0} {$k < [llength $module_list]} {incr k} {
                set m  [lindex $module_list $k]
                
                set filterOpenend "false"
                
                # check if we need to start a new directory group
                set cur_dir_root [lindex [split $cur_dir "/\\"] 0];
                set mod_dir_root [lindex [split $mod($m,dir) "/\\"] 0];
                if {$genDirGroups && ($cur_dir_root != $mod_dir_root)} {
                    # end current directory group
                    if {$cur_dir != "none"} {
                        end "Filter"
                    }
                    # start a new group
                    begin "Filter"
                        # only the last path should be name of the filter
                        set pathList [split $mod($m,dir) "/\\"]
                        set filterName [lindex $pathList [expr [llength $pathList] - 1]]
                        
                        attr "Name" "$filterName"
                        attr "Filter" "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;cc"
                        close_head
                    set cur_dir $mod($m,dir)
                }
                
                # create filter for each module and add files
                set createModuleFilter "false"
                set endFilter "false"
                if {$genDirGroups} {
                    # only the last path should be name of the filter
                    set pathList [split $mod($m,name) "/\\"]
                    set filterName [lindex $pathList [expr [llength $pathList] - 1]]
                    
                    # do not create the filter if parent filter has the same name                
                    if {($cur_dir != "none") && ($mod_dir_root != $mod($m,dir))} {
                        set createModuleFilter "true"
                        set endFilter "true"
                        #check if next module has sublevel
                        if {([expr $k + 1] < [llength $module_list])} {                        
                            set nextM  [lindex $module_list [expr $k + 1]]
                            set nextPathList [split $mod($nextM,name) "/\\"]
                            if {[llength $pathList] < [llength $nextPathList]} {
                                incr subLevels
                                set endFilter "false"
                            }
                        }
                    }
                    
                    if {$createModuleFilter == "true"} {
                        begin "Filter"            
                            attr "Name" "$filterName"
                            attr "Filter" "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;cc"
                            close_head
                    }
                }

                # generate a sorted file list
                set file_list [gen_filelist $mod($m,headers) $mod($m,files) $mod($m,nidls)]
                
                for {set f 0} {$f < [llength $file_list]} {incr f} {
                    set file [lindex $file_list $f]
                    set file_name [lindex $file 0]
                    set file_type [lindex $file 1]

                    # header or source?
                    if {"header" == $file_type} {
                        begin "File"
                            attr "RelativePath" "$headerPrefix\\$tar($t,basedir)\\$mod($m,dir)\\${file_name}"
                            close_head
                        end "File"
                    } elseif {"source" == $file_type} {
                        begin "File"
                            attr "RelativePath" "$sourcePrefix\\$tar($t,basedir)\\$mod($m,dir)\\${file_name}"
                            close_head
                        end "File"
                    } elseif {"nidl" == $file_type} {
                        begin "File"
                            attr "RelativePath" "$sourcePrefix\\$tar($t,basedir)\\$mod($m,dir)\\${file_name}"
                            close_head
                        end "File"
                    }
                }
                #if we are in sublevels
                if {$subLevels > 0} {
                    #check if next module has no sublevel, so we have to go up enough levels
                    set levelDiff 0
                    if {[expr $k + 1] < [llength $module_list]} {
                        set nextM  [lindex $module_list [expr $k + 1]]
                        set nextPathList [split $mod($nextM,name) "/\\"]
                        set levelDiff [expr [llength $pathList] - [llength $nextPathList]]
                    } else {
                       set levelDiff $subLevels 
                    }
                    if {$levelDiff > 0} {
                        for {set iL 0} {$iL < $levelDiff} {incr iL} {
                            end "Filter"
                            set subLevels [expr $subLevels - 1]
                        }
                    }
                }
                
                # close the module filter if created
                if {$genDirGroups && $endFilter == "true"} {
                    end "Filter"
                }
            }
            if {$genDirGroups} {
                end "Filter"
            }            
        end "Files"
    }
}

#-------------------------------------------------------------------------------
#   write_lib_tool [project] [platform] [config]
#   Write the librarian tool settings.
#-------------------------------------------------------------------------------
proc write_lib_tool { project platform config } {
    begin "Tool"
        set fileId "."
        append fileId [string tolower $config]
        attr "Name" "VCLibrarianTool"
        attr "OutputFile" ".\\$platform\\$project$fileId.lib"
    end ""
}


