
CREATE TABLE IF NOT EXISTS `pages` (
  `page_id` int(11) NOT NULL auto_increment,
  `page_name` varchar(100) NOT NULL,
  `page_location` varchar(255) NOT NULL,
  `page_template` text character set utf8 NOT NULL,
  `page_timestamp` datetime NOT NULL,
  `page_html` tinyint(4) NOT NULL DEFAULT '1',
  PRIMARY KEY  (`page_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1;

INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('page', 'action_add', 'admin');
INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('page', 'action_delete', 'admin');
INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('page', 'action_edit', 'admin');
