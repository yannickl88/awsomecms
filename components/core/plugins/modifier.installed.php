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
* Modifier to check if a given component is installed
* 
* @param string $element
* @return Boolean
*/
function smarty_modifier_installed($element)
{
    return RegisterManager::getInstance()->hasComponent($element);
}
