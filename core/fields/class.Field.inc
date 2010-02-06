<?php
import('/core/class.InvalidFormException.inc');

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
    
    protected $type;
    protected $name;
    protected $displayName;
    protected $value;
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
    public function __construct($name, $displayName = "", $style = 0)
    {
        $this->style = $style;
        $this->name = $name;
        $this->displayName = ucfirst($displayName);
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
        
        $this->type = "text";
        
        return $this;
    }
    
    public function hasFlag($flag)
    {
        return Config::hasFlag($this->style, $flag);
    }
    
    public function getType()
    {
        return $this->type;
    }
    
    public function getName()
    {
        return $this->name;
    }
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
    * Set the Value of this field with an result object
    * 
    * @param Object $value
    * @return Field
    */
    public function setData($record)
    {
        $name = $this->getName();
        return $this->setValue($record->$name);
    }
    
    /**
    * request the data from this field
    * 
    * @param Object $data       Record object to fetch the data from
    * @return mixed
    */
    public function getValue($data = null)
    {
        if(!empty($data))
        {
            $name = $this->getName();
            return $data->$name;
        }
        else
        {
            $value = $this->value;
            
            if(SQLQuery::isEmpty($value))
            {
                $value = $this->getDefaultValue();
            }
            
            return $value;
        }
    }
    
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
                if($this->isIndex() && !$this->hasFlag(Field::FORCEUPDATE))
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
    
    public function __toString()
    {
        return $this->toHTML(Field::VIEW);
    }
    
    protected function toHTML_add()
    {
        $value = $this->getDefaultValue();
        
        if($this->hasValue())
        {
            $value = $this->getValue();
        }
        
        return "<input type='text' name='{$this->getName()}' id='{$this->getName()}' value='{$value}'/>";
    }
    protected function toHTML_edit()
    {
        return "<input type='text' name='{$this->getName()}' id='{$this->getName()}' value='{$this->getValue()}'/>";
    }
    protected function toHTML_edit_hidden()
    {
        return "<input type='hidden' name='{$this->getName()}' id='{$this->getName()}' value='{$this->getValue()}'/>";
    }
    protected function toHTML_view()
    {
        return $this->getValue();
    }
    public function getJavascript()
    {
        return "";
    }
    
    /**
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
            $query->whereWhenSet($this->getName(), $this->value2db($this->getValue($data)));
        }
        $query->select($this->getName());
    }
    public function insert($query, $data = array())
    {
        if($this->hasFlag(Field::DUMMY) || $this->hasFlag(Field::AUTOKEY))
        {
            return;
        }
        if($this->isRequired(Field::ADD) || $this->hasFlag(Field::FORCEUPDATE))
        {
            $query->insert($this->getName(), $this->value2db($this->getValue()));
        }
        else
        {
            $query->insertWhenSet($this->getName(), $this->value2db($this->getValue($data)));
        }
    }
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
                $query->update($this->getName(), $this->value2db($this->getValue($data)));
            }
            else
            {
                $query->updateWhenSet($this->getName(), $this->value2db($this->getValue($data)));
            }
        }
        else
        {
            if($this->hasFlag(Field::FORCEUPDATE))
            {
                $query->update($this->getName(), $this->value2db($this->getValue($data)));
            }
            
            if($this->isRequired(Field::EDIT))
            {
                $query->where($this->getName(), $this->value2db($this->getValue($data)));
            }
            else
            {
                $query->whereWhenSet($this->getName(), $this->value2db($this->getValue($data)));
            }
        }
    }
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
                $query->where($this->getName(), $this->value2db($this->getValue($data)));
            }
            else
            {
                $query->whereWhenSet($this->getName(), $this->value2db($this->getValue($data)));
            }
        }
    }
    
    public function validate($mode)
    {
        $value = $this->getValue();
        
        if(($mode == Field::ADD || $mode == Field::EDIT) &&
            $this->isRequired($mode) && 
            empty($value) && 
            ($this->hasFlag(Field::AUTOKEY) == false && $mode != Field::EDIT)
        )
        {
            throw new FormException($this, "Field was not filled in");
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
}