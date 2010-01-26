CREATE TABLE IF NOT EXISTS `comments` (
  `comment_id` int(11) NOT NULL auto_increment,
  `comment_text` text NOT NULL,
  `comment_username` varchar(100) NOT NULL,
  `comment_hook` varchar(255) NOT NULL,
  `comment_time` datetime NOT NULL,
  PRIMARY KEY  (`comment_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 ;


