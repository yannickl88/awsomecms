CREATE TABLE IF NOT EXISTS `forms` (
  `form_id` int(11) NOT NULL auto_increment,
  `form_name` varchar(100) NOT NULL,
  `form_to` varchar(100) NOT NULL,
  `form_bcc` varchar(100) NOT NULL,
  `form_subject` varchar(100) NOT NULL,
  `form_thanks` varchar(100) NOT NULL,
  PRIMARY KEY  (`form_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1;

CREATE TABLE IF NOT EXISTS `form_fields` (
  `field_id` int(11) NOT NULL auto_increment,
  `field_form` int(11) NOT NULL,
  `field_name` varchar(100) NOT NULL,
  `field_code` varchar(100) NOT NULL,
  `field_type` tinyint(4) NOT NULL,
  `field_required` tinyint(4) NOT NULL,
  PRIMARY KEY  (`field_id`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1;

INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('contact', 'action_add', 'admin');
INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('contact', 'action_delete', 'admin');
INSERT INTO `auth` (`auth_component`, `auth_action`, `auth_privilage`) VALUES('contact', 'action_edit', 'admin');
