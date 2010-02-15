<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package core.fields
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */
importField("IntField");

/**
 * Boolean field.
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class BooleanField extends IntField
{
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_add()
     */
    protected function toHTML_add()
    {
        $html = "";
        $html .= "<input type='radio' name='{$this->name}' id='{$this->name}' value='0' checked=\"checked\"/> On";
        $html .= "Off <input type='radio' name='{$this->name}' id='{$this->name}' value='0'/>";
        
        return $html;
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_edit()
     */
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
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_view()
     */
    protected function toHTML_view()
    {
        return ($this->value)? "Yes" : "No";
    }
}