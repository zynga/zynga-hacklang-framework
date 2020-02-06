#!/bin/bash

##
# Recreate the testing schema.
##
cat /var/source/tests/sql/mysql/create_test_database.sql | mysql phpunit

mysql -e 'SHOW DATABASES' && \
mysql -e 'SHOW TABLES' phpunit
