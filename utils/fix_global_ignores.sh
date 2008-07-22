#!/usr/bin/env sh
# Summit? 2008, Aim

# WARNING:
#  Running this script will wipe all previous svn:ignore propsets

if [ ! -f configure.ac ] ; then
  echo "You must run this script in the root of the trunk."
  exit 1
fi

echo "Updating svn:ignore"
for i in `find . -path '*/.svn' -prune -o -type d -print`; do
        if [ -f $i/.svn.ignore ] ; then
                echo "Merging $i/.svn.ignore with global"
                cat $i/.svn.ignore .svn.ignore.global | sort  | uniq > $i/.svn.ignore
                svn propset svn:ignore -F $i/.svn.ignore $i
        else
                echo .
                svn propset svn:ignore -F .svn.ignore.global $i
        fi
done

