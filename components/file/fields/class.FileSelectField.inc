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
class FileSelectField extends Field
{
    protected $types;
    
    /**
     * Constructor
     *
     * @param string $name
     * @param int $style
     */
    public function __construct($name, $types, $style=0)
    {
        parent::__construct($name, $style|Field::HIDE_VIEW);
        $this->types = $types;
    }
    
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_add()
     */
    protected function toHTML_add()
    {
        $files = Table::init("file.files")
            ->doSelect();
        
        $html = "<select id=\"{$this->getName()}\" name=\"{$this->getName()}[]\" multiple=\"multiple\">";
        while($row = $files->getRow())
        {
            if(in_array(array_pop(explode("/", $row->file_data->type)), $this->types))
            {
                $html .= "<option value=\"{$row->file_id}\">{$row->file_name}</option>";
            }
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
        $files = Table::init("file.files")
            ->doSelect();
        
        $html = "<select id=\"{$this->getName()}\" name=\"{$this->getName()}[]\" multiple=\"multiple\">";
        while($row = $files->getRow())
        {
            if(in_array(array_pop(explode("/", $row->file_data->type)), $this->types))
            {
                if($this->getValue() != "" && in_array($row->file_id, $this->getValue()))
                {
                    $html .= "<option selected=\"selected\" value=\"{$row->file_id}\">{$row->file_name}</option>";
                }
                else
                {
                    $html .= "<option value=\"{$row->file_id}\">{$row->file_name}</option>";
                }
            }
        }
        $html .= "</select>";
        
        return $html;
    }
    /**
     * Convert the dbvalue of the param to what the field expects
     * 
     * @param $value mixed
     * @return mixed
     */
    public function db2value($value)
    {
        if(!empty($value))
        {
            return explode(",", $value);
        }
        
        return $value;
    }
    /**
     * Convert the value of the param to that what the DB expects
     * 
     * @param $value String
     * @return String
     */
    public function value2db($value)
    {
        if(!empty($value))
        {
            return implode(",", $value);
        }
        
        return $value;
    }
}