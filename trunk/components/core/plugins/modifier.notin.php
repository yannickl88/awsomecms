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
* Modifier to check if value is in an array
* 
* @param mixed $element
* @param Array $array
* @return Boolean
*/
function smarty_modifier_notin($element, $array)
{
    return !in_array($element, $array);
}
