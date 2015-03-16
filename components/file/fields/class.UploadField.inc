<?php
/**
 * Field for uploading files
 *
 * @author Yannick <yannick.l.88@gmail.com>
 */
class UploadField extends Field
{
    protected $uploadDir;
    protected $resizeTo = null;

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
    public function setResize($to)
    {
        $this->resizeTo = $to;
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_edit()
     */
    protected function toHTML_edit()
    {
        $value = $this->getValue();
        $info = $preview = $valueUrl = "";

        if(empty($value->url))
            $info = Language::get("nofile");
        else
        {
            $c = Component::init("file");

            switch ($c->getType($value->type))
            {
                case "images":
                    $info .= "<img src='/img/icons/image.png' alt='image'/>";
                    $preview = "<img src='{$value->url}' alt='preview' style='max-height: 150px;'/><br />";
                    break;
                case "vid":
                    $info .= "<img src='/img/icons/vid.png' alt='video'/>";
                    break;
                case "doc":
                    $info .= "<img src='/img/icons/doc.png' alt='document'/>";
                    break;
                default:
                    $info .= "<img src='/img/icons/other.png' alt='other'/>";
                    break;
            }
            $info .= " ".$value->url;
            $valueUrl = $value->url;
        }

        $delete = "<input type='checkbox' name='{$this->getName()}_delete' />".Language::get("delete");

        return $preview.$info."<br />".$this->toHTML_add()."<input type='hidden' name='{$this->getName()}_data' id='{$this->getName()}_data' value='".$valueUrl."'/>".$delete;
    }
    /**
     * (non-PHPdoc)
     * @see Field::toHTML_view()
     */
    protected function toHTML_view()
    {
        $value = $this->getValue();
        $info = $preview = $valueUrl = "";

        if(empty($value->url))
            $info = Language::get("nofile");
        else
        {
            $c = Component::init("file");

            switch ($c->getType($value->type))
            {
                case "images":
                    $info .= "<img src='/img/icons/image.png' alt='image'/>";
                    $preview = "<img src='{$value->url}' alt='preview' style='max-height: 150px;'/><br />";
                    break;
                case "vid":
                    $info .= "<img src='/img/icons/vid.png' alt='video'/>";
                    break;
                case "doc":
                    $info .= "<img src='/img/icons/doc.png' alt='document'/>";
                    break;
                default:
                    $info .= "<img src='/img/icons/other.png' alt='other'/>";
                    break;
            }
            $info .= " ".$value->url;
            $valueUrl = $value->url;
        }

        return $preview.$info;
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
        $record2 = $this->parent->getRecord();

        if($query->getAction() == "delete" || (($query->getAction() == "update" || $query->getAction() == "insert") && $record2[0]->{$this->getName()."_delete"} == "on"))
        {
            $data = $this->parent->doSelect()->getRows();

            foreach($data as $row)
            {
                if($row && isset($row->{$this->getDBName()}->url) && file_exists(Config::get("websiteroot", "/")."/".$row->{$this->getDBName()}->url))
                {
                    unlink(Config::get("websiteroot", "/")."/".$row->{$this->getDBName()}->url);
                }
                if($row && isset($row->{$this->getDBName()}->thumb) && file_exists(Config::get("websiteroot", "/")."/".$row->{$this->getDBName()}->thumb))
                {
                    unlink(Config::get("websiteroot", "/")."/".$row->{$this->getDBName()}->thumb);
                }
            }

            $this->setValue("");
            $this->style = $this->style | Field::FORCEUPDATE;
        }

        if($query->getAction() == "update" || $query->getAction() == "insert")
        {
            if(is_object($this->getValue())) return;

            if(!empty($_FILES[$this->getName()]['name']))
            {
                $data = $record2[0]->{$this->getName()."_data"};
                $fileData = $_FILES[$this->getName()];
                $newData = new stdClass();

                if($query->getAction() == "update") {
                    //delete old images
                    $row = $this->parent->doSelect()->getRow();

                    if($row && isset($row->{$this->getDBName()}->url) && file_exists(Config::get("websiteroot", "/")."/".$row->{$this->getDBName()}->url))
                    {
                        unlink(Config::get("websiteroot", "/")."/".$row->{$this->getDBName()}->url);
                    }
                    if($row && isset($row->{$this->getDBName()}->url) && file_exists(Config::get("websiteroot", "/")."/".$row->{$this->getDBName()}->thumb))
                    {
                        unlink(Config::get("websiteroot", "/")."/".$row->{$this->getDBName()}->thumb);
                    }
                //delete old images
                $row = $this->parent->doSelect()->getRow();

                if($row && isset($row->{$this->getDBName()}->url) && file_exists(Config::get("websiteroot", "/")."/".$row->{$this->getDBName()}->url))
                {
                    unlink(Config::get("websiteroot", "/")."/".$row->{$this->getDBName()}->url);
                }
                if($row && isset($row->{$this->getDBName()}->url) && file_exists(Config::get("websiteroot", "/")."/".$row->{$this->getDBName()}->thumb))
                {
                    unlink(Config::get("websiteroot", "/")."/".$row->{$this->getDBName()}->thumb);
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
    }
    private function handleFileUpload($filename, $tmpUrl)
    {
        $newUrl = $this->uploadDir.$this->parent->getName()."/".$filename;

        if(!file_exists(Config::get("websiteroot", "/")."/".$this->uploadDir.$this->parent->getName()))
        {
            mkdir(Config::get("websiteroot", "/")."/".$this->uploadDir.$this->parent->getName(), 0777, true);
        }

        $newLoc = Config::get("websiteroot", "/")."/".$newUrl;

        //check if we need to resize
        if($this->resizeTo != null)
        {
            $src = $this->resize($tmpUrl, $this->resizeTo, false);

            if($src)
            {
                imagepng($src, $newLoc);
                imagedestroy($src);
            }
        }
        else
        {
            move_uploaded_file($tmpUrl, $newLoc);
        }

        //create thumb file
        $thumb = $this->createThumb($filename, $newLoc, Config::get("crop", true, "file"));

        return array($newUrl, $thumb);
    }
    private function calcBoundingBox($imageSize, $maxSize)
    {
        $ratio = $imageSize[0] / $imageSize[1];

        if(is_numeric($maxSize[0]) && $imageSize[0] > $maxSize[0]) {
            $imageSize[0] = $maxSize[0];
            $imageSize[1] = floor($maxSize[0] / $ratio);
        }

        if(is_numeric($maxSize[1]) && $imageSize[1] > $maxSize[1]) {
            $imageSize[1] = $maxSize[1];
            $imageSize[0] = floor($maxSize[1] * $ratio);
        }

        return $imageSize;
    }
    private function resize($fileLoc, $to, $crop = true)
    {
        $info = getimagesize($fileLoc);

        if(in_array($info[2], array(1,2,3,6)))
        {
            $src = null;

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

            //make a resize version to crop from
            $newSrc = null;

            if($crop)
            {
                $newSrc = imagecreatetruecolor($to[0], $to[1]);
                imagealphablending( $newSrc, false );
                imagesavealpha( $newSrc, true );
                $rInfo = $this->calcBoundingBox($info, array(400, 400));

                $tmpSrc = imagecreatetruecolor($rInfo[0], $rInfo[1]);

                imagecopyresampled($tmpSrc, $src, 0, 0, 0, 0, $rInfo[0], $rInfo[1], $info[0], $info[1]);
                imagecopy($newSrc, $tmpSrc, 0, 0, floor($rInfo[0]/2 - ($to[0]/2)), floor($rInfo[1]/2 - ($to[1]/2)), $rInfo[0], $rInfo[1]);
                imagedestroy($tmpSrc);
            }
            else
            {
                $rInfo = $this->calcBoundingBox($info, $to);
                $newSrc = imagecreatetruecolor($rInfo[0], $rInfo[1]);
                imagealphablending( $newSrc, false );
                imagesavealpha( $newSrc, true );

                imagecopyresampled($newSrc, $src, 0, 0, 0, 0, $rInfo[0], $rInfo[1], $info[0], $info[1]);
            }

            imagedestroy($src);
            return $newSrc;
        }

        return false;
    }

    private function createThumb($filename, $fileLoc, $crop = true)
    {
        $newUrl = $this->uploadDir.$this->parent->getName()."/".substr($filename, 0, strrpos($filename, "."))."_thumb".substr($filename, strrpos($filename, "."));
        $maxDimentions = explode("x", Config::get("thumbsize", "100x100", "file"));
        $newSrc = $this->resize($fileLoc, $maxDimentions, $crop);

        if($newSrc)
        {
            imagepng($newSrc, Config::get("websiteroot", "/").$newUrl);
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