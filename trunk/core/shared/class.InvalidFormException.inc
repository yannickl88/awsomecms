<?php
/**
* @desc Exception class when a form is not valid
*/
class InvalidFormException extends Exception
{
    private $fields;
    private $record;
    private $redirect;
    private $redirectData;
    
    public function __construct($record, $redirect, $message = null, $code = 0)
    {
        parent::__construct($message, $code);
        
        $this->fields = array();
        $this->record = $record;
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
    
    public function getRedirectData()
    {
        return $this->redirectData;
    }
    
    public function setRedirectData($redirectData)
    {
        $this->redirectData = $redirectData;
    }
}