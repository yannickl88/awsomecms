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
 * Show a date interval for the copyright year.
 * 
 * @param array $params
 * @param Smarty $smarty
 * @return string
 */
function smarty_function_copyrightdate($params, &$smarty)
{
    $year = (int) $params['year'];
    $cYear = (int) date("Y");
    
    if($year >= $cYear)
        return $year;
    else
        return "{$year} - {$cYear}";
}