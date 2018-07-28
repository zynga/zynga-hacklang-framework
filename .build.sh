#!/bin/bash

set -ex

/etc/init.d/memcached start
/etc/init.d/mysql start
/etc/init.d/postgresql start

# dump out the schema as it stands
mysql -e 'SHOW DATABASES'
mysql -e 'SHOW TABLES' phpunit

# dump out the schema as it stands
echo '\d' | psql --user=zframework --host=localhost phpunit

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
