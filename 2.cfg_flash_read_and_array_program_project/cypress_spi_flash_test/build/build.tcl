
# Recommended build directory structure
#
# build/
# |    build.tcl
# |    sources.tcl
# |    *.ldf
# |    *.sty
# |    *.edf
# |    *.bit
# |
# |--- constraints/
# |    |    *.fdc
# |    |    *.lpf
# |
# |--- debug/
# |    |    *.hub
# |    |    *.rva
# |    |    *.rvl
# |    |    *.rvs
# |
# |--- reports/
# |    |    bitgen_report.txt
# |    |    fpga_pad_report.txt
# |    |    map_report.txt
# |    |    par_report.txt
# |    |    synplify_options.txt
# |    |    synth_report.txt
# |    |    timing_report.txt
# |
# |--- syn/
# |    |    syn.tcl (run_options.txt copied from tmp/)
# |
# |--- tmp/
# |    |    ALL TEMPORARY BUILD FILES (COPY THE ONES YOU DESIRE TO KEEP IN ONE OF THE OTHER DIRECTORIES)
# |


# I put these here so that if the build script fails mid build you don't have to manually type
# these commands before re-running the script.
catch "rvl_project close -force"
catch "prj_project close"

################################################################################

#
# BEGIN SCRIPT VARIABLE DECLARATIONS
#
#

# Build Folder/Directory definitions (IT'S NOT RECOMMENDED TO CHANGE THESE)

set impl_dir "tmp";                                         # Temporary dir for build related output files.  Gets deleted after build so copy out whatever you want to keep.
set constraints_dir "constraints";                          # Location of design constraint files
set debug_dir "debug";                                      # Location of Reveal Inserter & Analyzer debug files

# Project, Device, Debug, etc. options
# NOTE: THESE ARE THE ONLY THINGS IN THE BUILD SCRIPT THAT YOU SHOULD TYPICALLY HAVE TO CHANGE

set dev "LFE5U-85F-8BG756C"
set impl_name "impl1"
set top_name "cfg_fpga_array_prog";                                     # NOTE: bit file location & name will be ./impl_dir/top_name_impl_name.bit
set lpf_file "cfg_fpga_array_prog.lpf";                                 # Lattice Logical Preferences

set syn_constraints "no";                                   # yes = pass Synplify specific constraints file to Synplify, no = no Synplify specific constraints file
#set syn_constraints_file "adc_top.fdc";                     # Synplify synthesis constraints (optional)

#set insert_debug "yes";                                     # yes = insert reveal debug core, no = skip debug core insertion
#set debug_file "adc_top.rvl";                               # project file for Reveal Analyzer cores to insert


set clean_tmp "no";                                        # yes = delete all temp files and folder, no = don't delete (set to no to keep all temp files and folder to aide in debugging issues)


################################################################################


#
# BEGIN BUILD SCRIPT
#
# NOTE: YOU SHOULDN'T NEED TO CHANGE THE SCRIPT BEYOND THIS POINT VERY MUCH IF EVER.
#       IF YOU MUST, DO SO WITH CARE.
#

prj_project new -name $top_name -impl $impl_name  -impl_dir $impl_dir -dev $dev -lpf $constraints_dir/$lpf_file
prj_impl option top $top_name;                                           # need to set top option for reveal debug insertion
prj_strgy set_value map_io_reg=Both;                                     # force mapping of IO registers to IO cells
prj_strgy set_value map_ignore_pref_err=False par_ignore_pref_err=False; # DON'T ignore preference errors
prj_syn set synplify

if {$syn_constraints} {
    prj_src add $constraints_dir/$syn_constraints_file
}


#
# ADD DESIGN SOURCES
#

source ./sources.tcl


#
# SYNTHESIS (MUST BE DONE BEFORE ADDING REVEAL DEBUG)
#

prj_run Synthesis
prj_src remove -all; # remove all source hdl files so that Reveal Inserter doesn't bitch about systemVerilog files
file copy -force $impl_dir/$top_name\_$impl_name.edi ./$top_name.edf
prj_src add ./$top_name.edf; # add only the EDIF netlist


#
# INSERT REVEAL DEBUG CORE(S) (OPTIONAL)
#

if {$insert_debug} {
    prj_src add $debug_dir/$debug_file
    rvl_project open $debug_dir/$debug_file
    rvl_project run; # inserts debug core into design
    rvl_project close
}


#
# GENERATE BIT FILE & REPORTS
#

prj_run Translate; # run synthesis before this if your sources include hdl files and not just an EDIF
prj_run Map
prj_run PAR -task PARTrace
prj_run PAR -task IOTiming
prj_run Export -task Bitgen


#
# SAVE PROJECT SO THAT IT CAN ALSO BE OPENED IN THE LATTICE DIAMOND GUI IF DESIRED
#

# prj_src remove -all; # remove edif and add back source files for anyone who wants to open the project in Diamond
# source ./sources.tcl
prj_project save
prj_project close


#
# COPY IMPORTANT FILES TO MORE SENSIBLY NAMED DIRECTORIES
#

if {![file exists "reports"]} { file mkdir "reports" }

if {![file exists "syn"]} { file mkdir "syn" }

if {$insert_debug} {
    file copy -force $impl_dir/$top_name\_$impl_name.hub ./$debug_dir/$top_name.hub; # Used by Reveal Analyzer to sort the data coming from the different Reveal modules.
}

file copy -force $impl_dir/$top_name\_$impl_name.srr ./reports/synth_report.txt;     # Synplify output
file copy -force $impl_dir/$top_name\_$impl_name.mrp ./reports/map_report.txt;       # Map report
file copy -force $impl_dir/$top_name\_$impl_name.par ./reports/par_report.txt;       # Place & Route report
file copy -force $impl_dir/$top_name\_$impl_name.pad ./reports/pad_report.txt;       # FPGA PAD settings report
file copy -force $impl_dir/$top_name\_$impl_name.twr ./reports/timing_report.txt;    # PARTrace timing report (from PAR)
file copy -force $impl_dir/$top_name\_$impl_name.ior ./reports/io_timing_report.txt; # IOTiming timing report (from PAR)
file copy -force $impl_dir/$top_name\_$impl_name.bgn ./reports/bitgen_report.txt;    # Bitgen report
file copy -force $impl_dir/$top_name\_$impl_name.bit ./$top_name.bit;                # bit file

file copy -force $impl_dir/$top_name\_$impl_name\_synplify.tcl ./syn/syn.tcl;        # TCL script that Lattice Diamond used to control Synplify with (you can source this from synplify to create a gui Synplify project)


#
# CLEAN UP ANY TEMP DIRECTORIES & FILES
#

if {$clean_tmp} {
    file delete -force $impl_dir

    file delete -force $top_name\_tcr.dir

    foreach f [glob -nocomplain *.ini]  { file delete -force $f }
    foreach f [glob -nocomplain *.html] { file delete -force $f }
    foreach f [glob -nocomplain *.xml]  { file delete -force $f }
}
