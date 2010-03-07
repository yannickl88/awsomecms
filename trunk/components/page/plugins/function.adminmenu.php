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
 * Show the admin menu
 * 
 * @param array $params
 * @param Smarty $smarty
 * @return string
 */
function smarty_function_adminmenu($params, &$smarty)
{
    import('components/page/util/class.Menu.inc');

    $components = Config::getInstance()->getComponenets();
    $menu = Menu::getInstance();

    foreach($components as $componentArray)
    {
        $component = Component::init($componentArray->component_name);

        if($component)
        {
            $component->registerMenuItems($menu);
        }
    }
    return $menu->toHTML();
}