+---------------------------------------------------------------------------------------+
|                                                                                       |
|                       PowerManga port for GCW0 With Compo4All:                        |
|                                                                                       |
+---------------------------------------------------------------------------------------+

Compo 4 all changes:
--------------------
Right after having entered your name in the highscore table the game will try to
upload your score to Compo4All server, for this to work there are two prerequisite:
1- You need to have a C4All account settled up on your GCW, use
   "Sparrow Compo4all Manager" for that.
2- You need to have wifi activated and connected, using the "Wireless" application
   bundled with your GCW0.

Known problems:
---------------
- There's no possibility to enter a name in the Highscore screen at the moment, however,
  if you have a c4all account it will by default propose your shortname.
- If there's no internet connection the game will lock for 5seconds during gameover
  right after you'll validate your high score.

Based on:
---------

This port is based on PowerManga version 0.92 avaiable from
svn://svn.code.sf.net/p/powermanga/code/

Compo4All support is offered by ZIZ's SparrowNet library avaiable from 
https://github.com/theZiz/sparrow3d.git

Credits:
--------

- the original PowerManga team
- Ziz for its awesome sparrowNet library
- skeezix for Compo4All



+---------------------------------------------------------------------------------------+
|                                                                                       |
|                              Original PowerManga ReadMe:                              |
|                                                                                       |
+---------------------------------------------------------------------------------------+

Powermanga is an arcade 2D shoot-em-up game with 41 levels and more 
than 200 sprites. It runs in 320x200 or 640x400 pixels, 
with Window mode or full screen 
and support for 8, 15, 16, 24, and 32 bpp. 

PREREQUISITES

  You need:
  - A IA-32, x86-64, PowerPC, ARM or MIPS platform
  - 32 MB RAM and 80 MB swap
  - Linux 2.x 
  - SDL 1.2
  - SDL_mixer
  - the GCC C++ compiler
  - GNU make
  - (X.Org Server) 
  - (X11 with the MIT-SHM extension headers)
  - (LinuxThreads)

COMPILING AND INSTALLING


  To install in /usr/local :

       ./bootstrap
       ./configure
        make
        su -c 'make install'

LEGAL

  Powermanga is copyright TLK Games 1998-2012 and available under the
  terms of the GNU General Public License. See COPYING for details.
