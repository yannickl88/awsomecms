INSERT INTO `pages` (`page_name`, `page_location`, `page_template`, `page_timestamp`) VALUES('index', '/', '<h1>Welcome</h1>\r\n<p>Installation was a success</p>\r\n<p>{news max=5}</p>', NOW());
INSERT INTO `pages` (`page_name`, `page_location`, `page_template`, `page_timestamp`) VALUES('login', '/', '{form component=''users'' form=''admin_login''}', NOW());

