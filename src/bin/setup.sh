#!/bin/bash

add_to_git_ignore() 
{
  gitFile="$1/.gitignore"
  ignoreTarget=$2

  if [ ! -f $gitFile ]; then
    echo "gitFile=$gitFile not present"
    return 255;
  fi

  if grep -q $ignoreTarget $gitFile; then
    echo "$ignoreTarget - already ignored"
  else
    echo "$ignoreTarget - adding to ignoreFile=$gitFile"
    echo $ignoreTarget >> $gitFile
  fi

  return 0;

}

checksum_file()
{
  targetFile=$1

  if [ ! -f $targetFile ]; then
    return "";
  }

  checksum=`md5sum $targetFile | awk '{print $1'}`

  return $checksum;

}

copy_from_skeleton()
{
  projectRoot=$1
  sourceFile=$2
  destFile=$3

  sourceChecksum = checksum_file $sourceFile;
  destChecksum = checksum_file $destFile;

  if [ $sourceChecksum -eq $destChecksum ]; then
    echo "$destFile - aok"
  else 
    echo "sourceFile=$sourceFile and destFile=$destFile do not match"
    echo "sourceChecksum=$sourceChecksum"
    echo "destChecksum=$destChecksum"
  fi

  add_to_git_ignore $projectRoot $destFile

}

setup_dir()
{
  targetDir=$1

  if [ ! -d $targetDir ]; then
    echo "Attempting to create targetDir=$targetDir"
    mkdir -p $targetDir
  else 
    echo "targetDir=$targetDir - aok"
    return 0
  fi

  if [ ! -d $targetDir ]; then
    echo "Failed to setup targetDir=$targetDir"
    return 255;
  fi

}

project_root="."

## 
# Exclude our standard directories we care about.
##
setup_dir "$project_root/tmp"
setup_dir "$project_root/vendor"

add_to_git_ignore $project_root "vendor";
add_to_git_ignore $project_root "tmp";

##
# Copy in our various files for the project.
##
echo "Copying in files from project skeleton"

# Copy over our makefile
copy_from_skeleton \
  $project_root \
  "$project_root/vendor/zynga/zynga-hhvm-moduledev/skeleton/Makefile" \
  "$project_root/Makefile"
