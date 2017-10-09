CREATE DATABASE IF NOT EXISTS `phpunit`;

CREATE TABLE IF NOT EXISTS `phpunit`.`phpunit` (
  `unit_test_stamp` int(11) NOT NULL,
  `int_data` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

GRANT ALL PRIVILEGES ON *.* TO 'zframework'@'localhost' IDENTIFIED BY 'i-am-a-walrus';
