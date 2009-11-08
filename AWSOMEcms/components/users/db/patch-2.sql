CREATE TABLE IF NOT EXISTS `userdata` (
  `userdata_id` int(11) NOT NULL auto_increment,
  `user_id` int(11) NOT NULL,
  `userdata_key` varchar(50) NOT NULL,
  `userdata_value` text NOT NULL,
  PRIMARY KEY  (`userdata_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

ALTER TABLE `privileges` ADD `privilege_locked` TINYINT NOT NULL DEFAULT '0';
UPDATE `awsomecms`.`privileges` SET `privilege_locked` = '1' WHERE CONVERT( `privileges`.`privilege_name` USING utf8 ) = 'admin' LIMIT 1 ;