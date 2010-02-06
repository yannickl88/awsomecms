<?php
importField("Field");

class DateField extends Field
{
    public function value2db($value)
    {
        return date("c", strtotime($value));
    }
    public function getValue($data = null)
    {
        return date("m/d/Y", strtotime(parent::getValue($data)));
    }
    public function getDefaultValue()
    {
        return date("m/d/Y", time());
    }
    
    protected function toHTML_add()
    {
        return "<input type='text' class='dateField' name='{$this->getName()}' id='{$this->getName()}' value='{$this->getDefaultValue()}'/>";
    }
    protected function toHTML_edit()
    {
        return "<input type='text' class='dateField' name='{$this->getName()}' id='{$this->getName()}' value='{$this->getValue()}'/>";
    }
    protected function toHTML_view()
    {
        return date("c", strtotime($this->value));
    }
    public function getJavascript()
    {
        return <<<JS
            $(function()
            {
                $(".dateField").datepicker({
                    nextText: ">>",
                    prevText: "<<",
                    showAnim: "blind",
                    currentText: "Today"
                });
            });
JS;
    }
}