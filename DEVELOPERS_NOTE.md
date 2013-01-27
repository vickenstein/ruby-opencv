# DEVELOPER'S NOTE

## Requirement to develop ruby-opencv

* OpenCV
* Git
* Microsoft Visual C++ (for mswin32)
  * <http://www.microsoft.com/visualstudio/eng/downloads#d-2010-express>
* MinGW and MSYS (for mingw32)
  * gcc, g++ and MSYS are needed.
  * <http://www.mingw.org>
* Some gems (see Gemfile)
  * [bundler](https://github.com/carlhuda/bundler/)
  * [hoe](https://github.com/seattlerb/hoe)
  * [hoe-gemspec](https://github.com/flavorjones/hoe-gemspec)
  * [rake-compiler](https://github.com/luislavena/rake-compiler)
  * [gem-compile](https://github.com/frsyuki/gem-compile)


## Create ruby-opencv gem
Run the following commands.
When you use mingw32, use **MSYS console**, or when you use mswin32, 
use [**Visual Studio Command Prompt**](http://msdn.microsoft.com/en-us/library/ms229859.aspx)
instead of cmd.exe.

```
$ git clone git://github.com/ruby-opencv/ruby-opencv.git
$ cd ruby-opencv
$ git checkout master
$ bundle install
$ git ls-files > Manifest.txt
$ rake gem:spec
$ rake gem
```
**ruby-opencv-x.y.z.gem** will be created in pkg/ directory.

To create pre-build binaries, run the following commands in Windows.

```
$ cd pkg
$ gem compile ruby-opencv-*.gem
```

**ruby-opencv-x.y.z-x86-mingw32.gem** will be created when you use mingw32, or
**ruby-opencv-x.y.z-x86-mswin32.gem** when you use mswin32.


## Install ruby-opencv manually
### Linux/Mac

```
$ git clone git://github.com/ruby-opencv/ruby-opencv.git
$ cd ruby-opencv
$ git checkout master
$ ruby extconf.rb --with-opencv-dir=/path/to/opencvdir
$ make
$ make install
```

Note: **/path/to/opencvdir** is the directory where you installed OpenCV.


### Windows (mswin32)

Run the following commands on [**Visual Studio Command Prompt**](http://msdn.microsoft.com/en-us/library/ms229859.aspx).

```
$ git clone git://github.com/ruby-opencv/ruby-opencv.git
$ cd ruby-opencv
$ git checkout master
$ ruby extconf.rb --with-opencv-dir=C:\path\to\opencvdir\install  # for your own built OpenCV library
$ nmake
$ nmake install
```

To use pre-built OpenCV libraries, set the following option to extconf.rb.

```
$ ruby extconf.rb --with-opencv-include=C:\path\to\opencvdir\build\include --with-opencv-lib=C:\path\to\opencvdir\build\x86\vc10\lib
```


### Windows (mingw32)

Run the following commands on **MSYS console**.

```
$ git clone git://github.com/ruby-opencv/ruby-opencv.git
$ cd ruby-opencv
$ git checkout master
$ ruby extconf.rb --with-opencv-dir=/C/path/to/opencvdir/install  # for your own built OpenCV library
$ make
$ make install
```

To use pre-built OpenCV libraries, set the following option to extconf.rb.

```
$ ruby extconf.rb --with-opencv-include=/c/path/to/opencvdir/build/include --with-opencv-lib=/c/path/to/opencvdir/build/x86/mingw/lib
```


## Run tests

To run all tests, run **test/runner.rb**

```
$ cd ruby-opencv/test
$ ruby runner.rb
```

To run tests of the specified function, run a specific test with --name option.

The following sample runs tests for CvMat#initialize

```
$ cd ruby-opencv/test
$ ruby test_cvmat.rb --name=test_initialize
```

