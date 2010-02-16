CREATE TABLE IF NOT EXISTS `images` (
  `image_id` int(11) NOT NULL auto_increment,
  `image_title` varchar(100) NOT NULL,
  `image_url` varchar(255) NOT NULL,
  `image_urlthumb` varchar(255) NOT NULL,
  `image_width` int(11) NOT NULL,
  `image_height` int(11) NOT NULL,
  `image_thumbwidth` int(11) NOT NULL,
  `image_thumbheight` int(11) NOT NULL,
  `image_tag` varchar(200) NOT NULL,
  PRIMARY KEY  (`image_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1;

INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('gallery', 'action_add', 'admin');
INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('gallery', 'action_delete', 'admin');
INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('gallery', 'action_edit', 'admin');