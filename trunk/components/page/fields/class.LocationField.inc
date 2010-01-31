<?php
importField("Field");

class LocationField extends Field
{
    public function __construct($name, $displayName = "", $style=0)
    {
        parent::__construct($name, $displayName, $style|Field::HIDE_VIEW);
    }
    
    protected function toHTML_add()
    {
        $smarty = Controller::getInstance()->getSmarty();
        
        $html = "";
        $html .= smarty_function_admintree(array("javascript" => $this->getName(), "folderonly" => true), $smarty);
        $html .= "<input type='text' name='{$this->name}' id='{$this->name}' value='{$this->defaultValue}'/>";
        return $html;
    }
    protected function toHTML_edit()
    {
        $smarty = Controller::getInstance()->getSmarty();
        
        $html = "";
        $html .= smarty_function_admintree(array("javascript" => $this->getName(), "folderonly" => true), $smarty);
        $html .= "<input type='text' name='{$this->name}' id='{$this->name}' value='{$this->value}'/>";
        
        return $html;
    }
}