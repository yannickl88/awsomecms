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
        return "<input type='file' name='{$this->getName()}' id='{$this->getName()}'/>";
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_edit()
     */
    protected function toHTML_edit()
    {
        return $this->toHTML_add()."<input type='hidden' name='{$this->getName()}_data' id='{$this->getName()}_data' value='".$this->getValue()->url."'/>";
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
                list($newData->url, $newData->thumb) = $this->handleFileUpload($filename, $fileData['tmp_name']);
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
                if($row && file_exists(Config::get("websiteroot", "/")."/".$row->{$this->getDBName()}->thumb))
                {
                    unlink(Config::get("websiteroot", "/")."/".$row->{$this->getDBName()}->thumb);
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
        
        $newLoc = Config::get("websiteroot", "/")."/".$newUrl;
        
        move_uploaded_file($tmpUrl, $newLoc);
        
        //create thumb file
        $thumb = $this->createThumb($filename, $newLoc);
        
        return array($newUrl, $thumb);
    }
    private function createThumb($filename, $fileLoc)
    {
        $info = getimagesize($fileLoc);

        if(in_array($info[2], array(1,2,3,6)))
        {
            $src = null;
            $newUrl = $this->uploadDir.$this->parent->getName()."/".substr($filename, 0, strrpos($filename, "."))."_thumb".substr($filename, strrpos($filename, "."));
            $maxDimentions = array(100, 100);
            
            switch($info[2])
            {
                case 1 :
                    $src = imagecreatefromgif($fileLoc);
                    break;
                case 2 :
                    $src = imagecreatefromjpeg($fileLoc);
                    break;
                case 3 :
                    $src = imagecreatefrompng($fileLoc);
                    imagesavealpha($src, true);
                    break;
                default :
                case 6 :
                    $src = imagecreatefromwbmp($fileLoc);
                    break;
                default :
                    return false;
                    break;
            }
            
            $ratio = $info[0]/$info[1];
            
            //make a resize version to crop from
            $newSrc = imagecreatetruecolor($maxDimentions[0], $maxDimentions[1]);
            if($info[0] > 400)
            {
                $tmpSrc = imagecreatetruecolor(400, 400*$ratio);
                imagecopyresampled($tmpSrc, $src, 0, 0, 0, 0, 400, 400*$ratio, $info[0], $info[1]);
                imagecopy($newSrc, $tmpSrc, 0, 0, (200 - ($maxDimentions[0]/2)), (200 - ($maxDimentions[1]/2)), 400, 400);
                imagedestroy($tmpSrc);
            }
            else
            {
                imagecopy($newSrc, $src, 0, 0, (($info[0]/2) - ($maxDimentions[0]/2)), ($info[1]/2 - ($maxDimentions[1]/2)), $info[0], $info[1]);
            }
            
            imagepng($newSrc, Config::get("websiteroot", "/").$newUrl);
            
            imagedestroy($src);
            imagedestroy($newSrc);
            
            return $newUrl;
        }
        
        return false;
    }
    
    public function validate($mode)
    {
        if(!empty($_FILES[$this->getName()]['name']) && (int) $_FILES[$this->getName()]['error'] > 0)
        {
            $msg = "";
            switch($_FILES[$this->getName()]['error'])
            {
                case 1:
                case 2:
                    $msg = Language::get("fileerror_1");
                    break;
                case 3:
                    $msg = Language::get("fileerror_2");
                    break;
                case 4:
                    $msg = Language::get("fileerror_3");
                    break;
                case 5:
                    $msg = Language::get("fileerror_4");
                    break;
                case 6:
                    $msg = Language::get("fileerror_5");
                    break;
                default:
                    $msg = Language::get("fileerror_6");
                    break;
            }
            
            throw new FormException($this, $msg);
        }
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