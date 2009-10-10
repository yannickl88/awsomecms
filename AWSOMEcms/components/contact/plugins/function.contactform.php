<?php
function smarty_function_contactform($params, &$smarty)
{
    $html = '';

    $components = Config::getInstance()->getComponenets();
    $contactComponent = Component::init('contact');
    
    $form = $contactComponent->select(array("form_name" => $params['form']));
    
    $smarty->assign("contactform", $form[0]);
    
    return $smarty->fetch("file:".$components['contact']->component_path.'/forms/form.contact.tpl');
}