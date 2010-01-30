ALTER TABLE `users` ADD `user_privileges` BIGINT NOT NULL ;

CREATE TABLE IF NOT EXISTS `privileges` (
  `privilege_name` varchar(100) NOT NULL,
  `privilege_int` bigint(20) NOT NULL,
  PRIMARY KEY  (`privilege_name`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
INSERT INTO `privileges` (`privilege_name`, `privilege_int`) VALUES
('admin', 1);