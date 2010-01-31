<?php
importField("Field");

class TextField extends Field
{
    public function __construct($name, $displayName = "", $style=0)
    {
        parent::__construct($name, $displayName, $style|Field::HIDE_VIEW);
    }
    
    protected function toHTML_add()
    {
        return "<textarea name='{$this->getName()}' id='{$this->getName()}' class=\"htmleditor\" rows=\"20\" cols=\"20\">{$this->getDefaultValue()}</textarea>";
    }
    protected function toHTML_edit()
    {
        return "<textarea name='{$this->getName()}' id='{$this->getName()}' class=\"htmleditor\" rows=\"20\" cols=\"20\">{$this->getValue()}</textarea>";
    }
}