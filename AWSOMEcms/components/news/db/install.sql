CREATE TABLE IF NOT EXISTS `news` (
  `news_id` int(11) NOT NULL auto_increment,
  `news_title` varchar(200) NOT NULL,
  `news_user` int(11) NOT NULL,
  `news_text` text NOT NULL,
  `news_tag` varchar(50) NOT NULL,
  `news_date` date NOT NULL,
  PRIMARY KEY  (`news_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1;