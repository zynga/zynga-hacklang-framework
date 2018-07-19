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

export DEBIAN_FRONTEND=noninteractive

# stand up a memcached on the image
apt-get install -y memcached
/etc/init.d/memcached start

# install mysql-server
apt-get install -y mysql-server
/etc/init.d/mysql start

# load up our test database
mysql < /var/source/tests/sql/mysql/create_test_database.sql
mysql -e 'SHOW DATABASES'
mysql -e 'SHOW TABLES' phpunit

# install postgresql
apt-get install -y postgresql postgresql-contrib
/etc/init.d/postgresql start

# install the database user.
su postgres -c 'psql < /var/source/tests/sql/postgresql/create_test_database.sql'

# setup the postgres password into the .pgpass property file.
echo "localhost:5432:phpunit:zframework:i-am-a-walrus" > ~/.pgpass
chmod 0600 ~/.pgpass

echo '\d' | psql --user=zframework --host=localhost phpunit

# Bring the latest composer into the environment
setup_composer;

# turn off the jit
echo hhvm.jit=0 >> /etc/hhvm/php.ini

# show the version of hhvm
hhvm --version

# swap to the source directory
cd /var/source

# setup the github token for use
composer config -g github-oauth.github.com $GITHUB_TOKEN

# make sure composer is configured correctly
composer validate --no-check-all --ansi

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
mkdir -p /opt/zynga
touch /opt/zynga/is_development

echo '<?php if (!extension_loaded("pgsql")) { echo "no-pgsql\n"; exit(255); } echo "pgsql-available\n"; exit(0);' | hhvm --php

make test

if [ $? -ne 0 ]; then
  echo "Make test failure"
  exit $?
fi

echo "BUILD: OK"
exit 0
