ALTER TABLE `components` DROP `component_requests`

CREATE TABLE IF NOT EXISTS `auth` (
  `auth_component` varchar(50) NOT NULL,
  `auth_action` varchar(50) NOT NULL,
  `auth_privilage` varchar(50) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;