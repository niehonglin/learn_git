
# Set up environment variables in order to build from IoE SDK source
# If SDK_ROOT is not set, this script must be sourced from within
# a script or from a non-login bash shell using:
#
#  . sdkenv.sh

export SDK_ROOT=${SDK_ROOT:-$(pwd)}
export FW=$SDK_ROOT
export INTERNALDIR=$FW/Internal
export SRC_IOE=$SDK_ROOT
export IMAGEDIR=$SRC_IOE/image
export LIBDIR=$SRC_IOE/lib
export BINDIR=$SRC_IOE/bin
export TOOLDIR=$SRC_IOE/tool
export PRINTF="/usr/bin/printf"
