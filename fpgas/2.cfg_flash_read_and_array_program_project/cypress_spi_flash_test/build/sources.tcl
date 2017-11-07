
set macro_search_path "./tmp;" ; # every diamond_sources.tcl script should append the paths to any IP/pre-compiled .ngo files they need

#
# TOP LEVEL PROJECT LOCAL SOURCES & IP
#

prj_src add ../hdl/cfg_fpga_array_prog.sv
prj_src add ../hdl/s25fl_qspi_reader.sv

# ECP5_SERIAL_SLAVE IP
prj_src add C:/FPGA/ip-library/ecp5_slave_serial_programmer/hdl/ecp5_slave_serial_programmer.sv

#prj_strgy set_value "bd_macro_search_path=$macro_search_path"; # needed in order for Translate to find IP core ngo files (since we remove all sources and only add back the edif file in build.tcl)