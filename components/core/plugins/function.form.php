<?php
/**
* Form tag, this is used to add a form from a component to a page. This will call the action and hooks related to the action of the form
* 
* @param array $params
* @param Smarty $smarty
*/
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
    $prehook = Config::getInstance()->getHook($params['component'], $params['form'], "pre");
    $posthook = Config::getInstance()->getHook($params['component'], $params['form'], "post");
    
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
            //pre action
            if($prehook)
            {
                try
                {
                    Debugger::getInstance()->log("Calling PRE hook {$prehook[1]} on {$prehook[0]}");
                    
                    $prehookComponent = Component::init($prehook[0]);
                    $prehookAction = "hook_".$prehook[1];
                    
                    if(method_exists($prehookComponent, $prehookAction))
                    {
                        $request = call_user_func(array($prehookComponent, $prehookAction), $smarty, $request);
                    }
                }
                catch(Exception $e) { }
            }
            
            try
            {
                call_user_func(array($componentObject, $actionHandler), $smarty, null, $request);
            }
            catch(Exception $e) { }
            
            //post action
            if($posthook)
            {
                try
                {
                    Debugger::getInstance()->log("Calling POST hook {$posthook[1]} on {$posthook[0]}");
                    
                    $posthookComponent = Component::init($posthook[0]);
                    $posthookAction = "hook_".$posthook[1];
                    
                    if(method_exists($posthookComponent, $posthookAction))
                    {
                        $request = call_user_func(array($posthookComponent, $posthookAction), $smarty, $request);
                    }
                }
                catch(Exception $e) { }
            }
        }
    }
    
    if(file_exists($formLoc))
    {
        $html = $smarty->fetch("file:".$formLoc);
    }

    return $html;
}