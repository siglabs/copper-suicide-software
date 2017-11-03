# Copper Suicide Array Program Project
This is a project for the CFG FPGA. It reads the Cypress SPI NOR flash and programs all array FPGAs.

Instructions for Programming FLASH
==================================

-   Copper Suicide Rev.2 uses Cypress S25FL512S as 512 MBit (64MByte) SPI Flash Memory. This particular part is not available in the existing drop down selection for SPI Serial Flash, when creating a .xcf file with operation “SPI Flash Erase, Program and Verify”.
-   Hence, replace a modified ispVM\_008.xdf file in path:- C:\\lscc\\RevealLA\\3.9\_x64\\data\\vmdata\\database\\flash.
-   Follow Steps 1-2-3 in order to program or erase SPI Flash Chip.

Step 1: Using Deployment Tool
=============================

-   Create and save a .xcf file for the operation " SPI Flash Erase Program Verify" to program and “SPI Flash Erase All” to erase.
-   Open Download Debugger from Diamond Programmer Standalone by selecting Design -&gt; Utilities -&gt; Deployment Tool
-   Choose “Tester”
-   Choose SVF- JTAG Chian,
-   Convert the .xcf file into SVF file.

Step 2: Editing generated SVF file
==================================

The following changes have to be made in order to allow 4-byte addressing for command instructions:-

-   Change the Sector Erase Command from 0xD8 to 0xDC (0x1B -&gt; 0x3B)
-   Change the Page Programming Command from 0x02 to 0x12 (0x40 -&gt; 0x48)
-   Change the Fast Read Command from 0x0B to 0x0C (0xD0 -&gt; 0x30)

Step 3: Using Download Debugger
===============================

-   Open Download Debugger from Diamond Programmer Standalone by selecting Design -&gt; Utilities -&gt; Download Debugger
-   Open the SVF file.
-   Click on Configuration Tab --&gt; Select “Cable and Port IO Setup”. Change Programming Speeds to x15.
-   Click on detect cable, once the cable gets detected click OK.
-   Press “F11” and try to execute instructions one by one to debug or Hit “Go” button to allow running entire program.
