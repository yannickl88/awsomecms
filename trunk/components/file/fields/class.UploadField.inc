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
class UploadField extends Field
{
    protected $uploadDir;
    
    /**
     * Constructor
     *
     * @param string $name
     * @param int $style
     */
    public function __construct($name, $uploadDir, $style=0)
    {
        parent::__construct($name, $style|Field::HIDE_VIEW);
        $this->uploadDir = $uploadDir;
    }
    
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_add()
     */
    protected function toHTML_add()
    {
        $this->parent->setMultipart(true);
        return "<input type='file' name='{$this->getName()}' id='{$this->getName()}'/><input type='hidden' name='{$this->getName()}_data' id='{$this->getName()}_data' value='".$this->getValue()->url."'/>";
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_edit()
     */
    protected function toHTML_edit()
    {
        return $this->toHTML_add();
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
            return unserialize($value);
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
            return serialize($value);
        }
        
        return $value;
    }
    /**
     * upload the file on edit
     * @param SQLRecord $record
     * @param SQLQuery $query
     */
    public function preSQL(&$record, $query)
    {
        if($query->getAction() == "update" || $query->getAction() == "insert")
        {
            if(!empty($_FILES[$this->getName()]['name']))
            {
                $record2 = $this->parent->getRecord();
                $data = $record2[0]->{$this->getName()."_data"};
                $fileData = $_FILES[$this->getName()];
                $newData = new stdClass();
                
                if($data && file_exists(Config::get("websiteroot", "/")."/".$data))
                {
                    unlink(Config::get("websiteroot", "/")."/".$data);
                }
                
                $filename = md5($fileData['name'].time())."-".$fileData['name'];
                
                $newData->filename = $filename;
                $newData->name = $fileData['name'];
                $newData->type = $fileData['type'];
                $newData->size = $fileData['size'];
                $newData->url = $this->handleFileUpload($filename, $fileData['tmp_name']);
                $this->setValue($newData);
            }
            else
            {
                $this->setValue("");
            }
        }
        else if($query->getAction() == "delete")
        {
            $data = $this->parent->doSelect()->getRows();
            
            foreach($data as $row)
            {
                if($row && file_exists(Config::get("websiteroot", "/")."/".$row->{$this->getDBName()}->url))
                {
                    unlink(Config::get("websiteroot", "/")."/".$row->{$this->getDBName()}->url);
                }
            }
        }
    }
    private function handleFileUpload($filename, $tmpUrl)
    {
        $newUrl = $this->uploadDir.$this->parent->getName()."/".$filename;
        
        if(!file_exists(Config::get("websiteroot", "/")."/".$this->uploadDir.$this->parent->getName()))
        {
            mkdir(Config::get("websiteroot", "/")."/".$this->uploadDir.$this->parent->getName(), 0777, true);
        }
        
        move_uploaded_file($tmpUrl, Config::get("websiteroot", "/")."/".$newUrl);
        
        return $newUrl;
    }
    
    public function validate($mode)
    {
        ;
    }
    
    public function isRequired($mode)
    {
        if($mode == Field::ADD && $this->hasFlag(Field::OPTIONAL_ADD))
        {
            return false;
        }
        elseif($mode == Field::EDIT)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
}