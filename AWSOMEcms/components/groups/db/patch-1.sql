ALTER TABLE `groups` ADD `group_locked` TINYINT NOT NULL ;

UPDATE `awsomecms`.`groups` SET `group_locked` = '1' WHERE `groups`.`group_id` = 1;
INSERT INTO `groups` (`group_id`, `group_name`, `group_privileges`) VALUES
(2, 'users', 0, 1);
INSERT INTO `usersgroups` (`group_id`, `user_id`) VALUES
(2, 1);