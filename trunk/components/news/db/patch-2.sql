ALTER TABLE `news` ADD `news_external` TINYINT NOT NULL DEFAULT '0';
ALTER TABLE `news` ADD `news_extlink` VARCHAR( 200 ) default NULL;