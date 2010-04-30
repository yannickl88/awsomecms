CREATE TABLE IF NOT EXISTS `gallery` (
  `gallery_id` int(11) NOT NULL auto_increment,
  `gallery_name` varchar(100) NOT NULL,
  `gallery_images` text NOT NULL,
  PRIMARY KEY  (`gallery_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('gallery', 'action_add', 'admin');
INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('gallery', 'action_delete', 'admin');
INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('gallery', 'action_edit', 'admin');