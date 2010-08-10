<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.core.plugins
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */

/**
 * Make the first letter of a string uppercase
 *
 * @param string $string
 * @return string
 */
function smarty_modifier_explode($string, $delimiter = ",")
{
    $arr = explode($delimiter, $string);
    array_walk($arr, create_function('&$a', '$a = trim($a);'));
    return $arr;
}
