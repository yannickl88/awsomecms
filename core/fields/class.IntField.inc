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
import('/core/class.InvalidFormException.inc');

importField("Field");

/**
 * Integer field
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class IntField extends Field
{
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_add()
     */
    protected function toHTML_add()
    {
        return "<input type='text' name='{$this->getName()}' id='{$this->getName()}' value='{$this->getDefaultValue()}' size='5' onkeypress='return form_IntField_handleKey(event);'/>";
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_edit()
     */
    protected function toHTML_edit()
    {
        return "<input type='text' name='{$this->getName()}' id='{$this->getName()}' value='{$this->getValue()}' size='5' onkeypress='return form_IntField_handleKey(event);'/>";
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#getJavascript()
     */
    public function getJavascript()
    {
        return <<<JS
            function form_IntField_handleKey(elem)
            {
                return (elem.charCode > 0 && !isNaN(parseInt(String.fromCharCode(elem.charCode)))) || elem.charCode == 0;
            }
JS;
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#validate($mode)
     */
    public function validate($mode)
    {
        $value = $this->getValue();
        
        if(is_int($value) || is_numeric($value))
        {
            return;
        }
        else
        {
            parent::validate($mode);
            
            if($mode == Field::ADD && ($this->isIndex() || $this->isRequired($mode)) && !$this->hasFlag(Field::AUTOKEY))
            {
                echo "asdf";
                throw new FormException($this, "Field was not a number");
            }
            
            if($mode == Field::EDIT && $this->isIndex() && !is_numeric($this->getValue()))
            {
                throw new FormException($this, "Field was not a number");
            }
        }
    }
}