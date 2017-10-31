CREATE DATABASE phpunit;

CREATE ROLE zframework WITH LOGIN PASSWORD 'i-am-a-walrus';

GRANT ALL ON DATABASE phpunit TO zframework;

\c phpunit

-- Note: don't emulate dual, it won't work the way you think it will.
--CREATE TABLE dual();

CREATE TABLE phpunit (
  unit_test_stamp NUMERIC(20),
  int_data NUMERIC(20)
);
