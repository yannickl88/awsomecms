<?php
/**
 * Date field, this shows a date selection field
 *
 * @author Yannick <yannick.l.88@gmail.com>
 */
class DateField extends Field
{
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#value2db($value)
     */
    public function value2db($value)
    {
        return date("c", strtotime($value));
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#getValue($data)
     */
    public function getValue($data = null)
    {
        return date("m/d/Y", strtotime(parent::getValue($data)));
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#getDefaultValue()
     */
    public function getDefaultValue()
    {
        return date("m/d/Y", time());
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_add()
     */
    protected function toHTML_add()
    {
        return "<input type='text' class='cmsfield dateField' name='{$this->getName()}' id='{$this->getName()}' value='{$this->getDefaultValue()}'/>";
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_edit()
     */
    protected function toHTML_edit()
    {
        return "<input type='text' class='cmsfield dateField' name='{$this->getName()}' id='{$this->getName()}' value='{$this->getValue()}'/>";
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_view()
     */
    protected function toHTML_view()
    {
        return date("c", strtotime($this->value));
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#getJavascript()
     */
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