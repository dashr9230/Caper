set BUILDTOOLS=buildtools

py %BUILDTOOLS%/incgen/incgen.py rwplcore stdarg.h stddef.h stdio.h stdlib.h
py %BUILDTOOLS%/incgen/incgen.py rwcore rwplcore.h
