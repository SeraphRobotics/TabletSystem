Seraph shared libs

build from separate dir, keep source dir clean.  make directory ex: seraph-build.  run 'qmake ../seraphLibs/'  and then run make.
resolve any dependencies.

to build test suite, clear the build directory and run: 'qmake CONFIG+=test ../seraphLibs/' and then run make






