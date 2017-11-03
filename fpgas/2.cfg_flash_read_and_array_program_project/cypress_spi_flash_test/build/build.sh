#!/bin/bash

clear

rm -f build.log

pnmainc build.tcl 2>&1 | tee build.log

tail reports/timing_report.txt

# get rid of annoying ^M characters when viewing in vim
dos2unix build.log
dos2unix build.log
