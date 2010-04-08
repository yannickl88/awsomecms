<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.gallery
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */
import('/core/class.Component.inc');
import('/components/gallery/util/class.ImageTree.inc');

/**
 * A simple component to manage and show a series of images on a page. Using a tag, images can be grouped together
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class GalleryComponent extends Component
{
    /**
     * Constructor
     * 
     * @return unknown_type
     */
    public function __construct()
    {
        parent::__construct('gallery');
    }
    /**
     * Action to add an image
     * 
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_add($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            $this->validate($request, "/".Config::get('galleryadd', 'galleryadd', 'admin'), "add");
            
            $request["image_url"] = "";
            $request["image_urlthumb"] = "";
            $request["image_width"] = 0;
            $request["image_height"] = 0;
            
            $id = Table::init("gallery.images")
                ->setRecord((object) $request)
                ->doInsert();
            
            $request["image_id"] = $id;
            
            list($request["image_url"], $request["image_width"], $request["image_height"]) = $this->handleFileUpload($id, 'image_image');
            
            if($request['image_tumb_generate'] == 'on')
            {
                list($request["image_urlthumb"], $request["image_thumbwidth"], $request["image_thumbheight"]) = $this->createThumb($id, $request["image_url"]);
            }
            else
            {
                list($request["image_urlthumb"], $request["image_thumbwidth"], $request["image_thumbheight"]) = $this->handleThumbUpload($id, 'image_thumb');
            }
            
            if(!$request["image_urlthumb"])
            {
                $exception = new InvalidFormException((object) $request, Config::get('contactadd', 'contactadd', 'admin'), "gallery.images", "The form was not valid");
                $exception->addInvalidField($field, Language::get("error_notfilledin"));
                die("ERROR");
            }
            
            Table::init("gallery.images")
                ->setRecord((object) $request)
                ->doUpdate();
        }
        
        $this->redirect("/".Config::get('galleryadmin', 'galleryadmin', 'admin'));
    }
    /**
     * Action to edit an image
     * 
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_edit($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            $this->validate($request, "/".Config::get('galleryedit', 'galleryedit', 'admin'), "edit");
            
            $record = Table::init("gallery.images")
                ->setRequest((object) $request)
                ->doSelect()
                ->getRows();
            
            $id = $request['image_id'];
            
            if($_FILES['image_image'] && !empty($_FILES['image_image']['tmp_name']))
            {
                //remove old one
                if($record[0]->image_url != '' && file_exists(Config::get('websiteroot', '.').$record[0]->image_url) && is_file(Config::get('websiteroot', '.').$record[0]->image_url))
                {
                    @unlink(Config::get('websiteroot', '.').'/'.$record[0]->image_url);
                }
                
                //upload new one
                list($request["image_url"], $request["image_width"], $request["image_height"]) = $this->handleFileUpload($id, 'image_image');
                
                if($request['image_tumb_generate'] == 'on')
                {
                    if($record[0]->image_urlthumb != '' && file_exists(Config::get('websiteroot', '.').$record[0]->image_urlthumb) && is_file(Config::get('websiteroot', '.').$record[0]->image_urlthumb))
                    {
                        @unlink(Config::get('websiteroot', '.').$record[0]->image_urlthumb);
                    }
                    list($request["image_urlthumb"], $request["image_thumbwidth"], $request["image_thumbheight"]) = $this->createThumb($id, $request["image_url"]);
                }
            }
            
            if($_FILES['image_thumb'])
            {
                //remove old one
                if(file_exists(Config::get('websiteroot', '.').'/'.Config::get('uploaddir', 'img/upload/', 'gallery').$record[0]->image_urlthumb) && is_file(Config::get('websiteroot', '.').$record[0]->image_urlthumb))
                {
                    unlink(Config::get('websiteroot', '.').'/'.Config::get('uploaddir', 'img/upload/', 'gallery').$record[0]->image_urlthumb);
                }
                
                //upload new one
                list($request["image_urlthumb"], $request["image_thumbwidth"], $request["image_thumbheight"]) = $this->handleThumbUpload($id, 'image_thumb');
            }
            
            Table::init("gallery.images")
                ->setRecord((object) $request)
                ->doUpdate();
            
            $this->redirect("/".Config::get('galleryadmin', 'galleryadmin', 'admin'));
        }
        else
        {
            if($request['image_id'])
            {
                $record = Table::init("gallery.images")
                    ->setRequest((object) $request)
                    ->doSelect()
                    ->getRow();
                
                $smarty->assign("record", $record);
            }
            else
            {
                $this->redirect("/".Config::get('galleryadmin', 'galleryadmin', 'admin'));
            }
        }
    }
    /**
     * Action to delete an image
     * 
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_delete($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            $records = Table::init("gallery.images")
                ->setRequest((object) $request)
                ->doSelect()
                ->getRows();
            
            //delete for when miltiple images were given
            foreach($records as $record)
            {
                //remove old one
                if(file_exists(Config::get('websiteroot', '.').$record->image_url))
                {
                    @unlink(Config::get('websiteroot', '.').'/'.$record->image_url);
                }
                if(file_exists(Config::get('websiteroot', '.').$record->image_urlthumb))
                {
                    @unlink(Config::get('websiteroot', '.').$record->image_urlthumb);
                }
            }
            
            Table::init("gallery.images")
                ->setRequest((object) $request)
                ->doDelete();
            
            $this->redirect("/".Config::get('galleryadmin', 'galleryadmin', 'admin'));
        }
        else
        {
            if($request['image_id'])
            {
                $record = Table::init("gallery.images")
                    ->setRequest((object) $request)
                    ->doSelect()
                    ->getRows();
                
                $smarty->assign("record", $record);
            }
            else
            {
                $this->redirect("/".Config::get('galleryadmin', 'galleryadmin', 'admin'));
            }
        }
    }
    /**
     * Upload a file to the server
     * 
     * @param int $id       Image id
     * @param string $name
     * @return array
     */
    private function handleFileUpload($id, $name)
    {
        $file = $_FILES[$name]['name'];
        $fileExtention = strtolower(substr($file, strrpos($file, '.')+1));
        $fileName = basename($file, '.'.$fileExtention);
        $fileSize = $_FILES[$name]['size'];
        $maxSize = Config::get('maxsize', -1, 'gallery');
        $allowedExtentions = explode(",", Config::get('extentions', 'jpg,gif,png', 'gallery'));
        $destName = $id.'-'.$fileName.'.'.$fileExtention;
        $destFile = realpath(Config::get('websiteroot', '.').'/'.Config::get('uploaddir', 'img/upload/', 'gallery')).'/'.$destName;
        
        if(($maxSize == -1 || $fileSize <= $maxSize) && in_array($fileExtention, $allowedExtentions))
        {
            move_uploaded_file($_FILES[$name]['tmp_name'], $destFile);
            
            $info = getimagesize($destFile);
            
            return array('/'.Config::get('uploaddir', 'img/upload/', 'gallery').$destName, $info[0], $info[1]);
        }
        else
        {
            return false;
        }
    }
    /**
     * Handle thumb upload
     * 
     * @param int $id       Image id
     * @param string $name
     * @return array
     */
    private function handleThumbUpload($id, $name)
    {
        $file = $_FILES[$name]['name'];
        $fileExtention = strtolower(substr($file, strrpos($file, '.')+1));
        $fileName = basename($file, '.'.$fileExtention);
        $fileSize = $_FILES[$name]['size'];
        $maxSize = Config::get('maxsize', -1, 'gallery');
        $allowedExtentions = explode(",", Config::get('extentions', 'jpg,gif,png', 'gallery'));
        $destName = $id.'-'.$fileName.'_thumb.'.$fileExtention;
        $destFile = realpath(Config::get('websiteroot', '.').'/'.Config::get('uploaddir', 'img/upload/', 'gallery')).'/'.$destName;
        $maxDimentions = explode('x', Config::get('tumbsize', "100x50", 'gallery'));
        $tempInfo = getimagesize($_FILES[$name]['tmp_name']);
        
        if(($maxSize == -1 || $fileSize <= $maxSize) && ($tempInfo[0] <= $maxDimentions[0] && $tempInfo[1] <= $maxDimentions[1]) && in_array($fileExtention, $allowedExtentions))
        {
            move_uploaded_file($_FILES[$name]['tmp_name'], $destFile);
            
            $info = getimagesize($destFile);
            
            return array('/'.Config::get('uploaddir', 'img/upload/', 'gallery').$destName, $info[0], $info[1]);
        }
        else
        {
            return false;
        }
    }
    /**
     * Generate the thumb from an image
     * 
     * @param int $id       Image ID
     * @param string $originalUrl
     * @return array
     */
    private function createThumb($id, $originalUrl)
    {
        $file = basename($originalUrl);
        $fileExtention = strtolower(substr($file, strrpos($file, '.')+1));
        $fileName = basename($file, '.'.$fileExtention);
        $destName = $fileName.'_thumb.'.$fileExtention;
        $destFile = realpath(Config::get('websiteroot', '.').'/'.Config::get('uploaddir', 'img/upload/', 'gallery')).'/'.$destName;
        $maxDimentions = explode('x', Config::get('tumbsize', "100x50", 'gallery'));
        $info = getimagesize(Config::get('websiteroot', '.').$originalUrl);
        
        switch($fileExtention)
        {
            case "jpg" :
                $src = imagecreatefromjpeg(Config::get('websiteroot', '.').$originalUrl);
                break;
            case "gif" :
                $src = imagecreatefromgif(Config::get('websiteroot', '.').$originalUrl);
                break;
            case "png" :
                $src = imagecreatefrompng(Config::get('websiteroot', '.').$originalUrl);
                break;
            default :
                return false;
                break;
        }
        
        $dest = imagecreatetruecolor($maxDimentions[0], $maxDimentions[1]);

        imagecopy($dest, $src, 0, 0, (($info[0]/2) - ($maxDimentions[0]/2)), (($info[1]/2) - ($maxDimentions[1]/2)), $info[0], $info[1]);

        switch($fileExtention)
        {
            case "jpg" :
                imagejpeg($dest, $destFile);
                break;
            case "gif" :
                imagegif($dest, $destFile);
                break;
            case "png" :
                imagepng($dest, $destFile);
                break;
            default :
                return false;
                break;
        }

        imagedestroy($dest);
        imagedestroy($src);
        
        $destInfo = getimagesize($destFile);
            
        return array('/'.Config::get('uploaddir', 'img/upload/', 'gallery').$destName, $destInfo[0], $destInfo[1]);
    }
    /**
     * Custom validation to check if the image was correcly uploaded
     * 
     * @param array $data
     * @param string $redirectOnFailure
     * @param string $mode
     */
    protected function validate($data, $redirectOnFailure, $mode = "edit")
    {
        $valid = true;
        $index = -1;
        $exception = new InvalidFormException((object) $data, $redirectOnFailure, "gallery.images", "The form was not valid");
        if(isset($data["image_id"]))
        {
            $exception->setRedirectData(array("image_id" => $data["image_id"]));
        }
        
        //check if the file did actully upload
        if($_FILES['image_image']['error'] > 0) //uh-oh error
        {
            if($mode == "edit" && $_FILES['image_image']['error'] != 4)
            {
                $error = "";
                
                switch($_FILES['image_image']['error'])
                {
                    case 1:
                    case 2:
                    case 3:
                        $error = Language::get("error_filetolarge");
                        break;
                    case 4:
                        $error = Language::get("error_nofileselect");
                        break;
                    case 5:
                        $error = Language::get("error_cennotwrite");
                        break;
                }
                
                $exception->addInvalidField('image_image', $error);
                throw $exception;
            }
        }
        
        //check if the file did actully upload
        if($data['image_tumb_generate'] != 'on' && $_FILES['image_thumb']['error'] > 0) //uh-oh error
        {
            if($mode == "edit" && $_FILES['image_thumb']['error'] != 4)
            {
                $error = "";
                
                switch($_FILES['image_thumb']['error'])
                {
                    case 1:
                    case 2:
                    case 3:
                        $error = "Filesize is to large";
                        break;
                    case 4:
                        $error = "No file selected";
                        break;
                    case 5:
                        $error = "Cannot write file to disk";
                        break;
                }
                
                $exception->addInvalidField('image_thumb', $error);
                throw $exception;
            }
        }
        
        if($data['image_tumb_generate'] != 'on')
        {
            //check the tumb dimentions if set
            $maxDimentions = explode('x', Config::get('tumbsize', "100x50", 'gallery'));
            $tempInfo = getimagesize($_FILES['image_thumb']['tmp_name']);
            
            if($tempInfo[0] > $maxDimentions[0] || $tempInfo[1] > $maxDimentions[1])
            {
                $exception->addInvalidField('image_thumb', "Tumb image was to large");
                throw $exception;
            }
        }
    }
    /**
     * (non-PHPdoc)
     * @see core/Component#registerTreeItems($tree)
     */
    public function registerTreeItems($tree)
    {
        $table = Table::init("gallery.images", "admintree");
        
        $result = $table
            ->doSelect()
            ->getRows();

        $tree->addChild(new ImageFolder("images", ""), "/", Tree::CONTENT);
        
        foreach($result as $row)
        {
            $tree->addChild(new ImageItem(
                $row->image_title, 
                "/".Config::get("galleryedit", "galleryedit", "admin")."?image_id={$row->image_id}", 
                "/".Config::get("gallerydelete", "gallerydelete", "admin")."?image_id={$row->image_id}", 
                $row->image_url
                ), 
                "/images/", Tree::CONTENT
            );
        }
    }
}