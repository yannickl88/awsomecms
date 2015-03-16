<?php
/**
 * Show the admin menu
 *
 * @param array $params
 * @param Smarty $smarty
 * @return string
 */
function smarty_function_adminmenu($params, &$smarty)
{
    $components = RegisterManager::getInstance()->getComponents();
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