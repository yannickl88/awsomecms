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
    protected $errorRecord;
    protected $errorFields;
    protected $title;
    
    protected $component;
    protected $action;
    
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
        $this->fields[] = $field;
        
        $class = get_class($field);
        if(!isset($this->fieldTypes[$class]))
        {
            $this->fieldTypes[$class] = $field->getJavascript();
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
    * @param array $data
    * @return Table
    */
    public function setRecord($data)
    {
        if(!isset($this->errorRecord) && !empty($data))
        {
            if(!is_array($data))
            {
                $this->record = array($data);
            }
            else
            {
                $this->record = $data;
            }
            
            foreach($this->fields as $field)
            {
                $field->setData($this->record[0]);
            }
        }
        
        return $this;
    }
    /**
    * Set the current error record of the table, this has priority over the current one
    * 
    * @param array $data
    * @param array $fields
    * @return Table
    */
    public function setErrorRecord($data, $fields)
    {
        if(is_array($data))
        {
            $this->errorRecord = array((object) $data);
        }
        else
        {
            $this->errorRecord = array($data);
        }
        
        $this->errorFields = $fields;
        
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
        $html = $this->getErrors();
        $html .= "<form method=\"post\" action=\"/\" class=\"admin_form admin_add\">\n";
        
        foreach($this->fields as $field)
        {
            if($field->visible(Field::ADD))
            {
                $html .= "<div class=\"admin_form_row\">\n";
                $html .= "<div class=\"admin_form_label\"><label for=\"".$field->getName()."\">".$field->getDisplayName().":</label></div>\n";
                $html .= "<div class=\"admin_form_field\">".$field->toHTML(Field::ADD)."</div>\n";
                $html .= "</div>\n";
            }
        }
        $html .= "<div class=\"admin_form_row admin_form_submit\">\n";
        $html .= "<input type='submit' value='Save' />\n";
        $html .= "</div>\n";
        $html .= $this->getAssignment(Field::ADD);
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
        $data = $this->doSelect($this->record[0])->getRow();
        
        $html_post = "";
        $html = $this->getErrors();
        $html .= "<form method=\"post\" action=\"/\" class=\"admin_form admin_edit\">\n";
        
        foreach($this->fields as $field) //@var $field Field
        {
            $field->setData($data);
            
            if($field->visible(Field::EDIT))
            {
                $html .= "<div class=\"admin_form_row\">\n";
                $html .= "<div class=\"admin_form_label\"><label for=\"".$field->getName()."\">".$field->getDisplayName().":</label></div>\n";
                $html .= "<div class=\"admin_form_field\">".$field->toHTML(Field::EDIT)."</div>\n";
                $html .= "</div>\n";
            }
            elseif($field->isIndex())
            {
                $html_post .= $field->toHTML(Field::EDIT)."\n";
            }
        }
        $html .= "<div class=\"admin_form_row admin_form_submit\">\n";
        $html .= "<input type='submit' value='Save' />\n";
        $html .= "</div>\n";
        $html .= $html_post;
        $html .= $this->getAssignment(Field::EDIT);
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
        $data = $this->doSelect($this->record[0])->getRows();
        
        $html = "<table class=\"admin_table\">\n";
        $html .= "    <tr class=\"admin_table_row\">\n";
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
            $html .= "            <a href='/{$this->editURL}{$queryString}'><img src=\"/img/icons/edit.png\" alt=\"edit\"></a>";
            $html .= "            <a href='/{$this->deleteURL}{$queryString}'><img src=\"/img/icons/delete.png\" alt=\"delete\"</a>\n";
            $html .= "        </td>\n";
            $html .= "        <td class=\"admin_table_cell admin_table_actions\">\n";
            $html .= "            <input type=\"checkbox\" value=\"".$this->keys[0]->getValue($row)."\" name=\"action[]\" class=\"actioncheckbox\">\n";
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
            $html .= "        <td class=\"admin_table_cell\" colspan=\"{$colspan}\">No Records</td>\n";
        }
        
        $colspan = $cols + 1;
        $html .= "    <tr class=\"admin_table_row\">\n";
        $html .= "        <td class=\"admin_table_cell admin_table_actions\" colspan=\"{$colspan}\">\n";
        $html .= "            <img src=\"/img/icons/add.png\" alt=\"add\"> <a href=\"/{$this->addURL}\">Add</a>\n";
        $html .= "        </td>\n";
        $html .= "        <td class=\"admin_table_cell\">\n";
        $html .= "            <a href=\"javascript: void(0);\" onclick=\"deleteMultiple('/{$this->deleteURL}', '{$this->keys[0]->getName()}', $('.actioncheckbox'));\"><img src=\"/img/icons/delete.png\" alt=\"delete selected\"></a>\n";
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
        $data = $this->doSelect($this->record[0])->getRows();
        
        $html = "";
        $post_html = "";
        $html .= "<form method=\"post\" action=\"/\" class=\"admin_form admin_delete\">\n";
        $html .= "    <div class=\"admin_form_row\">\n";
        $html .= "        <div class=\"admin_form_text\">\n";
        $html .= "            Are you sure you want to delete this item? <br />\n";
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
            $html .= "                <li><img src=\"/img/icons/page.png\" alt=\"\">{$title}</li>\n";
            
            foreach($this->keys as $field)
            {
                $post_html .= "    <input type=\"hidden\" name=\"{$field->getName()}[]\" value=\"{$field->getValue($row)}\" />\n";
            }
        }
        
        $html .= "            </ul>\n";
        $html .= "        </div>\n";
        $html .= "    </div>\n";
        $html .= "    <div class=\"admin_form_row admin_form_submit\">\n";
        $html .= "        <input type=\"submit\" value=\"Delete\" id=\"page_submit\" class=\"admin_form_submit\">\n";
        $html .= "    </div>\n";
        $html .= $post_html;
        $html .= $this->getAssignment(Field::DELETE);
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
            $title = str_replace("[{$match}]", $this->getField($match)->getValue($data), $title);
        }
        
        return $title;
    }
    /**
    * Preform a select on the table
    * 
    * @return SQLRecord
    */
    public function doSelect($data = array())
    {
        $this->validate(Field::VIEW);
        
        $query = SQLQuery::doSelect();
        $query->table($this->name);
        
        foreach($this->fields as $field)
        {
            $field->select($query, $data);
        }
        
        $return = $this->select($query, $data);
        
        if(isset($return))
        {
            return $return;
        }
        
        return $query->exec();
    }
    /**
    * Preform an insert on the table
    * 
    * @return int, last inserted ID
    */
    public function doInsert($data = array())
    {
        $this->validate(Field::ADD);
        
        $query = SQLQuery::doInsert();
        $query->table($this->name);
        
        foreach($this->fields as $field)
        {
            $field->insert($query, $this->record[0]);
        }
        
        $return = $this->insert($query, $data);
        
        if(isset($return))
        {
            return $return;
        }
        
        return $query->exec()->insertID();
    }
    /**
    * Preform an update on the table
    * 
    * @return SQLRecord
    */
    public function doUpdate($data = array())
    {
        $this->validate(Field::EDIT);
        
        $query = SQLQuery::doUpdate();
        $query->table($this->name);
        
        foreach($this->fields as $field)
        {
            $field->update($query, $this->record[0]);
        }
        
        $return = $this->update($query, $data);
        
        if(isset($return))
        {
            return $return;
        }
        
        return $query->exec();
    }
    /**
    * Preform a delete on the table
    * 
    * @return SQLRecord
    */
    public function doDelete($data = array())
    {
        $this->validate(Field::DELETE);
        
        $query = SQLQuery::doDelete();
        $query->table($this->name);
        
        foreach($this->fields as $field)
        {
            $field->delete($query, $this->record[0]);
        }
        $return = $this->delete($query, $data);
        
        if(isset($return))
        {
            return $return;
        }
        
        return $query->exec();
    }
    /**
     * Get the JavaScript for this table
     * 
     * @return string
     */
    protected function getJavascript()
    {
        $html = "<script type='text/javascript'>\n";
        $html .= implode("\n", $this->fieldTypes);
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
        if(isset($this->errorFields))
        {
            $html = "<div class=\"form_error\">\n";
            $html .= "    The form was not filled in correctly. Please check the following fields:\n";
            $html .= "    <ul id=\"errorlist\">\n";
            $html .= "    </ul>\n";
            $html .= "</div>\n";
            $html .= "<script>\n";
            foreach($this->errorFields as $field => $error)
            {
                $html .= "        addFieldError('{$field}', '{$error}');\n";
            }
            $html .= "</script>\n";
            
            return $html;
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
        
        $exception = new InvalidFormException($this->record, "/".$redirect, $this->ID, "The form was not valid");
        
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
}