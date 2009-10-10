CREATE TABLE IF NOT EXISTS `users` (
  `user_id` int(11) NOT NULL auto_increment,
  `user_name` varchar(100) NOT NULL,
  `user_pass` varchar(100) NOT NULL,
  `user_privileges` bigint(20) NOT NULL,
  PRIMARY KEY  (`user_id`),
  UNIQUE KEY `user_name` (`user_name`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=4 ;


CREATE TABLE IF NOT EXISTS `privileges` (
  `privilege_name` varchar(100) NOT NULL,
  `privilege_int` bigint(20) NOT NULL,
  PRIMARY KEY  (`privilege_name`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

INSERT INTO `users` (`user_name`, `user_pass`, `user_privileges`) VALUES
('admin', 'fe01ce2a7fbac8fafaed7c982a04e229', 1);
INSERT INTO `privileges` (`privilege_name`, `privilege_int`) VALUES
('admin', 1);