ALTER TABLE `news` ADD `news_extsite` VARCHAR( 50 ) NOT NULL AFTER `news_external` ;
ALTER TABLE `news` ADD `news_titleurl` VARCHAR( 250 ) NOT NULL AFTER `news_title` ;