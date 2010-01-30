<?php
require_once realpath(dirname(__FILE__).'/functions.util.inc');

import('/core/class.Debugger.inc');

set_error_handler("handleError");

import('/libs/Smarty.class.php');
import('/core/class.Config.inc');
import('/core/class.SmartyPageLoader.inc');
import('/core/class.Controller.inc');

