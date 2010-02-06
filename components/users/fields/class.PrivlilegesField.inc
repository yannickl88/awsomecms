<?php
importField("Field");

class PrivlilegesField extends Field
{
    protected function toHTML_add()
    {
        $privs = Table::init("users.privileges")
            ->doSelect()
            ->getRows();
        
        $html = "";
        foreach($privs as $priv)
        {
            $html .= "<input type=\"checkbox\" name=\"{$this->name}[]\" value=\"{$priv->privilege_int}\" checked=\"checked\"/>{$priv->privilege_name}<br />";
        }
        return $html;
    }
    protected function toHTML_edit()
    {
        $privs = Table::init("users.privileges")
            ->doSelect()
            ->getRows();
        
        $html = "";
        foreach($privs as $priv)
        {
            $checked = "";
            
            if(Config::hasFlag((int) $this->getValue(), (int) $priv->privilege_int))
            {
                $checked = "checked=\"checked\"";
            }
            $html .= "<input type=\"checkbox\" name=\"{$this->name}[]\" value=\"{$priv->privilege_int}\" {$checked}/>{$priv->privilege_name}<br />";
        }
        return $html;
    }
    
    public function value2db($value)
    {
        if($value)
        {
            return array_sum($value);
        }
        else
        {
            return 0;
        }
    }
}