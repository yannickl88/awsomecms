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
 * Container for dynamic data, this always returnes NULL when a propertie is not found
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class DataContainer
{
    /**
     * Getter
     * 
     * @param mixed $key
     * @return mixed
     */
    public function __get($key)
    {
        if(isset($this->$key))
        {
            return $this->$key;
        }
        
        return null;
    }
    /**
     * Setter
     * 
     * @param mixed $key
     * @param mixed $value
     */
    public function __set($key, $value)
    {
        $this->$key = $value;
    }
    /**
     * Wrap an object or an array containing object in the DataContainer
     * 
     * @param object $data
     * @return mixed
     */
    public static function init($data)
    {
        if(is_array($data))
        {
            $dataObj = array();
            
            foreach($data as $key => $value)
            {
                $dataObj[$key] = self::init($value);
            }
            
            return $dataObj;
        }
        else if(is_object($data))
        {
            $dataObj = new DataContainer();
            
            foreach($data as $key => $value)
            {
                $dataObj->$key = $value;
            }
            
            return $dataObj;
        }
        
        return $data;
    }
}
/**
 * Abstract class that handels all the information about a table
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
abstract class Table
{
    private static $tables;
    
    public $ID;
    
    protected $fields;
    protected $fieldTypes;
    protected $keys;
    protected $name;
    protected $record;
    protected $request;
    protected $error;
    protected $errorRecord;
    protected $errorFields;
    protected $title;
    
    protected $component;
    protected $action;
    protected $lastInsertedID;
    protected $fromExternal;
    protected $multipart;
    
    protected $adminURL;
    protected $addURL;
    protected $editURL;
    protected $deleteURL;
    
    /**
    * Static function to get a table object
    * 
    * @param string $table      <component>.<tabel>
    * @return Table
    */
    public static function init($table, $ID = "")
    {
        if(isset(self::$tables[$ID.$table]))
        {
            return self::$tables[$ID.$table];
        }
        else
        {
            if(!isset(self::$tables))
            {
                self::$tables = array();
            }
            
            $components = Config::getInstance()->getComponenets();
            
            $tableID = explode(".", $table);
            
            $className = ucfirst($tableID[1])."Table";
            $loc = $components[$tableID[0]]->component_path.'/class.'.$className.'.inc';
            
            Debugger::getInstance()->log("Creating Table '{$className}' {$ID}");
            
            if(file_exists($loc) && array_key_exists($tableID[0], $components))
            {
                include_once $loc;
                
                $obj = new $className();
                $obj->ID = $table;
                self::$tables[$ID.$table] = $obj;
                
                return $obj;
            }
            else
            {
                Debugger::getInstance()->error("Cannot find Table '{$className}'");
                self::$tables[$ID.$table] = false;
                return false;
            }
        }
    }
    /**
    * Set the urls the Table needs to have for redirection on errors
    * 
    * @param string $admin
    * @param string $add
    * @param string $edit
    * @param string $delete
    * @return Table
    */
    protected function setURLs($admin, $add, $edit, $delete)
    {
        $this->adminURL = $admin;
        $this->addURL = $add;
        $this->editURL = $edit;
        $this->deleteURL = $delete;
        
        return $this;
    }
    /**
    * Constructor for Table
    * 
    * @param string $name
    * @return Table
    */
    public function __construct($name)
    {
        $this->name = $name;
        $this->fields = array();
        $this->keys = array();
        $this->fieldTypes = array();
        $this->fromExternal = false;
        
        return $this;
    }
    /**
     * Set if the table is being accessed from an external source.
     * 
     * @param boolean $fromExternal
     * @return Table
     */
    public function setExternal($fromExternal)
    {
        $this->fromExternal = $fromExternal;
        
        return $this;
    }
    /**
     * Check if the table is being accessed from an external source.
     * 
     * @return boolean
     */
    public function getExternal()
    {
        return $this->fromExternal;
    }
    /**
     * Set the multipart so the forms are send with that field
     * 
     * @param Boolean $multipart
     * @return Table
     */
    public function setMultipart($multipart = true)
    {
        $this->multipart = $multipart;
        return $this;
    }
    /**
    * Assign the table to a component with an action. 
    * 
    * @param string $component
    * @param string $action
    * @return Table
    */
    public function assign($component = null, $action = null)
    {
        $this->component = $component;
        $this->action = $action;
        
        return $this;
    }
    /**
    * Add Field to the table
    * 
    * @param Field $field
    * @param int $sort      The lower the sort, the furter on top in the list
    * @return Table
    */
    public function addField($field, $sort = 0)
    {
        $field->sort = $sort;
        $field->setParent($this);
        $this->fields[] = $field;
        
        $class = get_class($field);
        if(!isset($this->fieldTypes[$class]))
        {
            $this->fieldTypes[$class] = $field;
        }
        
        if($field->isIndex())
        {
            $this->keys[] = $field;
        }
        
        $this->sortFields();
        
        return $this;
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
     * Get a field based on it's name, NULL when there was no field found
     * 
     * @param string $name
     * @return Field
     */
    public function getField($name)
    {
        foreach($this->fields as $field)
        {
            if($field->getName() == $name)
            {
                return $field;
            }
        }
        
        return null;
    }
    /**
     * Get all the fields in this table
     * 
     * @return array
     */
    public function getFields()
    {
        return $this->fields;
    }
    /**
     * Get the name of this tabel.
     * 
     * @return String
     */
    public function getName()
    {
        return $this->name;
    }
    /**
     * Get the title of this tabel record. This is a pre-parsed string with the values from the record
     * 
     * @return String
     */
    public function getTitle()
    {
        return $this->title;
    }
    /**
     * Set the title of this tabel record. This is a string with the field names between [ and ]
     * 
     * @param $titleString String
     * @return Table
     */
    public function setTitle($titleString)
    {
        $this->title = $titleString;
        
        return $this;
    }
    /**
    * Set the current record of the table
    * 
    * @param Object $data
    * @return Table
    */
    public function setRecord($data)
    {
        if(!isset($this->errorRecord) && !empty($data))
        {
            if(!is_array($data))
            {
                $this->record = DataContainer::init(array($data));
            }
            else
            {
                $this->record = DataContainer::init($data);
            }
            
            foreach($this->fields as $field)
            {
                $field->setData($this->record[0]);
            }
        }
        
        return $this;
    }
    /**
     * Set the request data for this table
     * 
     * @param Object $data
     * @return Table
     */
    public function setRequest($data)
    {
        if(!empty($data))
        {
            if(!is_array($data))
            {
                $this->request = DataContainer::init(array($data));
            }
            else
            {
                $this->request = DataContainer::init($data);
            }
            
            foreach($this->fields as $field)
            {
                $field->setRequestData($this->request[0]);
            }
        }
        
        return $this;
    }
    public function getRecord()
    {
        return $this->record;
    }
    public function getRequest()
    {
        return $this->request;
    }
    /**
    * Set the current error record of the table, this has priority over the current one
    * 
    * @param InvalidFormException $error
    * @return Table
    */
    public function setError($error)
    {
        $data = $error->getRecord();
        
        if(!is_array($data))
        {
            $this->errorRecord = DataContainer::init(array($data));
        }
        else
        {
            $this->errorRecord = DataContainer::init($data);
        }
        
        $this->errorFields = $error->getInvalidFields();
        $this->error = $error;
        
        foreach($this->fields as $field)
        {
            $field->setData($this->errorRecord[0]);
        }
        
        return $this;
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
                return $this->toHTML_edit();
                break;
            case Field::DELETE:
                return $this->toHTML_delete();
                break;
            case Field::VIEW:
            default:
                return $this->toHTML_view();
                break;
        }
    }
    /**
     * Convert this table to a string
     * 
     * @return string
     */
    public function __toString()
    {
        try
        {
            return $this->toHTML(Field::VIEW);
        }
        catch(Exception $e)
        {
            return nl2br($e->__toString());
        }
    }
    /**
     * Sort the fields by their sort values
     */
    protected function sortFields()
    {
        usort($this->fields, array($this, "handleSort"));
    }
    /**
     * Sorthing algorithem
     * 
     * @param Field $a
     * @param Field $b
     * @return int
     */
    protected function handleSort($a, $b)
    {
        if ($a->sort == $b->sort)
        {
            return strcasecmp($a->getName(), $b->getName());
        }
        return ($a->sort < $b->sort) ? -1 : 1;
    }
    /**
     * HTML representation for add Form
     * 
     * @return string
     */
    protected function toHTML_add()
    {
        $fields = "";
        foreach($this->fields as $field)
        {
            if($field->visible(Field::ADD))
            {
                $fields .= "<div class=\"admin_form_row\">\n";
                $fields .= "<div class=\"admin_form_label\"><label for=\"".$field->getName()."\">".$field->getDisplayName().":</label></div>\n";
                $fields .= "<div class=\"admin_form_field\">".$field->toHTML(Field::ADD)."</div>\n";
                $fields .= "</div>\n";
            }
        }
        
        $html = $this->getErrors();
        $html .= "<h2>".ucfirst(Language::get("addto"))." ".Language::get("comp_".$this->component)."</h2>\n";
        $multipart = "";
        if($this->multipart)
        {
            $multipart = ' enctype="multipart/form-data"';
        }
        
        $html .= "<form method=\"post\" action=\"/\" class=\"admin_form admin_add\"{$multipart}>\n";
        $html .= $fields;
        $html .= "<div class=\"admin_form_row admin_form_submit\">\n";
        $html .= "<input type='submit' value='".ucfirst(Language::get("save"))."' />\n";
        $html .= $this->getAssignment(Field::ADD);
        $html .= "</div>\n";
        $html .= "</form>\n";
        $html .= $this->getJavascript();
        
        return $html;
    }
    /**
     * HTML representation for edit Form
     * 
     * @return string
     */
    protected function toHTML_edit()
    {
        $data = $this->doSelect($this->request[0])->getRow();
        $html_post = "";
        $fields = "";
        foreach($this->fields as $field) //@var $field Field
        {
            $field->setData($data);
            
            if($field->visible(Field::EDIT))
            {
                $fields .= "<div class=\"admin_form_row\">\n";
                $fields .= "<div class=\"admin_form_label\"><label for=\"".$field->getName()."\">".$field->getDisplayName().":</label></div>\n";
                $fields .= "<div class=\"admin_form_field\">".$field->toHTML(Field::EDIT)."</div>\n";
                $fields .= "</div>\n";
            }
            elseif($field->isIndex())
            {
                $html_post .= $field->toHTML(Field::EDIT)."\n";
            }
        }
        
        $html = $this->getErrors();
        $html .= "<h2>".ucfirst(Language::get("edit"))." ".$this->parseTitle($this->title, $data)."</h2>\n";
        $multipart = "";
        if($this->multipart)
        {
            $multipart = ' enctype="multipart/form-data"';
        }
        $html .= "<form method=\"post\" action=\"/\" class=\"admin_form admin_edit\"{$multipart}>\n";
        $html .= $fields;
        $html .= "<div class=\"admin_form_row admin_form_submit\">\n";
        $html .= "<input type='submit' value='".ucfirst(Language::get("save"))."' />\n";
        $html .= $html_post;
        $html .= $this->getAssignment(Field::EDIT);
        $html .= "</div>\n";
        $html .= "</form>\n";
        $html .= $this->getJavascript();
        
        return $html;
    }
    /**
     * HTML representation for view Form
     * 
     * @return string
     */
    protected function toHTML_view()
    {
        $data = $this->doSelect($this->request[0])->getRows();
        
        $html = "";
        $html .= "<h2>".ucfirst(Language::get("comp_".$this->component))." ".Language::get("overview")."</h2>\n";
        $html .= "<table class=\"admin_table\">\n";
        $html .= "    <tr class=\"admin_table_header\">\n";
        foreach($this->fields as $field)
        {
            if($field->visible(Field::VIEW))
            {
                $html .= "        <th class=\"admin_table_header\">".$field->getDisplayName()."</th>\n";
            }
        }
        $html .= "        <th class=\"admin_table_header admin_table_actions\">\n        </th>\n";
        $html .= "        <th class=\"admin_table_header admin_table_actions\">\n        </th>\n";
        $html .= "    </tr>\n";
        
        $count = 0;
        $cols = 0;
        foreach($data as $row)
        {
            $class = "admin_table_row";
            
            if($count%2 == 1)
            {
                $class .= " highlighted";
            }
            
            $html .= "    <tr class=\"{$class}\">\n";
            $cols = 0;
            foreach($this->fields as $field)
            {
                if($field->visible(Field::VIEW))
                {
                    $html .= "        <td class=\"admin_table_cell\">".$field->getValue($row)."</td>\n";
                    $cols++;
                }
            }
            
            $queryString = "";
            
            foreach($this->keys as $key)
            {
                if($queryString != "")
                {
                    $queryString .= "&";
                }
                
                $queryString .= $key->getName()."=".$key->getValue($row);
            }
            
            if($queryString != "")
            {
                $queryString = "?".$queryString;
            }
            
            //actions
            $html .= "        <td class=\"admin_table_cell admin_table_actions\">\n";
            $html .= "            <a href='/{$this->editURL}{$queryString}'><img src=\"/img/icons/edit.png\" alt=\"".Language::get("edit")."\" /></a>";
            $html .= "            <a href='/{$this->deleteURL}{$queryString}'><img src=\"/img/icons/delete.png\" alt=\"".Language::get("delete")."\" /></a>\n";
            $html .= "        </td>\n";
            $html .= "        <td class=\"admin_table_cell admin_table_actions\">\n";
            $html .= "            <input type=\"checkbox\" value=\"".$this->keys[0]->getValue($row)."\" name=\"action[]\" class=\"actioncheckbox\" />\n";
            $html .= "        </td>\n";
            
            $html .= "    </tr>\n";
            $count++;
        }
        if($count == 0) //add empty row
        {
            foreach($this->fields as $field)
            {
                if($field->visible(Field::VIEW))
                {
                    $cols++;
                }
            }
            
            $colspan = $cols + 2;
            $html .= "    <tr>\n";
            $html .= "        <td class=\"admin_table_cell\" colspan=\"{$colspan}\">".Language::get("norecords")."</td>\n";
            $html .= "    </tr>\n";
        }
        
        $colspan = $cols + 1;
        $html .= "    <tr class=\"admin_table_row admin_table_actions_row\">\n";
        $html .= "        <td class=\"admin_table_cell admin_table_actions\" colspan=\"{$colspan}\">\n";
        $html .= "            <img src=\"/img/icons/add.png\" alt=\"add\" /> <a href=\"/{$this->addURL}\">".ucfirst(Language::get("add"))."</a>\n";
        $html .= "        </td>\n";
        $html .= "        <td class=\"admin_table_cell\">\n";
        $html .= "            <a href=\"#\" onclick=\"return deleteMultiple('/{$this->deleteURL}', '{$this->keys[0]->getName()}', $('.actioncheckbox'));\"><img src=\"/img/icons/delete.png\" alt=\"".Language::get("deleteselected")."\" /></a>\n";
        $html .= "        </td>\n";
        $html .= "    </tr>\n";
        $html .= "</table>\n";
        $html .= $this->getJavascript();
        
        return $html;
    }
    /**
     * HTML representation for delete Form
     * 
     * @return string
     */
    protected function toHTML_delete()
    {
        $data = $this->doSelect($this->request[0])->getRows();
        
        $html = "";
        $post_html = "";
        $html .= "<h2>".ucfirst(Language::get("deletefrom"))." ".Language::get("comp_".$this->component)."</h2>\n";
        $html .= "<form method=\"post\" action=\"/\" class=\"admin_form admin_delete\">\n";
        $html .= "    <div class=\"admin_form_row\">\n";
        $html .= "        <div class=\"admin_form_text\">\n";
        $html .= "            ".Language::get("areyousuredelete")." <br />\n";
        $html .= "            <ul>\n";
        
        foreach($data as $row)
        {
            $title = "";
            
            if(true)
            {
                $title = $this->parseTitle($this->title, $row);
            }
            else
            {
                $title = $this->keys[0]->getName();
                if(!empty($title))
                {
                    $title = $row->$title;
                }
            }
            $html .= "                <li>{$title}</li>\n";
            
            foreach($this->keys as $field)
            {
                $post_html .= "    <input type=\"hidden\" name=\"{$field->getName()}[]\" value=\"{$field->getValue($row)}\" />\n";
            }
        }
        
        $html .= "            </ul>\n";
        $html .= "        </div>\n";
        $html .= "    </div>\n";
        $html .= "    <div class=\"admin_form_row admin_form_submit\">\n";
        $html .= "        <input type=\"submit\" value=\"".ucfirst(Language::get("delete"))."\" id=\"page_submit\" class=\"admin_form_submit\" />\n";
        $html .= $post_html;
        $html .= $this->getAssignment(Field::DELETE);
        $html .= "    </div>\n";
        $html .= "</form>\n";
        
        return $html;
    }
    /**
     * A way to parse the title string.
     * 
     * @param string $title
     * @param array $data
     * @return string
     */
    protected function parseTitle($title, $data)
    {
        $matches = array();
        $titleArray = preg_match_all('/\[(.*?)\]/', $title, $matches);
        
        foreach($matches[1] as $match)
        {
            $field =  $this->getField($match);
            if($field)
            {
                $title = str_replace("[{$match}]", $field->getValue($data), $title);
            }
        }
        
        return $title;
    }
    /**
    * Preform a select on the table
    * 
    * @return SQLRecord
    */
    public function doSelect()
    {
        $data = $this->request[0];
        
        $this->validate(Field::VIEW);
        
        $query = SQLQuery::doSelect();
        $query->table($this);
        
        $this->callPreFields($data, $query);
        foreach($this->fields as $field)
        {
            $field->select($query, $data);
        }
        
        $this->select($query, $data);
        
        $result = $query->exec();
        $this->callPostFields($data, $query);
        
        return $result;
    }
    /**
    * Preform an insert on the table
    * 
    * @return int, last inserted ID
    */
    public function doInsert()
    {
        $data = $this->request[0];
        $this->validate(Field::ADD);
        
        $query = SQLQuery::doInsert();
        $query->table($this);
        
        $this->callPreFields($data, $query);
        foreach($this->fields as $field)
        {
            $field->insert($query, $data);
        }
        
        $this->insert($query, $data);
        
        $result = $query->exec();
        $this->callPostFields($data, $query);
        
        $this->lastInsertedID = $result->insertID();
        
        return $this->getInsertedID();
    }
    /**
    * Preform an update on the table
    * 
    * @return SQLRecord
    */
    public function doUpdate()
    {
        $data = $this->request[0];
        $this->validate(Field::EDIT);
        
        $query = SQLQuery::doUpdate();
        $query->table($this);
        
        $this->callPreFields($data, $query);
        foreach($this->fields as $field)
        {
            $field->update($query, $data);
        }
        
        $this->update($query, $data);
        
        $result = $query->exec();
        $this->callPostFields($data, $query);
        
        return $result;
    }
    /**
    * Preform a delete on the table
    * 
    * @return SQLRecord
    */
    public function doDelete()
    {
        $data = $this->request[0];
        $this->validate(Field::DELETE);
        
        
        $query = SQLQuery::doDelete();
        $query->table($this);
        
        $this->callPreFields($data, $query);
        foreach($this->fields as $field)
        {
            $field->delete($query, $data);
        }
        $this->delete($query, $data);
        
        $result = $query->exec();
        $this->callPostFields($data, $query);
        
        return $result;
    }
    /**
     * Get the JavaScript for this table
     * 
     * @return string
     */
    protected function getJavascript()
    {
        $html = "<script type='text/javascript'>\n";
        foreach($this->fieldTypes as $field)
        {
            $html .= "\n".$field->getJavascript();
        }
        $html .= "</script>\n";
        
        return $html;
    }
    /**
     * Get the HTML for if there were any errors
     * 
     * @return string
     */
    protected function getErrors()
    {
        if(isset($this->error))
        {
            return "".$this->error;
        }
    }
    /**
     * Assignment field which containt the componentn and action values
     * 
     * @param int $mode
     * @return String       HTML with the hidden input fields
     */
    protected function getAssignment($mode = null)
    {
        $html = "";
        if(!empty($this->component))
        {
            $html .= "<input type='hidden' name='component' value='{$this->component}'/>\n";
        }
        if(!empty($this->action))
        {
            $html .= "<input type='hidden' name='action' value='{$this->action}'/>\n";
        }
        elseif($mode == Field::ADD || $mode == Field::DELETE || $mode == Field::EDIT)
        {
            $html .= "<input type='hidden' name='action' value='{$this->getAction($mode)}'/>\n";
        }
        
        return $html;
    }
    /**
     * Mapping of the action to the actuly hidden field content.
     * When in need of custom mapping, like if you have two tables for one component, override this method
     * 
     * @param int $mode
     * @return String       edit, delete, add
     */
    public function getAction($mode)
    {
        if(!is_numeric($mode))
        {
            $mode = $this->string2action($mode);
        }
        
        switch($mode)
        {
            case Field::EDIT:
                return "edit";
                break;
            case Field::DELETE:
                return "delete";
                break;
            case Field::ADD:
            default:
                return "add";
                break;
        }
    }
    /**
     * String to an action that the table can understand
     * 
     * @param string $string
     * @return int
     */
    protected function string2action($string)
    {
        switch($string)
        {
            case 'edit':
                return Field::EDIT;
                break;
            case 'delete':
                return Field::DELETE;
                break;
            case 'add':
            default:
                return Field::ADD;
                break;
        }
    }
    /**
     * Aditional Select data
     * 
     * @param SQLQuery $query
     * @param Object $data
     */
    abstract protected function select($query, $data = null);
    /**
     * Aditional Insert data
     * 
     * @param SQLQuery $query
     * @param Object $data
     */
    abstract protected function insert($query, $data = null);
    /**
     * Aditional Update data
     * 
     * @param SQLQuery $query
     * @param Object $data
     */
    abstract protected function update($query, $data = null);
    /**
     * Aditional Delete data
     * 
     * @param SQLQuery $query
     * @param Object $data
     */
    abstract protected function delete($query, $data = null);
    /**
     * Validate the data with the fields in this table. If not valid a InvalidFormException is thrown
     * 
     * @param int $mode
     */
    public function validate($mode = Field::ADD)
    {
        $redirect;
        
        switch($mode)
        {
            case Field::ADD:
                $redirect = $this->addURL;
                break;
            case Field::EDIT:
                $redirect = $this->editURL;
                break;
            case Field::DELETE:
                $redirect = $this->deleteURL;
                break;
            case Field::VIEW:
            default:
                $redirect = $this->adminURL;
                break;
        }
        
        $valid = true;
        $index = -1;
        
        if($this->errorRecord)
        {
            $record = $this->errorRecord;
        }
        else
        {
            $record = $this->record;
        }
        
        $exception = new InvalidFormException($record[0], "/".$redirect, $this->ID, "The form was not valid");
        
        foreach($this->fields as $field)
        {
            try
            {
                $field->validate($mode);
            }
            catch(FormException $e)
            {
                $valid = false;
                Debugger::getInstance()->log("Validation ERROR {$e->getField()->getName()} '{$e->getMessage()}'");
                $exception->addInvalidField($e->getField()->getName(), $e->getMessage());
            }
        }
        
        if(!$valid)
        {
            if($mode == Field::EDIT)
            {
                $data = array();
                
                foreach($this->keys as $keyField)
                {
                    $data[$keyField->getName()] = $keyField->getValue();
                }
                
                $exception->setRedirectData($data);
            }
            
            Debugger::getInstance()->log("Record did not validate, redirecting back");
            
            throw $exception;
        }
    }
    /**
     * Get the last inserted ID for this table. 
     * Note: this is not fetched, only kept if there was an insert. So only use
     * this if you are sure there was an insert.
     * 
     * @return int
     */
    public function getInsertedID()
    {
        return $this->lastInsertedID;
    }
    /**
     * Parse the record, this allows for data conversions or deserialization
     * 
     * @param Array $record
     * @return Array
     */
    public function db2value($record)
    {
        if(!empty($record))
        {
            foreach($record as $key => &$value)
            {
                $field = $this->getField($key);
                
                if($field)
                {
                    $value = $field->db2value($value);
                }
            }
        }
        
        return $record;
    }
    private function callPreFields(&$record, $query)
    {
        foreach($this->fields as $field)
        {
            $field->preSQL($record, $query);
        }
    }
    private function callPostFields(&$record, $query)
    {
        foreach($this->fields as $field)
        {
            $field->postSQL($record, $query);
        }
    }
}