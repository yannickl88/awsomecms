CREATE TABLE IF NOT EXISTS `news` (
  `news_id` int(11) NOT NULL auto_increment,
  `news_title` varchar(200) NOT NULL,
  `news_user` int(11) NOT NULL,
  `news_text` text character set utf8 NOT NULL,
  `news_tag` varchar(50) NOT NULL,
  `news_date` date NOT NULL,
  `news_external` tinyint(4) NOT NULL default '0',
  `news_extsite` varchar(50) NOT NULL,
  `news_extlink` varchar(200) default NULL,
  PRIMARY KEY  (`news_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 ;

INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('news', 'action_add', 'admin');
INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('news', 'action_delete', 'admin');
INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('news', 'action_edit', 'admin');