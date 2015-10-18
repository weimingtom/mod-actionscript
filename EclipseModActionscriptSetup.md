# Setup mod\_actionscript in Eclipse CDT #

First, follow EclipseTamarinSetup instructions.

Then:

  * apt-get install zlib1g-dev
  * apt-get install libapache2-mod-fcgid
  * apt-get install libfcgi-dev
  * apt-get install libmysqlclient15-dev



copy fcgishell directory inside tamarin-central, under your project

edit tamarin-central/manifest.mk:
change line:
`$(call RECURSE_DIRS,shell)`
to:
`$(call RECURSE_DIRS,fcgishell)`

Build the project (click the hammer icon in CDT toolbar or press Ctrl+B - this runs make in your obj directory)

the executable is `obj-debug/fcgishell/fcgishell`



Whenever you change something in your `shell_toplevel.as`, and accordingly, in your c++ native classes, run shell\_toplevel.py. This will generate the needed c++ link code (shell\_toplevel.h and shell\_toplevel.cpp) and also the .abc native libraries thet the compiler uses. You need to have asc.jar in tamarin-central/utils for this.