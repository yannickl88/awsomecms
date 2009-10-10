<?php
function smarty_function_form($params, &$smarty)
{
    $html = '';

    $config = Config::getInstance();
    $components = $config->getComponenets();

    $componentLoc = $components[$params['component']]->component_path;
    $formLoc = $componentLoc.'/forms/form.'.$params['form'].'.tpl';

    //call the action for the form
    $actionHandler = "action_".$params['form'];
    $componentObject = Component::init($params['component']);
    
    if($componentObject)
    {
        switch(strtoupper($_SERVER['REQUEST_METHOD']))
        {
            case 'POST' :
                $request = $_POST;
                $request['methode'] = 'post';
                break;
            default :
                $request = $_GET;
                $request['methode'] = 'get';
                break;
        }
        
        if(isset($request['component']))
        {
            unset($request['component']);
        }
        if(isset($request['action']))
        {
            unset($request['action']);
        }
        
        if(method_exists($componentObject, $actionHandler))
        {
            try
            {
                call_user_func(array($componentObject, $actionHandler), $smarty, null, $request);
            }
            catch(Exception $e) { }
        }
    }
    
    if(file_exists($formLoc))
    {
        $html = $smarty->fetch("file:".$formLoc);
    }

    return $html;
}