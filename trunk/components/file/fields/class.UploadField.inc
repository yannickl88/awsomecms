<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.file.fields
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */
importField("Field");

/**
 * Field for uploading files
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class UploadField extends Field
{
/**
     * Constructor
     * 
     * @param string $name
     * @param int $style
     */
    public function __construct($name, $style=0)
    {
        parent::__construct($name, $style|Field::HIDE_VIEW|Field::DUMMY);
    }
    
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_add()
     */
    protected function toHTML_add()
    {
        return "<input type='file' name='{$this->getName()}' id='{$this->getName()}'/>";
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_edit()
     */
    protected function toHTML_edit()
    {
        return $this->toHTML_add();
    }
    /**
     * Convert the dbvalue of the param to what the field expects
     * 
     * @param $value mixed
     * @return mixed
     */
    public function db2value($value)
    {
        return unserialize($value);
    }
    /**
     * Convert the value of the param to that what the DB expects
     * 
     * @param $value String
     * @return String
     */
    public function value2db($value)
    {
        return serialize($value);
    }
}