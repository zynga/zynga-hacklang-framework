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

CREATE TABLE IF NOT EXISTS phpunit.user_inventory (
  id int(11) NOT NULL AUTO_INCREMENT,
  user_id int(11) NOT NULL DEFAULT 0,
  item_type_id int(11) NOT NULL DEFAULT 0,
  PRIMARY KEY(id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE INDEX phpunit_user_inventory_user_id_idx ON phpunit.user_inventory (user_id);
CREATE INDEX phpunit_user_inventory_item_type_id_idx ON phpunit.user_inventory (item_type_id);

-- 19690720: Apollo 11 is our user id for full set testing
-- 19700411: Apollo 13 is our user id for partial set testing
INSERT INTO phpunit.user_inventory (id, user_id, item_type_id) VALUES (30, 19690720, 12387451);
INSERT INTO phpunit.user_inventory (id, user_id, item_type_id) VALUES (31, 19690720, 12387452);
INSERT INTO phpunit.user_inventory (id, user_id, item_type_id) VALUES (32, 19690720, 12387453);
INSERT INTO phpunit.user_inventory (id, user_id, item_type_id) VALUES (33, 19690720, 12387454);
INSERT INTO phpunit.user_inventory (id, user_id, item_type_id) VALUES (34, 19690720, 12387455);

INSERT INTO phpunit.user_inventory (id, user_id, item_type_id) VALUES (35, 19700411, 12387451);
INSERT INTO phpunit.user_inventory (id, user_id, item_type_id) VALUES (36, 19700411, 12387453);
INSERT INTO phpunit.user_inventory (id, user_id, item_type_id) VALUES (37, 19700411, 12387455);

GRANT ALL PRIVILEGES ON *.* TO 'zframework'@'localhost' IDENTIFIED BY 'i-am-a-walrus';
