<?php
/**
 * AuthsField list of check boxes
 *
 * @author Yannick <yannick.l.88@gmail.com>
 */
class AuthsField extends Field
{
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_add()
     */
    protected function toHTML_add()
    {
        $groups = RegisterManager::getInstance()->getAuths();

        $html = "";
        foreach($groups as $group => $auths)
        {
            $html .= "<input type=\"checkbox\" name=\"{$this->name}[]\" value=\"{$group}\" />{$group}<br />";
        }
        return $html;
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_edit()
     */
    protected function toHTML_edit()
    {
        $groups = RegisterManager::getInstance()->getAuths();

        $html = "";
        foreach($groups as $group => $auths)
        {
            if(in_array($group, $this->getValue()))
            {
                $html .= "<input type=\"checkbox\" name=\"{$this->name}[]\" value=\"{$group}\" checked=\"checked\"/>{$group}<br />";
            }
            else
            {
                $html .= "<input type=\"checkbox\" name=\"{$this->name}[]\" value=\"{$group}\" />{$group}<br />";
            }
        }
        return $html;
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field::value2db()
     */
    public function value2db($value)
    {
        if($value == null) return "";
        return implode("|", $value);
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field::db2value()
     */
    public function db2value($value)
    {
        return explode("|", $value);
    }
}