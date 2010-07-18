CREATE TABLE IF NOT EXISTS `components` (
  `component_name` varchar(50) NOT NULL,
  `component_path` varchar(255) NOT NULL,
  `component_auth` int(11) NOT NULL,
  `component_patchlevel` int(11) NOT NULL,
  `component_version` varchar(50) NOT NULL,
  `component_updateurl` varchar(200) NOT NULL,
  PRIMARY KEY  (`component_name`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE IF NOT EXISTS `hooks` (
  `hook_component` varchar(100) NOT NULL,
  `hook_target` varchar(100) NOT NULL,
  `hook_prepost` varchar(4) NOT NULL,
  `hook_action` varchar(100) NOT NULL,
  `hook_function` varchar(100) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

CREATE TABLE IF NOT EXISTS `auth` (
  `auth_component` varchar(50) NOT NULL,
  `auth_action` varchar(50) NOT NULL,
  `auth_privilage` varchar(50) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

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


INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('core', 'action_auth', 'admin');
INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('core', 'action_components', 'admin');
INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('core', 'action_install', 'admin');
INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('core', 'action_layout', 'admin');
INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('core', 'action_update', 'admin');