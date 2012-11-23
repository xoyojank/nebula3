#-------------------------------------------------------------------------------
#   countlines.tcl
#   Recursively count lines in *.cc and *.h code.
#   (C) 2006 Radon Labs GmbH
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
#   CountLines
#   Count lines in a single files
#-------------------------------------------------------------------------------
proc CountLines { filename } {
    set numLines 0
    set cid [open $filename r]
    while {![eof $cid]} {
        set line [gets $cid]
        incr numLines
    }
    # puts "$filename: $numLines"
    close $cid
    return $numLines
}

#-------------------------------------------------------------------------------
#   RecurseCountLines
#   Count lines in all *.cc, *.cs and *.h files in current directory, and recurse
#-------------------------------------------------------------------------------
proc RecurseCountLines { numLines dirPattern countLines } {

    # get all *.cc and *.h files
    set dirNumLines 0
    if {$countLines} {
        set sourceFiles [glob -nocomplain *.cc *.cs *.h *.mel *.fx] 
        for {set i 0} {$i < [llength $sourceFiles]} {incr i} {
            incr dirNumLines [CountLines [lindex $sourceFiles $i]]
        }
    }
    puts "    [pwd] $dirNumLines"
    incr numLines $dirNumLines
    
    # recurse into directories    
    set dirs [glob -nocomplain -type d *]
    for {set i 0} {$i < [llength $dirs]} {incr i} {
        set curDir [lindex $dirs $i]
        if {($curDir != "CVS") && ($curDir != ".svn")} {
            # puts "-> entering dir $curDir"
            cd $curDir
            if {[string match $dirPattern $curDir]} {
                # count lines in this directory
                set numLines [RecurseCountLines $numLines $dirPattern 1]
            } else {
                # don't count lines in this directory
                set numLines [RecurseCountLines $numLines $dirPattern 0]
            }
            cd ..
        }
    }

    puts "[pwd] $numLines"
    return $numLines;
}

#-------------------------------------------------------------------------------
#   main
#   The first arguments indicates a directory pattern, if directory pattern does
#   not match, no lines will be counted in this directory.
#-------------------------------------------------------------------------------

set dirPattern "*"
if {$argc > 0} {
    set dirPattern [lindex $argv 0]
}

set numLines 0
set numLines [RecurseCountLines $numLines $dirPattern 1]
puts "$numLines"
return $numLines

