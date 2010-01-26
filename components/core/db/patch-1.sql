ALTER TABLE `components` ADD `component_patchlevel` INT NOT NULL ;
ALTER TABLE `components` ADD `component_requests` INT NOT NULL AFTER `component_name`;