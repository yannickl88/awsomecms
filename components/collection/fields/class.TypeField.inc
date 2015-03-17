<?php
/**
 * Field for uploading files
 *
 * @author Yannick <yannick.l.88@gmail.com>
 */
class TypeField extends Field
{
    protected $types = array(
        "machine" => 1,
        "model" => 2,
        "type" => 3,
        "brand" => 4,
    );

    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_add()
     */
    protected function toHTML_add()
    {
        $html = "<select id=\"{$this->getName()}\" name=\"{$this->getName()}\" >";
        foreach($this->types as $key => $value)
        {
            $html .= "<option value=\"{$value}\">{$key}</option>";
        }
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
        foreach($this->types as $key => $value)
        {
            if($value == $this->getValue())
            {
                $html .= "<option selected=\"selected\" value=\"{$value}\">{$key}</option>";
            }
            else
            {
                $html .= "<option value=\"{$value}\">{$key}</option>";
            }
        }
        $html .= "</select>";
        return $html;
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_edit()
     */
    protected function toHTML_view()
    {
        $names = array_flip($this->types);
        return Language::get($names[$this->getValue()]);
    }
}