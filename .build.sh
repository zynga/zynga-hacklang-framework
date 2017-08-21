#!/bin/bash

# run composer
composer update

if [ $? -ne 0 ]; then 
  echo "Composer failed to update"
  exit $?
fi

# Dump the composer modules that were installed
composer show

# composer is available now run the vendor setup script
./vendor/zynga/zynga-hhvm-moduledev/bin/setup.sh

if [ $? -ne 0 ]; then 
  echo "Moduledev failed to configure properly"
  exit $?
fi

make test

if [ $? -ne 0 ]; then 
  echo "Make test failure"
  exit $?
fi

echo "BUILD: OK"
exit 0
