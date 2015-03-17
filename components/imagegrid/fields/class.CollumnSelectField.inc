<?php
/**
 * Field for uploading files
 *
 * @author Yannick <yannick.l.88@gmail.com>
 */
class CollumnSelectField extends IntField
{
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_add()
     */
    protected function toHTML_add()
    {
        $html = "<select id=\"{$this->getName()}\" name=\"{$this->getName()}\" >";
        $html .= "<option value=\"0\" selected='selected'>auto</option>";
        $html .= "<option value=\"2\">2 columns</option>";
        $html .= "<option value=\"3\">3 columns</option>";
        $html .= "</select>";
        return $html;
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_edit()
     */
    protected function toHTML_edit()
    {
        $html = "<select id=\"{$this->getName()}\" name=\"{$this->getName()}\" >";
        $html .= "<option value=\"0\"" . ($this->getValue() == 0 ? " selected='selected'" : "") .">auto</option>";
        $html .= "<option value=\"2\"" . ($this->getValue() == 2 ? " selected='selected'" : "") .">2 columns</option>";
        $html .= "<option value=\"3\"" . ($this->getValue() == 3 ? " selected='selected'" : "") .">3 columns</option>";
        $html .= "</select>";
        return $html;
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_edit()
     */
    protected function toHTML_view()
    {
        if($this->getValue() == 0) {
            return "auto";
        }
        return $this->getValue() . " columns";
    }
}