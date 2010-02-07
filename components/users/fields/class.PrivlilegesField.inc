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
 * Privileges list of check boxes
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class PrivlilegesField extends Field
{
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_add()
     */
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
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_edit()
     */
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
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#value2db($value)
     */
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