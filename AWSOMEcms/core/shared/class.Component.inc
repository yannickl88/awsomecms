<?php
import('/core/shared/class.InvalidFormException.inc');

/**
* @desc superclass for all the components
*/
class Component
{
    private static $components;
    
    protected $component;
    protected $request;
    protected $fields;
    protected $multiTable;
    protected $currentTable;
    
    /**
    * @desc constructor
    * 
    * @param $component     Component name
    */
    public function __construct($component)
    {
        $this->component = $component;
        $this->request = $component;
        $this->multiTable = false;
        
        if(!isset($this->fields))
        {
            $this->fields = array();
        }
    }
    
    /**
    * Initialisation of the component corresponding with the give name
    * 
    * @param $component     String
    * @return               Component object or false when it cannot be found
    */
    public static function init($component)
    {
        if(isset(Component::$components[$component]))
        {
            return Component::$components[$component];
        }
        else
        {
            if(!isset(Component::$components))
            {
                Component::$components = array();
            }
            
            $components = Config::getInstance()->getComponenets();
            
            $className = ucfirst($component)."Component";
            $loc = $components[$component]->component_path.'/class.'.$className.'.inc';
            
            Debugger::getInstance()->log("Creating Component '{$className}'");
            
            if(file_exists($loc) && array_key_exists($component, $components))
            {
                include_once $loc;
                
                $obj = new $className();
                Component::$components[$component] = $obj;
                
                return $obj;
            }
            else
            {
                Debugger::getInstance()->log("Cannot find Component '{$className}'");
                Component::$components[$component] = false;
                return false;
            }
        }
    }
    
    /**
    * Set the default request that is linked to the component, default is the same as the component name
    * 
    * @param $request       String
    */
    protected function setDefaultRequest($request)
    {
        $this->request = $request;
    }
    
    /**
    * @desc Do an select for the component
    * 
    * @param $data          Array with data to be send
    * @return               Mixed
    */
    public function select($data = array())
    {
        Debugger::getInstance()->warning('Depricated ::select(), use the request object');
        return Request::init($this->request)->doSelect($data);
    }
    
    /**
    * @desc Do an delete for the component
    * 
    * @param $data          Array with data to be send
    * @return               Mixed
    */
    public function delete($data = array())
    {
        Debugger::getInstance()->warning('Depricated ::delete(), use the request object');
        return Request::init($this->request)->doDelete($data);
    }
    
    /**
    * @desc Do an update for the component
    * 
    * @param $data          Array with data to be send
    * @return               Mixed
    */
    public function update($data = array())
    {
        Debugger::getInstance()->warning('Depricated ::update(), use the request object');
        return Request::init($this->request)->doUpdate($data);
    }
    
    /**
    * @desc Do an insert for the component
    * 
    * @param $data          Array with data to be send
    * @return               Mixed
    */
    public function insert($data = array())
    {
        Debugger::getInstance()->warning('Depricated ::insert(), use the request object');
        return Request::init($this->request)->doInsert($data);
    }
    
    /**
    * @desc Do an redirect
    * 
    * @param $url           Url to redirect to
    * @param $data          Array with data to be send
    */
    protected function redirect($url, $data = array())
    {
        $query = '';
        
        foreach($data as $key => $item)
        {
            if($query == '')
            {
                $query .= '?';
            }
            else
            {
                $query .= '&';
            }
            
            $query .= $key.'='.$item;
        }
        
        $url .= $query;
        
        if(Config::get('debug', 0, 'debug') > 1)
        {
            echo "Non debug would have redirected to \" <a href='{$url}'>{$url}</a> \"";
        }
        else
        {
            header("Location: {$url}");
        }
        exit;
    }
    
    /**
    * @desc Do an ajax return (just data, no HTML) in json format
    * 
    * @param $data          Mixed data to be outputted
    */
    protected function ajax($data = true)
    {
        //header('Content-type: application/json');
        echo json_encode($data);
        exit;
    }
    
    /**
    * Validate the data against the fields
    * 
    * @param $data          Array with the data (fieldname => data)
    * @param $redirectOnFailure String with the location to redirect when not valid, this is usally the same page the form is on
    * @param $mode          String with the modus (add|edit)
    * @return boolean/Exception
    */
    protected function validate($data, $redirectOnFailure, $mode = "edit")
    {
        $valid = true;
        $index = -1;
        $exception = new InvalidFormException($data, $redirectOnFailure, "The form was not valid");
        
        foreach($this->getFields() as $field => $type)
        {
            //check for index
            if($index == -1 && ($type == 'index' || $type == 'str_index'))
            {
                $index = $field;
                
                if($mode == 'add') //if we are in add more, no need to validate something the isn't there yet
                {
                    continue;
                }
            }
            
            //isset
            if(!array_key_exists($field, $data) || empty($data[$field]))
            {
                $valid = false;
                
                $exception->addInvalidField($field, "Field was not filled in");
                continue;
            }
            
            // extra validation
            switch(strtolower($type))
            {
                case 'index' :
                    if(!is_numeric($data[$field]))
                    {
                        $valid = false;
                        
                        $exception->addInvalidField($field, "Field was not a number");
                        continue;
                    }
                    break;
                case 'int' :
                    if(!is_numeric($data[$field]))
                    {
                        $valid = false;
                        
                        $exception->addInvalidField($field, "Field was not a number");
                        continue;
                    }
                    break;
            }
        }
        
        if(!$valid)
        {
            if($mode == "edit" && $index != -1)
            {
                $exception->setRedirectData(array($index => $data[$index]));
            }
            
            throw $exception;
        }
        
        return $valid;
    }
    
    protected function getFields()
    {
        if($this->multiTable)
        {
            if(isset($this->currentTable) && array_key_exists($this->currentTable, $this->fields))
            {
                return $this->fields[$this->currentTable];
            }
            else
            {
                $keys = array_keys($this->fields);
                return $this->fields[$keys[0]];
            }
        }
        else
        {
            return $this->fields;
        }
    }
    
    protected function setCurrentTable($table)
    {
        $this->currentTable = $table;
    }
    
    public function registerMenuItems($menu) { }
}