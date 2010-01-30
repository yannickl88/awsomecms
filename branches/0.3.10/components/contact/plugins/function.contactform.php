<?php
/**
* Contact for tag, this addes a form to a page
* 
* params: form
* 
* @param array $params
* @param Smarty $smarty
*/
function smarty_function_contactform($params, &$smarty)
{
    $html = '';

    $components = Config::getInstance()->getComponenets();
    $contactComponent = Component::init('contact');
    
    $form = $contactComponent->select(array("form_name" => $params['form']));
    
    $smarty->assign("contactform", $form[0]);
    
    return $smarty->fetch("file:".$components['contact']->component_path.'/forms/form.contact.tpl');
}