<?php
class FormException extends Exception
{
    private $field;
    
    public function __construct($field, $message = null, $code = 0)
    {
        parent::__construct($message, $code);
        
        $this->field = $field;
    }
    
    public function getField()
    {
        return $this->field;
    }
}

/**
* @desc Exception class when a form is not valid
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
    
    public function addInvalidField($field, $message)
    {
        $this->fields[$field] = $message;
    }
    
    public function getInvalidFields()
    {
        return $this->fields;
    }
    
    public function getRecord()
    {
        return $this->record;
    }
    
    public function getRedirect()
    {
        return $this->redirect;
    }
    
    public function getTable()
    {
        return $this->table;
    }
    
    public function getRedirectData()
    {
        return $this->redirectData;
    }
    
    public function setRedirectData($redirectData)
    {
        $this->redirectData = $redirectData;
    }
}