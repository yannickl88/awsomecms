CREATE TABLE IF NOT EXISTS `privileges` (
  `privilege_id` int(11) NOT NULL auto_increment,
  `privilege_name` varchar(100) NOT NULL,
  `privilege_int` bigint(20) NOT NULL,
  `privilege_locked` tinyint(4) NOT NULL default '0',
  PRIMARY KEY  (`privilege_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=2 ;

INSERT INTO `privileges` (`privilege_name`, `privilege_int`, `privilege_locked`) VALUES
('admin', 1, 1);

CREATE TABLE IF NOT EXISTS `userdata` (
  `userdata_id` int(11) NOT NULL auto_increment,
  `user_id` int(11) NOT NULL,
  `userdata_key` varchar(50) NOT NULL,
  `userdata_value` text NOT NULL,
  PRIMARY KEY  (`userdata_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 ;

CREATE TABLE IF NOT EXISTS `users` (
  `user_id` int(11) NOT NULL auto_increment,
  `user_name` varchar(100) NOT NULL,
  `user_pass` varchar(150) NOT NULL,
  `user_privileges` bigint(20) NOT NULL,
  PRIMARY KEY  (`user_id`),
  UNIQUE KEY `user_name` (`user_name`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 ;


INSERT INTO `users` (`user_name`, `user_pass`, `user_privileges`) VALUES
('admin', 'a:2:{s:8:"password";s:40:"f88430b2bd2ee105810d924f3ea47a39d09b3c75";s:4:"salt";s:9:"UDRaaEuSY";}', 1);

INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('users', 'action_add', 'admin');
INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('users', 'action_delete', 'admin');
INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('users', 'action_edit', 'admin');
INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('users', 'action_privadd', 'admin');
INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('users', 'action_privdelete', 'admin');
INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('users', 'action_privedit', 'admin');