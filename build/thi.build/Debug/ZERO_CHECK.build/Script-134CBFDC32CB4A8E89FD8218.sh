#!/bin/sh
make -C /Users/marcusmathiassen/dev/thi/build -f /Users/marcusmathiassen/dev/thi/build/CMakeScripts/ZERO_CHECK_cmakeRulesBuildPhase.make$CONFIGURATION OBJDIR=$(basename "$OBJECT_FILE_DIR_normal") all
