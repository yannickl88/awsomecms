CREATE TABLE IF NOT EXISTS `s_groups` (
  `group_id` int(11) NOT NULL auto_increment,
  `group_title` varchar(200) NOT NULL,
  `group_header` int(11) NOT NULL,
  PRIMARY KEY  (`group_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 ;

CREATE TABLE IF NOT EXISTS `s_groups_images` (
  `group_id` int(11) NOT NULL,
  `image_id` int(11) NOT NULL,
  PRIMARY KEY  (`group_id`,`image_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE IF NOT EXISTS `s_images` (
  `image_id` int(11) NOT NULL auto_increment,
  `image_title` varchar(100) NOT NULL,
  `image_url` varchar(250) NOT NULL,
  `image_width` int(11) NOT NULL,
  `image_height` int(11) NOT NULL,
  `image_size` tinyint(4) NOT NULL default '0',
  PRIMARY KEY  (`image_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 ;

INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('slideshow', 'action_add_group', 'admin');
INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('slideshow', 'action_add_image', 'admin');
INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('slideshow', 'action_admin_group', 'admin');
INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('slideshow', 'action_delete_group', 'admin');
INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('slideshow', 'action_delete_image', 'admin');
INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('slideshow', 'action_edit_group', 'admin');
INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('slideshow', 'action_edit_image', 'admin');