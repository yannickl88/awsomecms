<?php
importField("Field");

class DateField extends Field
{
    protected function toHTML_add()
    {
        return "<input type='text' name='{$this->getName()}' value='{$this->getDefaultValue()}'/>";
    }
    protected function toHTML_edit()
    {
        return "<input type='text' name='{$this->getName()}' value='{$this->getDefaultValue()}'/>";
    }
    protected function toHTML_view()
    {
        return date("c", strtotime($this->value));
    }
}