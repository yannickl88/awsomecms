CREATE TABLE IF NOT EXISTS `files` (
  `file_id` int(11) NOT NULL auto_increment,
  `file_name` varchar(200) NOT NULL,
  `file_data` text NOT NULL,
  `file_date` date NOT NULL,
  `file_tag` varchar(255) NOT NULL,
  `file_description` text NOT NULL,
  PRIMARY KEY  (`file_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1;

INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('file', 'action_add', 'admin');
INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('file', 'action_delete', 'admin');
INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('file', 'action_edit', 'admin');
INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('file', 'action_upload', 'admin');