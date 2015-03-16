<?php
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
        return "<input type='text' class='cmsfield' name='{$this->getName()}' id='{$this->getName()}' value='".htmlentities($this->getDefaultValue(), ENT_QUOTES, "UTF-8")."' size='5' onkeypress='return form_IntField_handleKey(event);'/>";
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_edit()
     */
    protected function toHTML_edit()
    {
        return "<input type='text' class='cmsfield' name='{$this->getName()}' id='{$this->getName()}' value='".htmlentities($this->getValue(), ENT_QUOTES, "UTF-8")."' size='5' onkeypress='return form_IntField_handleKey(event);'/>";
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
        $value = $this->value2db($this->getValue());

        if(is_int($value) || is_numeric($value))
        {
            return;
        }
        else
        {
            parent::validate($mode);

            if($mode == Field::ADD && ($this->isIndex() || $this->isRequired($mode)) && !$this->hasFlag(Field::AUTOKEY))
            {
                throw new FormException($this, Language::get("error_notnumber"));
            }

            if($mode == Field::EDIT && $this->isIndex() && !is_numeric($this->getValue()))
            {
                throw new FormException($this, Language::get("error_notnumber"));
            }
        }
    }
}