CREATE TABLE IF NOT EXISTS `hooks` (
  `hook_component` varchar(100) NOT NULL,
  `hook_target` varchar(100) NOT NULL,
  `hook_prepost` varchar(4) NOT NULL,
  `hook_action` varchar(100) NOT NULL,
  `hook_function` varchar(100) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;