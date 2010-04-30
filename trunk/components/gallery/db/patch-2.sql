DROP TABLE `images`

CREATE TABLE IF NOT EXISTS `gallery` (
  `gallery_id` int(11) NOT NULL auto_increment,
  `gallery_name` varchar(100) NOT NULL,
  `gallery_images` text NOT NULL,
  PRIMARY KEY  (`gallery_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
