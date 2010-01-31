<?php
import('/core/class.InvalidFormException.inc');

importField("Field");

class IntField extends Field
{
    protected function toHTML_add()
    {
        return "<input type='text' name='{$this->getName()}' id='{$this->getName()}' value='{$this->getDefaultValue()}' size='5' onkeypress='return form_IntField_handleKey(event);'/>";
    }
    protected function toHTML_edit()
    {
        return "<input type='text' name='{$this->getName()}' id='{$this->getName()}' value='{$this->getValue()}' size='5' onkeypress='return form_IntField_handleKey(event);'/>";
    }
    public function getJavascript()
    {
        return <<<JS
            function form_IntField_handleKey(elem)
            {
                return (elem.charCode > 0 && !isNaN(parseInt(String.fromCharCode(elem.charCode)))) || elem.charCode == 0;
            }
JS;
    }
    
    public function validate($mode)
    {
        parent::validate($mode);
        
        if(($mode == Field::ADD || $mode == Field::EDIT) && (!is_numeric($this->getValue()) && (!$this->isIndex() && $mode == Field::EDIT)))
        {
            throw new FormException($this, "Field was not a number");
        }
    }
}