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
        $html .= "<input type='checkbox' name='{$this->name}' id='{$this->name}' value='1' checked=\"checked\"/>";
        
        return $html;
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_edit()
     */
    protected function toHTML_edit()
    {
        $html = "";
        
        $html .= "<input type='checkbox' name='{$this->name}' id='{$this->name}' value='1'";
        if($this->getValue() == 1)
        {
            $html .= " checked=\"checked\"";
        }
        $html .="/>";
        
        return $html;
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field::setData()
     *
     * @param object $record
     */
    public function setData($record)
    {
        $name = $this->getName();
        if(isset($record->$name))
        {
            $this->setValue($record->$name);
        }
        else 
        {
            $this->setValue(0);
        }
        
        return $this;
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