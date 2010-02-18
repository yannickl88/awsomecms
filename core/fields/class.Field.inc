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

/**
 * Superclass for a Field.
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class Field
{
    const ADD       = 1;
    const EDIT      = 2;
    const VIEW      = 3;
    const DELETE    = 4;
    
    //fieldoptions
    const HIDE_VIEW     = 1;
    const HIDE_ADD      = 2;
    const HIDE_EDIT     = 4;
    const HIDE          = 7; //HIDE_VIEW|HIDE_ADD|HIDE_EDIT
    const NO_INDEX      = 0; //not really needed but usefull
    const INDEX         = 8;
    const STR_INDEX     = 16;
    const OPTIONAL_ADD  = 32;
    const OPTIONAL_EDIT = 64;
    const OPTIONAL      = 96; //OPTIONAL_ADD|OPTIONAL_EDIT
    const NOTNULL       = 128;
    const AUTOKEY       = 256; //auto_increment is turned on at the database
    const FORCEUPDATE   = 512;
    const DUMMY         = 1024; //does not get's added to the selection list
    
    protected $name;
    protected $table;
    protected $displayName;
    protected $value;
    protected $requestValue;
    protected $defaultValue;
    protected $maxSize;
    
    public $sort;
    protected $style;
    
    //table stuff
    protected $required;
    protected $null;
    protected $index;
    
    /**
     * Constructor of the field
     *
     * @param string $name
     * @param mixed $value
     * @return Field
     */
    public function __construct($name, $style = 0)
    {
        $this->style = $style;
        
        $nameArray = explode(".", $name);
        
        if(count($nameArray) == 2)
        {
            $this->name = $nameArray[1];
            $this->table = $nameArray[0];
        }
        else
        {
            $this->name = $name;
            $this->table = null;
        }
        $this->displayName = Language::get("field_".$name);
        $this->value = "";
        
        $this->defaultValue = false;
        $this->null = !$this->hasFlag(Field::NOTNULL);
        if($this->hasFlag(Field::INDEX))
        {
            $this->index = Field::INDEX;
            $this->style = $this->style|Field::HIDE;
        }
        elseif($this->hasFlag(Field::STR_INDEX))
        {
            $this->index = Field::STR_INDEX;
        }
        else
        {
            $this->index = Field::NO_INDEX;
        }
        
        $this->maxSize = -1;
        
        return $this;
    }
    /**
     * Check if this field has a give flag
     * 
     * @param int $flag
     * @return boolean
     */
    public function hasFlag($flag)
    {
        return Config::hasFlag($this->style, $flag);
    }
    /**
     * Get the name of this field
     * 
     * @return string
     */
    public function getName()
    {
        return $this->name;
    }
    /**
     * Get the name that should be show in the forms
     * 
     * @return string
     */
    public function getDisplayName()
    {
        return $this->displayName;
    }
    /**
     * Set the Value of this field
     *
     * @param mixed $value
     * @return Field
     */
    public function setValue($value)
    {
        $this->value = $value;
        return $this;
    }
    /**
     * Set the value that the field got in the request
     * 
     * @param mixed $value
     * @return Field
     */
    public function setRequestValue($value)
    {
        $this->requestValue = $value;
        return $this;
    }
    /**
     * Set the Value of this field with an result object
     *
     * @param Object $value
     * @return Field
     */
    public function setData($record)
    {
        $name = $this->getName();
        if(isset($record->$name))
        {
            $this->setValue($record->$name);
        }
        
        return $this;
    }
    /**
     * Set the request value of this field with an request object
     * 
     * @param Object $record
     * @return Field
     */
    public function setRequestData($record)
    {
        $name = $this->getName();
        if(isset($record->$name))
        {
            return $this->setRequestValue($record->$name);
        }
        else
        {
            return null;
        }
    }
    /**
     * get the data from this field
     *
     * @param Object $data       Record object to fetch the data from
     * @return mixed
     */
    public function getValue($data = null)
    {
        if($data != null)
        {
            $name = $this->getName();
            
            if(isset($data->$name))
            {
                return $data->$name;
            }
            else
            {
                return null;
            }
        }
        else
        {
            $value = $this->value;
            return $value;
        }
    }
    /**
     * get the request data from this field
     * 
     * @param Object $data
     * @return mixed
     */
    public function getRequestValue($data = null)
    {
        if($data != null)
        {
            $name = $this->getName();
            return $data->$name;
        }
        else
        {
            $value = $this->requestValue;
            
            if(SQLQuery::isEmpty($value))
            {
                $value = $this->getDefaultValue();
            }
            
            return $value;
        }
    }
    /**
     * Check if this field has a value that is not empty
     * 
     * @param array $data
     * @return boolean
     */
    public function hasValue($data = null)
    {
        if(!empty($data))
        {
            return true;
        }
        else
        {
            return !SQLQuery::isEmpty($this->value);
        }
    }
    /**
     * Check if this field is required
     * 
     * @param int $mode
     * @return boolean
     */
    public function isRequired($mode)
    {
        if($mode == Field::ADD && $this->hasFlag(Field::OPTIONAL_ADD))
        {
            return false;
        }
        elseif($mode == Field::EDIT && $this->hasFlag(Field::OPTIONAL_EDIT))
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    /**
     * Set the field to be required
     *
     * @param boolean $required
     * @return Field
     */
    public function setRequired($required = true)
    {
        $this->required = ($required === true);
        return $this;
    }
    /**
     * Check if this field can be null
     * 
     * @return boolean
     */
    public function canBeNull()
    {
        return $this->null;
    }
    /**
     * Set the field to be null
     *
     * @param boolean $null
     * @return Field
     */
    public function setCanBeNull($null = true)
    {
        $this->null = ($null === true);
        return $this;
    }
    /**
     * Get the default value of this field
     * 
     * @return mixed
     */
    public function getDefaultValue()
    {
        return $this->defaultValue;
    }
    /**
     * Set the field to have a default value
     *
     * @param mixed $value
     * @return Field
     */
    public function setDefaultValue($value)
    {
        $this->defaultValue = $value;
        return $this;
    }
    /**
     * get the max size of this field
     * 
     * @return int
     */
    public function getMaxSize()
    {
        return $this->maxSize;
    }
    /**
     * Set the field max size, -1 to have no size
     *
     * @param int $size
     * @return Field
     */
    public function setMaxSize($size)
    {
        $this->maxSize = (int) $value;
        return $this;
    }
    /**
     * Check if this field in an index field
     * 
     * @return boolean
     */
    public function isIndex()
    {
        return $this->index > 0;
    }
    /**
     * Set the field ability to have an index
     *
     * @param string $index      FIELD::INDEX, FIELD::NO_INDEX, FIELD::STR_INDEX
     * @return Field
     */
    public function setIndex($index = FIELD::NO_INDEX)
    {
        switch($index)
        {
            case FIELD::NO_INDEX:
            case Field::INDEX:
            case Field::STR_INDEX:
                $this->index = $index;
                break;
        }
        return $this;
    }
    /**
     * check if this field is visible
     * 
     * @param int $mode
     * @return boolean
     */
    public function visible($mode = Field::VIEW)
    {
        return (
            ($mode == Field::ADD && (!Config::hasFlag($this->style, Field::HIDE_ADD))) || 
            ($mode == Field::EDIT && (!Config::hasFlag($this->style, Field::HIDE_EDIT))) || 
            ($mode == Field::VIEW && (!Config::hasFlag($this->style, Field::HIDE_VIEW))) || 
            ($mode == Field::DELETE)
        );
    }
    /**
     * This is designed to give fast acces to the toHTML functions
     *
     * @param string $name
     * @return string
     */
    public function __get($name)
    {
        switch($name)
        {
            case "add":
                return $this->toHTML(Field::ADD);
                break;
            case "edit":
                return $this->toHTML(Field::EDIT);
                break;
            case "delete":
                return $this->toHTML(Field::DELETE);
                break;
            case "view":
            default:
                return $this->toHTML(Field::VIEW);
                break;
        }
    }
    /**
     * Get the HTML representation of this field.
     * NOTE:Do not overwrite this function. Use the toHTML_add/edit/view instaid if you want to change the way the HTML looks
     *
     * @param int $mode
     */
    public function toHTML($mode = Field::ADD)
    {
        switch($mode)
        {
            case Field::ADD:
                return $this->toHTML_add();
                break;
            case Field::EDIT:
                if(!$this->visible($mode) && !$this->hasFlag(Field::FORCEUPDATE))
                {
                    return $this->toHTML_edit_hidden();
                }
                else
                {
                    return $this->toHTML_edit();
                }
                break;
            case Field::VIEW:
            case Field::DELETE:
            default:
                return $this->toHTML_view();
                break;
        }
    }
    /**
     * String representation of this field
     * 
     * @return string
     */
    public function __toString()
    {
        return $this->toHTML(Field::VIEW);
    }
    /**
     * Create a HTML representation of add field
     * 
     * @return string
     */
    protected function toHTML_add()
    {
        $value = $this->getDefaultValue();
        
        if($this->hasValue())
        {
            $value = $this->getValue();
        }
        
        return "<input type='text' name='{$this->getName()}' id='{$this->getName()}' value='".htmlentities($value, ENT_QUOTES)."'/>";
    }
    /**
     * Create a HTML representation of edit field
     * 
     * @return string
     */
    protected function toHTML_edit()
    {
        return "<input type='text' name='{$this->getName()}' id='{$this->getName()}' value='".htmlentities($this->getValue(), ENT_QUOTES)."'/>";
    }
    /**
     * Create a HTML representation of hidden field
     * 
     * @return string
     */
    protected function toHTML_edit_hidden()
    {
        return "<input type='hidden' name='{$this->getName()}' id='{$this->getName()}' value='".htmlentities($this->getValue(), ENT_QUOTES)."'/>";
    }
    /**
     * Create a HTML representation of view field
     * 
     * @return string
     */
    protected function toHTML_view()
    {
        return $this->getValue();
    }
    /**
     * get the javascript for this field
     * 
     * @return string
     */
    public function getJavascript()
    {
        return "";
    }
    /**
     * Add select data for this field to a query
     * 
     * @param $query SQLquery
     * @param $data
     */
    public function select($query, $data = array())
    {
        if($this->hasFlag(Field::DUMMY))
        {
            return;
        }
        
        if($this->isIndex())
        {
            $query->whereWhenSet($this->getDBName(), $this->value2db($this->getValue($data)));
        }
        $query->select($this->getDBName());
    }
    /**
     * Add insert data for this field to a query
     * 
     * @param $query SQLquery
     * @param $data
     */
    public function insert($query, $data = array())
    {
        if($this->hasFlag(Field::DUMMY) || $this->hasFlag(Field::AUTOKEY))
        {
            return;
        }
        if($this->isRequired(Field::ADD) || $this->hasFlag(Field::FORCEUPDATE))
        {
            $query->insert($this->getDBName(), $this->value2db($this->getValue()));
        }
        else
        {
            $query->insertWhenSet($this->getDBName(), $this->value2db($this->getValue()));
        }
    }
    /**
     * Add update data for this field to a query
     * 
     * @param $query SQLquery
     * @param $data
     */
    public function update($query, $data = array())
    {
        if($this->hasFlag(Field::DUMMY))
        {
            return;
        }
        
        if(!$this->isIndex())
        {
            if($this->isRequired(Field::EDIT) || $this->hasFlag(Field::FORCEUPDATE))
            {
                $query->update($this->getDBName(), $this->value2db($this->getValue()));
            }
            else
            {
                $query->updateWhenSet($this->getDBName(), $this->value2db($this->getValue()));
            }
        }
        else
        {
            if($this->hasFlag(Field::FORCEUPDATE))
            {
                $query->update($this->getDBName(), $this->value2db($this->getValue()));
            }
            
            if($this->isRequired(Field::EDIT))
            {
                $query->where($this->getDBName(), $this->value2db($this->getValue()));
            }
            else
            {
                $query->whereWhenSet($this->getDBName(), $this->value2db($this->getValue()));
            }
        }
    }
    /**
     * Add delete data for this field to a query
     * 
     * @param $query SQLquery
     * @param $data
     */
    public function delete($query, $data = array())
    {
        if($this->hasFlag(Field::DUMMY))
        {
            return;
        }
        
        if($this->isIndex())
        {
            if($this->isRequired(Field::DELETE))
            {
                $query->where($this->getDBName(), $this->value2db($this->getValue($data)));
            }
            else
            {
                $query->whereWhenSet($this->getDBName(), $this->value2db($this->getValue($data)));
            }
        }
    }
    /**
     * Validate the data for this field. This throws a FormException when not valid
     * 
     * @param int $mode
     */
    public function validate($mode)
    {
        $value = $this->value2db($this->getValue());
        
        if($mode == Field::ADD &&
            $this->isRequired($mode) && 
            !$this->hasValue($value) &&
            $this->hasFlag(Field::AUTOKEY) == false &&
            !$this->hasFlag(Field::DUMMY)
        )
        {
            throw new FormException($this, Language::get("error_notfilledin"));
        }
        if($mode == Field::EDIT &&
            $this->isRequired($mode) && 
            !$this->hasValue($value) &&
            !$this->hasFlag(Field::DUMMY)
        )
        {
            throw new FormException($this, Language::get("error_notfilledin"));
        }
    }
    /**
     * Convert the value of the param to that what the DB expects
     * 
     * @param $value String
     * @return String
     */
    public function value2db($value)
    {
        return $value;
    }
    /**
     * Get the name to be used in the SQL queries
     * 
     * @return string
     */
    public function getDBName()
    {
        if($this->table)
        {
            return $this->table.".".$this->getName();
        }
        else
        {
            return $this->getName();
        }
    }
}