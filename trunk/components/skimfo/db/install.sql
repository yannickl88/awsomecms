CREATE TABLE IF NOT EXISTS `skimspots` (
  `spot_id` int(11) NOT NULL auto_increment,
  `spot_header` varchar(100) NOT NULL,
  `spot_title` varchar(200) NOT NULL,
  `spot_text` text NOT NULL,
  `spot_img` varchar(100) NOT NULL,
  `spot_location` int(11) NOT NULL,
  PRIMARY KEY  (`spot_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

CREATE TABLE IF NOT EXISTS `locations` (
  `location_id` int(11) NOT NULL auto_increment,
  `location_name` varchar(100) NOT NULL,
  PRIMARY KEY  (`location_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;