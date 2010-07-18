CREATE TABLE IF NOT EXISTS `search_items` (
  `item_id` int(11) NOT NULL AUTO_INCREMENT,
  `item_link` varchar(200) NOT NULL,
  `item_title` varchar(100) NOT NULL,
  `item_checksum` varchar(50) NOT NULL,
  `item_hook` varchar(20) NOT NULL,
  PRIMARY KEY (`item_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1;

CREATE TABLE IF NOT EXISTS `search_refs` (
  `ref_word` int(11) NOT NULL,
  `ref_item` int(11) NOT NULL,
  `ref_count` int(11) NOT NULL,
  `ref_text` varchar(100) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE IF NOT EXISTS `search_words` (
  `word_id` int(11) NOT NULL,
  `word_word` varchar(100) NOT NULL,
  KEY `word_id` (`word_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
