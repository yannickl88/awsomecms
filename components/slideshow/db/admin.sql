INSERT INTO `pages` (`page_name`, `page_location`, `page_template`, `page_timestamp`) VALUES('admin', '/admin/slideshow/group/', '{form table=''slideshow.groups'' form=''admin''}', NOW());
INSERT INTO `pages` (`page_name`, `page_location`, `page_template`, `page_timestamp`) VALUES('admin', '/admin/slideshow/image/', '{form table=''slideshow.images'' form=''admin''}', NOW());
INSERT INTO `pages` (`page_name`, `page_location`, `page_template`, `page_timestamp`) VALUES('add', '/admin/slideshow/image/', '{form component=''slideshow'' form=''add_image''}', NOW());
INSERT INTO `pages` (`page_name`, `page_location`, `page_template`, `page_timestamp`) VALUES('add', '/admin/slideshow/group/', '{form component=''slideshow'' form=''add_group''}', NOW());
INSERT INTO `pages` (`page_name`, `page_location`, `page_template`, `page_timestamp`) VALUES('edit', '/admin/slideshow/group/', '{form component=''slideshow'' form=''edit_group''}', NOW());
INSERT INTO `pages` (`page_name`, `page_location`, `page_template`, `page_timestamp`) VALUES('edit', '/admin/slideshow/image/', '{form component=''slideshow'' form=''edit_image''}', NOW());
INSERT INTO `pages` (`page_name`, `page_location`, `page_template`, `page_timestamp`) VALUES('delete', '/admin/slideshow/group/', '{form table=''slideshow.groups'' form=''delete''}', NOW());
INSERT INTO `pages` (`page_name`, `page_location`, `page_template`, `page_timestamp`) VALUES('delete', '/admin/slideshow/image/', '{form table=''slideshow.images'' form=''delete''}', NOW());