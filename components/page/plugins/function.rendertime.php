<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.page.plugins
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */

/**
 * Show the time it has taken for the site to render
 * 
 * @param array $params
 * @param Smarty $smarty
 * @return int
 */
function smarty_function_rendertime($params, &$smarty)
{
    global $start;
    
    return round(microtime(true) - $start, 3);
}