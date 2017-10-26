#!/bin/bash

echo "Welcome to Siglabs Embedded"

eclipse_down_link="http://mirror.csclub.uwaterloo.ca/eclipse/technology/epp/downloads/release/neon/3/eclipse-cpp-neon-3-linux-gtk-x86_64.tar.gz"
arm_gnu_tool_chain="https://developer.arm.com/-/media/Files/downloads/gnu-rm/6_1-2017q1/gcc-arm-none-eabi-6-2017-q1-update-linux.tar.bz2"
jlink_debugger="https://www.segger.com/downloads/jlink/JLink_Linux_V614h_i386.deb"

echo "Let's Download some files for you :p"

echo "change scope to Home"
cd $HOME
mkdir $HOME/embedded_utils

NEW_PATH=$HOME/embedded_utils

echo "Do you have the files downloaded? (y/n) "
read -n 1 my_prompt

if [ "$my_prompt" == "n" ]; then

		echo "We Download Eclipse FROM: $eclipse_down_link"
		echo "We Download ARM FROM: $arm_gnu_tool_chain"
		echo "We Download JLink Debugger FROM: $jlink_debugger"
		

		echo "if this did not work, please download the deps to $NEW_PATH"
		echo -e "***** Getting Eclipse *****\n\n"
		wget $eclipse_down_link -O $NEW_PATH/eclipse.gz

		echo -e "\n\n***** Getting Gnu ARM ToolChain *****\n\n"
		wget $arm_gnu_tool_chain -O $NEW_PATH/arm_gcc.tar.bz2

		echo -e "\n ***Downloading JLink Debugger tools ***\n"
		echo -e "\n +++--+++-- UNTESTED PORTION STARTS HERE --+++--+++\n"
		echo -e "Please note: you need to revisit this script say (y) in \
				 the prompt to install every thing :) \n\
				 Rename: JLink_Linux_V616_i386.deb to jlink_debugger.deb in\ 
				 downloads.\
				 "
		xdg-open $jlink_debugger
else
    echo "you are on your own :P"
		echo -e "\n\n***** extracting Eclipse *****\n\n"
		tar -xzvf $NEW_PATH/eclipse.gz --directory $NEW_PATH
		echo -e "\n\n***** Extracting Gnu ARM ToolChain *****\n\n"
		tar -xvf $NEW_PATH/arm_gcc.tar.bz2 --directory $NEW_PATH
		mv $NEW_PATH/gcc-arm*/ $NEW_PATH/arm_gcc
		echo -e "\n\n***** Adding to arm_gnu bins to zshrc *****\n\n"
		echo "export PATH=\$PATH:$NEW_PATH/arm_gcc/bin" >> ~/.zshrc
		source ~/.zshrc
		echo -e "\n +++--+++-- UNTESTED PORTION STARTS HERE --+++--+++\n"
		sudo dpkg -i ~/Downloads/JLink_Linux_V616_i386.deb 
fi
