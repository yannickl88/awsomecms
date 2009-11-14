CREATE TABLE IF NOT EXISTS `components` (
  `component_name` varchar(50) NOT NULL,
  `component_requests` text NOT NULL,
  `component_path` varchar(255) NOT NULL,
  `component_auth` int(11) NOT NULL,
  `component_patchlevel` int(11) NOT NULL,
  PRIMARY KEY  (`component_name`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;


CREATE TABLE IF NOT EXISTS `hooks` (
  `hook_component` varchar(100) NOT NULL,
  `hook_target` varchar(100) NOT NULL,
  `hook_prepost` varchar(4) NOT NULL,
  `hook_action` varchar(100) NOT NULL,
  `hook_function` varchar(100) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;