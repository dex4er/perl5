#! /bin/sh

# Support for Android

. ./hints/linux.sh

osvers="`$run getprop ro.build.version.release`"
afs='false'
d_csh='undef'
useshrplib='true'
d_libname_unique='define'
i_db='undef'
i_gdbm='undef'
i_ndbm='undef'
i_dbm='undef'
i_sdbm='undef'
useopcode='define'
man1dir=''
man3dir=''
man1ext=''
man3ext=''
unset siteman1
unset siteman3
unset vendorman1
unset vendorman3
installusrbinperl='undef'
i_locale='undef'
d_locconv='undef'
d_setlocale='undef'
d_setlocale_r='undef'
d_getservent_r='undef'
d_getpwnam_r='undef'
d_getpwuid_r='undef'
d_getpwent='undef'
d_useshellcmds='define'
d_libname_unique='define'
sh='/system/bin/sh'
libswanted="$libswanted m"
d_useshellcmds='define'
libs='-ldl -lm -lc'
libc='/system/lib/libc.so'
