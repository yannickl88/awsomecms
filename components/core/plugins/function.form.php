<?php
import("/core/class.Table.inc");

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
    $component;
    $table;
    
    if(isset($params['component']))
    {
        $component = $params['component'];
    }
    else
    {
        $tableID = explode(".", $params['table']);
        $component = $tableID[0];
        $table = $tableID[1];
    }
    
    //call the action for the form
    $actionHandler = "action_".$params['form'];
    $componentObject = Component::init($component);
    $prehook = Config::getInstance()->getHook($tableID[0], $params['form'], "pre");
    $posthook = Config::getInstance()->getHook($tableID[0], $params['form'], "post");
    
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
    
    $render = $params['form'];
    
    if(isset($params['component']))
    {
        $loc = $components[$component]->component_path."/forms/form.".$render.".tpl";
        return $smarty->fetch("file:".$loc);
    }
    else
    {
        $tableObject = Table::init($params['table']);
        
        if(isset($params['render']))
        {
            $render = $params['render'];
        }
        
        switch(strtolower($render))
        {
            case "add":
                return $tableObject->toHTML(Field::ADD);
                break;
            case "edit":
                $tableObject->setRecord($smarty->get_template_vars("record"));
                return $tableObject->toHTML(Field::EDIT);
                break;
            case "delete":
                return $tableObject->toHTML(Field::DELETE);
                break;
            case "view":
            default:
                return $tableObject->toHTML(Field::VIEW);
                break;
        }
    }
}