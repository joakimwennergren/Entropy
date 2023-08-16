#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/joakim/Desktop/Symbios/build-iOS
  make -f /Users/joakim/Desktop/Symbios/build-iOS/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/joakim/Desktop/Symbios/build-iOS
  make -f /Users/joakim/Desktop/Symbios/build-iOS/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/joakim/Desktop/Symbios/build-iOS
  make -f /Users/joakim/Desktop/Symbios/build-iOS/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/joakim/Desktop/Symbios/build-iOS
  make -f /Users/joakim/Desktop/Symbios/build-iOS/CMakeScripts/ReRunCMake.make
fi

