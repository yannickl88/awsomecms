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

/**
 * Exception to be triggered when a field is not validly filled in
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class FormException extends Exception
{
    private $field;
    
    public function __construct($field, $message = null, $code = 0)
    {
        parent::__construct($message, $code);
        
        $this->field = $field;
    }
    /**
     * Get the field that were not valid
     * 
     * @return string
     */
    public function getField()
    {
        return $this->field;
    }
}

/**
* Exception class when a form is not valid
* 
* @author Yannick <yannick.l.88@gmail.com>
*/
class InvalidFormException extends Exception
{
    private $fields;
    private $record;
    private $table;
    private $redirect;
    private $redirectData;
    
    public function __construct($record, $redirect, $table, $message = null, $code = 0)
    {
        parent::__construct($message, $code);
        
        $this->fields = array();
        $this->record = $record;
        $this->table = $table;
        $this->redirect = $redirect;
        $this->redirectData = array();
    }
    /**
     * Add field that is was not valid
     * 
     * @param string $field
     * @param string $message
     */
    public function addInvalidField($field, $message)
    {
        $this->fields[$field] = $message;
    }
    /**
     * Get the fields that were not valid
     * 
     * @return array
     */
    public function getInvalidFields()
    {
        return $this->fields;
    }
    /**
     * Get the record that was not valid
     * 
     * @return object
     */
    public function getRecord()
    {
        return $this->record;
    }
    /**
     * Get the page that should be redirected to
     * 
     * @return string
     */
    public function getRedirect()
    {
        return $this->redirect;
    }
    /**
     * Get the table that the fields belong to
     */
    public function getTable()
    {
        return $this->table;
    }
    /**
     * Get the data that should be passed when redirecting
     * 
     * @return array
     */
    public function getRedirectData()
    {
        return $this->redirectData;
    }
    /**
     * Set the data that should be passed when redirecting
     * 
     * @param array $redirectData
     */
    public function setRedirectData($redirectData)
    {
        $this->redirectData = $redirectData;
    }
    
    public function __toString()
    {
        $html = "";
        $html .= "<div class=\"form_error\">\n";
        $html .= "    ".Language::get("formerror").":\n";
        $html .= "    <ul id=\"errorlist\">\n";
        $html .= "    <li></li>\n";
        $html .= "    </ul>\n";
        $html .= "</div>\n";
        $html .= "<script type=\"text/javascript\">\n";
        $html .= "        $(\"#errorlist\").ready(function(e) {\n";
        $html .= "            $('html, body').animate({scrollTop: $(\"#errorlist\").position().top}, \"slow\");\n";
        $html .= "            $(\"#errorlist\").empty();\n";
        foreach($this->fields as $field => $error)
        {
            $html .= "            addFieldError('{$field}', '{$error}');\n";
        }
        
        foreach($this->record as $field => $value)
        {
            if($field != "method")
            {
                $html .= "            setFieldValue('{$field}', \"".htmlentities("".$value, ENT_QUOTES)."\");\n";
            }
        }
        $html .= "        });";
        $html .= "</script>\n";
        
        return $html;
    }
}