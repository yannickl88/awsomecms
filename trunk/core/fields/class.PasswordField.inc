<?php
importField("Field");

class PasswordField extends Field
{
    public function __construct($name, $displayName = "", $style=0)
    {
        parent::__construct($name, $displayName, $style|Field::HIDE_VIEW);
    }
    
    protected function toHTML_add()
    {
        return "<input type='password' name='{$this->getName()}' id='{$this->getName()}' value='{$this->getDefaultValue()}'/>";
    }
    protected function toHTML_edit()
    {
        return "<input type='password' name='{$this->getName()}' id='{$this->getName()}' value='{$this->getDefaultValue()}'/>";
    }
}