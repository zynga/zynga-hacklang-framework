#!/bin/bash

setup_composer()
{

  composerFile=/usr/local/bin/composer

  if [ ! -f $composerFile ]; then
    # install the composer binary to /usr/local/bin
    curl https://getcomposer.org/installer | hhvm --php -- /dev/stdin --install-dir=/usr/local/bin --filename=composer
  fi

  if [ ! -f $composerFile ]; then
    echo "Failed to install composer to $composerFile";
    exit 255;
  fi

}

set -ex

# update the environment via apt
apt-get update -y

# install modules we need to use to test with.
apt-get install -y wget curl git make time

# Bring the latest composer into the environment
setup_composer;

# turn off the jit
echo hhvm.jit=0 >> /etc/hhvm/php.ini

# show the version of hhvm
hhvm --version

# swap to the source directory
cd /var/source

# run composer
composer update

if [ $? -ne 0 ]; then
  echo "Composer failed to update"
  exit $?
fi

# Dump the composer modules that were installed
echo "composer show"
composer show

# composer is available now run the vendor setup script
./bin/setup.sh

if [ $? -ne 0 ]; then
  echo "Dev environment failed to configure properly"
  exit $?
fi

echo "marking host as development"
touch /opt/zynga/is_development

make test

if [ $? -ne 0 ]; then
  echo "Make test failure"
  exit $?
fi

echo "BUILD: OK"
exit 0
