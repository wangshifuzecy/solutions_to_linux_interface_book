#!/bin/sh
#
# consh_cleanup.sh
#
# (C) 2024, Michael Kerrisk
#
# Licensed under the GNU General Public License version 2 or later

usage()
{
cat << EOF 1>&2

Usage: $0 [-c <cgroup>] <overlay-dir>

<overlay-dir> is the location containing the components of the OverlayFS
	mount (lower, upper, work) and the mount (merged). The mount will be
	unmounted and the subdirectories will be removed.
-c <cgroup> is the cgroup directory used for the container. This directory will
	be removed.
EOF
    exit 1
}

cgroup=""
while getopts ":c:" optname "$@"; do
    case "$optname" in
    c)  cgroup=$OPTARG
        ;;
    *)  echo "Unknown option: $OPTARG" 1>&2
	usage
        ;;
    esac
done

shift $(expr $OPTIND - 1)

if test $# -lt 1; then
    echo "Missing argument" 1>&2
    usage
fi

if test "X$cgroup" != "X"; then
    cgpath="/sys/fs/cgroup/$cgroup"
    sudo rmdir $cgpath
fi

ovly_dir=$1

if mountpoint -q $ovly_dir/merged; then
    sudo umount $ovly_dir/merged
fi

rm -rf $ovly_dir/{merged,lower,work,upper}
