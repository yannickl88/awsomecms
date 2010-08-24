<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package core
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */
import('/core/class.InvalidFormException.inc');

/**
 * Superclass for all the components
 *
 * @author Yannick <yannick.l.88@gmail.com>
 */
class Component
{
    private static $components;

    protected $component;
    protected $request;
    protected $currentTable;
    protected $location;

    /**
     * constructor
     *
     * @param string $component
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
     * @param string $component
     * @return Component
     */
    public static function init($component)
    {
        if(isset(Component::$components[$component]))
        {
            return Component::$components[$component];
        }
        else
        {
            $components = RegisterManager::getInstance()->getComponents();
            if(array_key_exists($component, $components))
            {
                try
                {
                    return self::createComponent($component, $components[$component]->component_path);
                }
                catch(Exception $e)
                {
                    Debugger::getInstance()->log($e->getMessage());
                }
            }
            else
            {
                Debugger::getInstance()->log("Cannot find Component file for '{$component}'");
            }
            Component::$components[$component] = false;
            return false;
        }
    }
    /**
     * Create a component instance based on the name and the path
     * 
     * NOTE: do not use this, use Component::init(). 
     *       Only use this to resolve ciclur reference from the register manager
     * @throws Exception
     * @param String $name
     * @param String $path
     * @return Component
     */
    public static function createComponent($name, $path)
    {
        $className = ucfirst($name)."Component";
        $loc = $path.'/class.'.$className.'.inc';

        Debugger::getInstance()->log("Creating Component '{$className}'");

        if(file_exists($loc) )
        {
            include_once $loc;

            $obj = new $className();
            
            if(!isset(Component::$components))
            {
                Component::$components = array();
            }
            
            Component::$components[$name] = $obj;
            $obj->location = $path;

            return $obj;
        }
        else
        {
            throw new Exception("Cannot find Component file for '{$className}'");
        }
    }
    /**
     * Set the default request that is linked to the component, default is the same as the component name
     *
     * @param string $request
     */
    protected function setDefaultRequest($request)
    {
        $this->request = $request;
    }
    /**
     * Get the name of the component
     * 
     * @return String;
     */
    public function getName()
    {
        return $this->component;
    }
    /**
     * Do an redirect
     *
     * @param string $url
     * @param array $data
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

        $link = makeLink($url).$query;

        Controller::getInstance()->setRedirect($link);
    }
    /**
     * Do an ajax return (just data, no HTML) in json format
     *
     * @param mixed $data
     */
    protected function ajax($data = true)
    {
        header('Content-type: application/json');
        Controller::getInstance()->setOutput(json_encode($data));
    }
    /**
     * Get the fields for this component
     * @deprecated Use tables
     *
     * @return array
     */
    protected function getFields()
    {
        Debugger::getInstance()->warning('Depricated ::getFields(), use the table to specifie the table layout');

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
    /**
     * Set the current table to be validated against
     * @deprecated Use tables
     *
     * @param string $table
     */
    protected function setCurrentTable($table)
    {
        Debugger::getInstance()->warning('Depricated ::setCurrentTable(), use the table to specifie the table layout');

        $this->currentTable = $table;
    }
    /**
     * Register menu items for this component
     *
     * @param Menu $menu
     */
    public function registerMenuItems($menu) { }
    /**
     * Register tree items for this component
     *
     * @param Tree $tree
     */
    public function registerTreeItems($tree) { }
    /**
     * Register actions/hooks/mappings to the framework
     *
     * @param RegisterManager $registerManager
     */
    public function register($registerManager) { }
    /**
     * Execute a task for the cron. This called in the interval secified in the config files.
     */
    public function execCron() { }
}