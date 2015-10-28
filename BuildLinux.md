# Quickstart for Debian/Ubuntu Users #

Ubuntu users can use the following packages to install all build dependencies for QSciTE.  Please note that the QScintilla2 packages are currently only available for Ubuntu Hardy (8.04) and above.  If you're using an older version, you'll have to download and compile QScintilla2 from the source package linked to below.  All other packages are available for at least Ubuntu 7.10 and above.

`libqt4-core libqt4-dev libqt4-gui qt4-dev-tools

The above set of packages has also been tested on Debian Lenny (current Testing branch).

# Dependencies #

## Qt 4 ##

We're developing QSciTE on the latest stable version of Qt 4 (currently 4.3.4).  Generally speaking, any Qt4 release in the 4.3.x series will probably work.  If these libraries are available as packages for your distribution, I suggest using those; if you take this approach, be sure to install any development packages as well, and not just the libraries themselves.

If you cannot obtain binary packages, you'll have to download and install Qt 4 from source, available here:

http://www.trolltech.com/download?target=ftp://ftp.trolltech.com/qt/source/qt-x11-opensource-src-4.3.4.tar.gz

# Compile Procedure #

The following instructions assume that you have a working copy of GCC 4.x installed along with the libraries and development packages listed above.

  1. Obtain a copy of the QSciTE source tree (svn or source package).
  1. Unpack the archive, if applicable.
  1. Enter the source directory and run "qmake qscite-release.pro" to generate the Makefile.
    * If Qt 3.x is also installed, you may need to run "qmake-qt4 qscite-release.pro" instead.
  1. Run "make" to start the compilation.
  1. The executable is called "qscite-release" and is in the "build" directory.

Currently there are no installation rules generated for the Makefile by QMake (we just haven't gotten around to that yet :) ).  You'll have to copy the executable to your favorite installation path to install the application (or just run it where it is).

# Configuration Files #

During testing, you may encounter bugs that store bad values in the QSciTE configuration file.  If the bad configuration prevents you from starting QSciTE, you may need to remove the configuration file before proceeding.  On my system, the configuration is stored in ~/.config/QSciteTeam; however, some systems mays simply store it in ~/.QSciteTeam.  In any event, if you delete the configuration file, a new one will be generated from hard-coded defaults.