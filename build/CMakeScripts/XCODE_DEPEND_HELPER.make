# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.thi.Debug:
/Users/marcusmathiassen/dev/thi/Debug/thi:
	/bin/rm -f /Users/marcusmathiassen/dev/thi/Debug/thi


PostBuild.thi.Release:
/Users/marcusmathiassen/dev/thi/Release/thi:
	/bin/rm -f /Users/marcusmathiassen/dev/thi/Release/thi


PostBuild.thi.MinSizeRel:
/Users/marcusmathiassen/dev/thi/MinSizeRel/thi:
	/bin/rm -f /Users/marcusmathiassen/dev/thi/MinSizeRel/thi


PostBuild.thi.RelWithDebInfo:
/Users/marcusmathiassen/dev/thi/RelWithDebInfo/thi:
	/bin/rm -f /Users/marcusmathiassen/dev/thi/RelWithDebInfo/thi




# For each target create a dummy ruleso the target does not have to exist
