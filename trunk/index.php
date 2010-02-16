<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */
$url = "http://www.".$_SERVER['SERVER_NAME'].'/'.$_SERVER['REQUEST_URI'];

header("HTTP/1.1 301 Moved Permanently");
header("Location: {$url}");
exit();
