<?php
function smarty_function_adminmenu($params, &$smarty)
{
    include_once Config::get('websiteroot', '.').'/../components/page/util/class.Menu.inc';

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