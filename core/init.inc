<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package core
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */

/**
 * Initalisation file, include this in order to use the CMS
 */
require_once realpath(dirname(__FILE__).'/functions.util.inc');

import('/core/class.Debugger.inc');

error_reporting(E_ALL);
set_error_handler("handleError");

import('/libs/smarty/Smarty.class.php');
import('/core/class.SQL.inc');
import('/core/class.Config.inc');
import('/core/class.SmartyPageLoader.inc');
import('/core/class.Controller.inc');
import('/core/class.Authentication.inc');
import('/core/class.Table.inc');
import('/core/class.Language.inc');
