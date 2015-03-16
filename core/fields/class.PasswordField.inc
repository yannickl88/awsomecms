<?php
/**
 * Password field
 *
 * @author Yannick <yannick.l.88@gmail.com>
 */
class PasswordField extends Field
{
    /**
     * Constructor
     *
     * @param string $name
     * @param string $displayName
     * @param int $style
     */
    public function __construct($name, $style=0)
    {
        parent::__construct($name, $style|Field::HIDE_VIEW);
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_add()
     */
    protected function toHTML_add()
    {
        return "<input type='password' class='cmsfield' name='{$this->getName()}' id='{$this->getName()}' value='".htmlentities($this->getDefaultValue(), ENT_QUOTES, "UTF-8")."'/>";
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_edit()
     */
    protected function toHTML_edit()
    {
        return "<input type='password' class='cmsfield' name='{$this->getName()}' id='{$this->getName()}' value='".htmlentities($this->getDefaultValue(), ENT_QUOTES, "UTF-8")."'/>";
    }

    public function value2db($value)
    {
        $salt = str_rand(9);

        return serialize(array("password" => sha1($salt.$value), "salt" => $salt));
    }

    public function db2value($value)
    {
        return unserialize($value);
    }
}