CREATE TABLE IF NOT EXISTS `privileges` (
  `privilege_name` varchar(100) NOT NULL,
  `privilege_int` bigint(20) NOT NULL,
  `privilege_locked` tinyint(4) NOT NULL default '0',
  PRIMARY KEY  (`privilege_name`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

INSERT INTO `privileges` (`privilege_name`, `privilege_int`, `privilege_locked`) VALUES
('admin', 1, 1);

CREATE TABLE IF NOT EXISTS `userdata` (
  `userdata_id` int(11) NOT NULL auto_increment,
  `user_id` int(11) NOT NULL,
  `userdata_key` varchar(50) NOT NULL,
  `userdata_value` text NOT NULL,
  PRIMARY KEY  (`userdata_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

CREATE TABLE IF NOT EXISTS `users` (
  `user_id` int(11) NOT NULL auto_increment,
  `user_name` varchar(100) NOT NULL,
  `user_pass` varchar(100) NOT NULL,
  `user_privileges` bigint(20) NOT NULL,
  PRIMARY KEY  (`user_id`),
  UNIQUE KEY `user_name` (`user_name`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=5 ;


INSERT INTO `users` (`user_id`, `user_name`, `user_pass`, `user_privileges`) VALUES
(4, 'admin', 'fe01ce2a7fbac8fafaed7c982a04e229', 1);
