CREATE DATABASE IF NOT EXISTS `phpunit`;

DROP TABLE IF EXISTS phpunit.phpunit;
DROP TABLE IF EXISTS phpunit.item_type;

CREATE TABLE IF NOT EXISTS `phpunit`.`phpunit` (
  `unit_test_stamp` int(11) NOT NULL,
  `int_data` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS phpunit.item_type (
  id int(11) NOT NULL AUTO_INCREMENT,
  name VARCHAR(255) NOT NULL,
  PRIMARY KEY(id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO phpunit.item_type (id, name) VALUES ( 12387451, 'this-is-a-test-valueset-1');
INSERT INTO phpunit.item_type (id, name) VALUES ( 12387452, 'this-is-a-test-valueset-2');
INSERT INTO phpunit.item_type (id, name) VALUES ( 12387453, 'this-is-a-test-valueset-3');
INSERT INTO phpunit.item_type (id, name) VALUES ( 12387454, 'this-is-a-test-valueset-4');
INSERT INTO phpunit.item_type (id, name) VALUES ( 12387455, 'this-is-a-test-valueset-5');

GRANT ALL PRIVILEGES ON *.* TO 'zframework'@'localhost' IDENTIFIED BY 'i-am-a-walrus';
