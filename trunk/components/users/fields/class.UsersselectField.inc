<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.users.fields
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */
importField("Field");

/**
 * Selection list of all the users, do note you may want to use this as a dummy
 * field and so something with it yourself. There is currently no relation 
 * option or something.
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class UsersselectField extends Field
{
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_add()
     */
    protected function toHTML_add()
    {
        $users = Table::init("users.users")
            ->doSelect()
            ->getRows();
        
        $html = "";
        $html .= "<div style=\"float: left; width: 150px;\">";
        $html .= "    Selected Users<br />";
        $html .= "    <select id=\"{$this->getName()}_activeusers\" multiple=\"multiple\" style=\"width: 140px; height: 120px;\">";
        $html .= "    </select>";
        $html .= "</div>";
        $html .= "<div style=\"width: 50px; float: left; text-align: center; padding-top: 30px;\">";
        $html .= "    <button type=\"button\" onclick=\"form_UsersselectField_moveItem('{$this->getName()}_nonusers', '{$this->getName()}_activeusers');\">&#171;</button>";
        $html .= "    <button type=\"button\" onclick=\"form_UsersselectField_moveItem('{$this->getName()}_activeusers', '{$this->getName()}_nonusers');\">&#187;</button>";
        $html .= "</div>";
        $html .= "<div style=\"margin-left: 200px; width: 150px;\">";
        $html .= "    Avalible Users<br />";
        $html .= "    <select id=\"{$this->getName()}_nonusers\" multiple=\"multiple\" style=\"width: 140px; height: 120px;\">";
        foreach($users as $user)
        {
            $html .= "        <option value=\"{$user->user_id}\">{$user->user_name}</option>";
        }
        $html .= "    </select>";
        $html .= "</div>";
        $html .= "<div id='{$this->getName()}_values'></div>";
        return $html;
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_edit()
     */
    protected function toHTML_edit()
    {
        $users = Table::init("users.users")
            ->doSelect()
            ->getRows();
        
        $html = "";
        $html .= "<div style=\"float: left; width: 150px;\">";
        $html .= "    Selected Users<br />";
        $html .= "    <select id=\"{$this->getName()}_activeusers\" multiple=\"multiple\" style=\"width: 140px; height: 120px;\">";
        foreach($users as $user)
        {
            if(isset($this->requestValue[$user->user_id]))
            {
                $html .= "        <option value=\"{$user->user_id}\">{$user->user_name}</option>";
            }
        }
        $html .= "    </select>";
        $html .= "</div>";
        $html .= "<div style=\"width: 50px; float: left; text-align: center; padding-top: 30px;\">";
        $html .= "    <button type=\"button\" onclick=\"form_UsersselectField_moveItem('{$this->getName()}_nonusers', '{$this->getName()}_activeusers');\">&#171;</button>";
        $html .= "    <button type=\"button\" onclick=\"form_UsersselectField_moveItem('{$this->getName()}_activeusers', '{$this->getName()}_nonusers');\">&#187;</button>";
        $html .= "</div>";
        $html .= "<div style=\"margin-left: 200px; width: 150px;\">";
        $html .= "    Avalible Users<br />";
        $html .= "    <select id=\"{$this->getName()}_nonusers\" multiple=\"multiple\" style=\"width: 140px; height: 120px;\">";
        foreach($users as $user)
        {
            if(!isset($this->requestValue[$user->user_id]))
            {
                $html .= "        <option value=\"{$user->user_id}\">{$user->user_name}</option>";
            }
        }
        $html .= "    </select>";
        $html .= "</div>";
        $html .= "<div id='{$this->getName()}_values'></div>";
        return $html;
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_view()
     */
    protected function toHTML_view()
    {
        return "";
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#getJavascript()
     */
    public function getJavascript()
    {
        //TODO: fix this so that it can be used multiple times on the same page. Now it is depended on the field name.
        return <<<JS
            function form_UsersselectField_moveItem(from, to)
            {
                var fromObj = $('#'+from);
                var toObj = $('#'+to);
                
                fromObj.find("option").each(function(key, value) {
                    if(value.selected)
                    {
                        toObj.append($(value));
                    }
                });
                
                form_UsersselectField_updateUsersField()
            }
            
            function form_UsersselectField_updateUsersField()
            {
                $("#{$this->getName()}_values").empty();
                $("#{$this->getName()}_activeusers option").each(function(key, value) {
                    $('#{$this->getName()}_values').append("<input type='hidden' name='users[]' value='"+$(value).val()+"' />");
                });
                
                return true;
            }
            
            $().ready(function(e) {
                form_UsersselectField_updateUsersField();
            });
JS;
    }
}