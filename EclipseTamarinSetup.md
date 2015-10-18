# Eclipse CDT setup for Tamarin #

You can very well compile Tamarin without any IDE, but when it comes to edit the code, you should consider using one that provides code completion. Here are the settings for Eclipse CDT


First, install Eclipse + CDT, mercurial, zlib1g-dev

Now, inside Eclipse

  * create a "make c++ project"

  * copy `tamarin-central` inside and set it as source folder

  * create `obj-debug dir` in the root of the project

  * run configuration script:
```
$ cd obj-debug
$ chmod +x ../tamarin-central/configure.py
$ ../tamarin-central/configure.py --enable-shell --enable-debugger
```

CDT Project Settings (where tamarin-fcgi is the project name)
> - C/C++ Build -> Build directory: ${workspace\_loc:/tamarin-fcgi/obj-debug}

Hit the hammer icon in the toolbar to build


get rid of red warning
MMgc/GC.cpp: line 1298
```
		if(shiftAmount || dst != pageMap) {
			memmove(dst + shiftAmount, pageMap, numBytesToCopy);
			//vic
			if (shiftAmount)
				memset(dst, 0, shiftAmount);
```


You will also need the AS compiler, asc.jar. Put it in tamarin-central/utils.