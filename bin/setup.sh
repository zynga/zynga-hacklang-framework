#!/bin/bash

add_to_git_ignore()
{
  gitFile="$1/.gitignore"
  ignoreTarget=$2

  touch $gitFile;

  if [ ! -f $gitFile ]; then
    echo "  gitIgnore - gitFile=$gitFile not present"
    return;
  fi

  if grep -q $ignoreTarget $gitFile; then
    echo "  gitIgnore - $ignoreTarget - already ignored"
  else
    echo "  gitIgnore - $ignoreTarget - adding to ignoreFile=$gitFile"
    echo $ignoreTarget >> $gitFile
  fi

}

copy_from_skeleton()
{
  projectRoot=$1
  sourceFile=$2
  destFile=$3

  sourceChecksum=""
  if [ -f $sourceFile ]; then
    sourceChecksum=`md5sum "$sourceFile" | awk '{print $1'}`;
  fi

  destChecksum=""
  if [ -f $destFile ]; then
    destChecksum=`md5sum "$destFile" | awk '{print $1'}`;
  fi

  if [ "$sourceChecksum" == "$destChecksum" ]; then
    echo "  CopyFromSkeleton - destFile=$destFile - aok"
  else
    echo "  CopyFromSkeleton - sourceFile=$sourceFile and destFile=$destFile do not match"
    echo "  CopyFromSkeleton - sourceChecksum=$sourceChecksum"
    echo "  CopyFromSkeleton -  destChecksum=$destChecksum"
    cp $sourceFile $destFile
  fi

}

setup_dir()
{
  targetDir=$1

  if [ ! -d $targetDir ]; then
    echo "  setupDir - Attempting to create targetDir=$targetDir"
    mkdir -p $targetDir
  else
    echo "  setupDir - targetDir=$targetDir - aok"
    return
  fi

  if [ ! -d $targetDir ]; then
    echo "  setupDir  - Failed to setup targetDir=$targetDir"
    exit 255;
  fi

}

echo "STARTUP"

SOURCE="${BASH_SOURCE[0]}"

project_root="."
skeleton_root="$(dirname $(dirname "$SOURCE"))/src/skeleton"

echo "  projectRoot=$project_root"
echo "  skeletonRoot=$skeleton_root"

##
# Exclude our standard directories we care about.
##
echo "Setting up top level directories"
setup_dir "$project_root/tmp"
setup_dir "$project_root/vendor"

echo "Excluding top level directories from git"
add_to_git_ignore $project_root "vendor";
add_to_git_ignore $project_root "composer.lock";
add_to_git_ignore $project_root "tmp";

##
# Copy in our various files for the project.
##
echo "Copying in files from project skeleton"

# Copy over our hhconfig
copy_from_skeleton \
  $project_root \
  "$skeleton_root/.hhconfig" \
  "$project_root/.hhconfig"

add_to_git_ignore $project_root ".hhconfig"

# Copy over our makefile
copy_from_skeleton \
  $project_root \
  "$skeleton_root/Makefile" \
  "$project_root/Makefile"

add_to_git_ignore $project_root "Makefile"

# copy over the bootstrap file
copy_from_skeleton \
  $project_root \
  "$skeleton_root/bootstrap" \
  "$project_root/bootstrap.hh"

add_to_git_ignore $project_root "bootstrap.hh"

# copy over the phpunit.xml config
copy_from_skeleton \
  $project_root \
  "$skeleton_root/phpunit.xml" \
  "$project_root/phpunit.xml"

add_to_git_ignore $project_root "phpunit.xml"

echo "DONE"
