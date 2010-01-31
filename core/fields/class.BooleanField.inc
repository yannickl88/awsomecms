<?php
importField("Field");

class BooleanField extends Field
{
    protected function toHTML_add()
    {
        $html = "";
        $html .= "<input type='radio' name='{$this->name}' id='{$this->name}' value='0' checked=\"checked\"/> On";
        $html .= "Off <input type='radio' name='{$this->name}' id='{$this->name}' value='0'/>";
        
        return $html;
    }
    protected function toHTML_edit()
    {
        $html = "";
        
        $html .= "<input type='radio' name='{$this->name}' id='{$this->name}' value='0'";
        if($this->value)
        {
            $html .= " checked=\"checked\"";
        }
        $html .="/> On";
        $html .= "Off <input type='radio' name='{$this->name}' id='{$this->name}' value='0'";
        if(!$this->value)
        {
            $html .= " checked=\"checked\"";
        }
        $html .= "/>";
        
        return $html;
    }
    protected function toHTML_view()
    {
        return ($this->value)? "Yes" : "No";
    }
}