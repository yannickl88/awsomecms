CREATE TABLE IF NOT EXISTS `components` (
  `component_name` varchar(50) NOT NULL,
  `component_requests` text NOT NULL,
  `component_path` varchar(255) NOT NULL,
  `component_auth` int(11) NOT NULL,
  `component_patchlevel` int(11) NOT NULL,
  PRIMARY KEY  (`component_name`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
