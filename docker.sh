#!/bin/bash

#https://medium.com/@hudsonmendes/docker-have-a-ubuntu-development-machine-within-seconds-from-windows-or-mac-fd2f30a338e4

docker run \
  --name ubuntu \
  -e HOST_IP=$(ifconfig en0 | awk '/ *inet /{print $2}') \
  -v /Users/ngcchk/Downloads/_Github/solitaire-cli:/src \
  -t -i \
  ubuntu /bin/bash

  # already root no sudo 

  #apt-get update
  #apt-get install build-essential
  #apt-get install libncurses5-dev libncursesw5-dev

  #setup UTF-8 still not ok

  # dpkg -l locales

#apt-get install locales


#dpkg-reconfigure locales

#apt-get install vim
#.bashrc b

#export LC_ALL=en_US.UTF-8
#export LANG=en_US.UTF-8
#export LANGUAGE=en_US.UTF-8

#locale  #to check

#make ascii
#./solitaire

#apt-get install gdb
# not sure about 

#solitaire-cli > c4c8
Program received signal SIGSEGV, Segmentation fault.
                                                    0x0000564565480953 in is_empty (pile=0x21) at main.c:198
198	int is_empty(pile *pile) { return pile->num_cards == 0; }
(gdb) 