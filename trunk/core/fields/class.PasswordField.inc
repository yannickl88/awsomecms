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
importField("Field");

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
        return "<input type='password' name='{$this->getName()}' id='{$this->getName()}' value='".htmlentities($this->getDefaultValue(), ENT_QUOTES)."'/>";
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_edit()
     */
    protected function toHTML_edit()
    {
        return "<input type='password' name='{$this->getName()}' id='{$this->getName()}' value='".htmlentities($this->getDefaultValue(), ENT_QUOTES)."'/>";
    }
    
    public function value2db($value)
    {
        return md5($value);
    }
}