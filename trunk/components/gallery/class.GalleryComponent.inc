<?php
import('/core/class.Component.inc');

class GalleryComponent extends Component
{
    public function __construct()
    {
        parent::__construct('gallery');
        
        $this->fields = array(
            "image_id" => "index",
            "image_title" => "text"
        );
    }
    
    public function action_index($smarty, $smartyLoader, $request)
    {
        
    }
    
    public function action_add($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            if($this->validate($request, "/".Config::get('galleryadd', 'galleryadd', 'admin'), "add"))
            {
                $requestObj = Request::init("gallery");
                
                $request["image_url"] = "";
                $request["image_urlthumb"] = "";
                $request["image_width"] = 0;
                $request["image_height"] = 0;
                
                $id = $requestObj->doInsert($request);
                
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
                    $exception = new InvalidFormException($request, Config::get('contactadd', 'contactadd', 'admin'), "The form was not valid");
                    $exception->addInvalidField($field, "Field was not filled in");
                    die("ERROR");
                }
                
                $id = $requestObj->doUpdate($request);
            }
            
            if($request['uploadMore'] == 1)
            {
                $this->redirect("/".Config::get('galleryadd', 'galleryadd', 'admin'));
            }
            else
            {
                $this->redirect("/".Config::get('galleryadmin', 'galleryadmin', 'admin'));
            }
        }
    }
    
    public function action_edit($smarty, $smartyLoader, $request)
    {
        $requestObj = Request::init("gallery");
        
        if($request['method'] == 'post')
        {
            if($this->validate($request, "/".Config::get('galleryedit', 'galleryedit', 'admin'), "edit"))
            {
                $record = $requestObj->doSelect($request);
                
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
                
                $requestObj->doUpdate($request);
            }
            
            $this->redirect("/".Config::get('galleryadmin', 'galleryadmin', 'admin'));
        }
        else
        {
            if($request['image_id'])
            {
                $record = $requestObj->doSelect($request);
                
                $smarty->assign("record", $record[0]);
            }
            else
            {
                $this->redirect("/".Config::get('galleryadmin', 'galleryadmin', 'admin'));
            }
        }
    }
    
    public function action_delete($smarty, $smartyLoader, $request)
    {
        $requestObj = Request::init("gallery");
        
        if($request['method'] == 'post')
        {
            $record = $requestObj->doSelect($request);
            
            $id = $request['image_id'];
            
            //remove old one
            if(file_exists(Config::get('websiteroot', '.').$record[0]->image_url))
            {
                @unlink(Config::get('websiteroot', '.').'/'.$record[0]->image_url);
            }
            if(file_exists(Config::get('websiteroot', '.').$record[0]->image_urlthumb))
            {
                @unlink(Config::get('websiteroot', '.').$record[0]->image_urlthumb);
            }
            $requestObj->doDelete($request);
            
            $this->redirect("/".Config::get('galleryadmin', 'galleryadmin', 'admin'));
        }
        else
        {
            if($request['image_id'])
            {
                $record = $requestObj->doSelect($request);
                
                $smarty->assign("record", $record);
            }
            else
            {
                $this->redirect("/".Config::get('galleryadmin', 'galleryadmin', 'admin'));
            }
        }
    }
    
    public function action_admin($smarty, $smartyLoader, $request)
    {
        $smarty->assign("records", Request::init("gallery")->doSelect());
        
        //urls
        $smarty->assign("actionurl_add", Config::get('galleryadd', 'galleryadd', 'admin'));
        $smarty->assign("actionurl_edit", Config::get('galleryedit', 'galleryedit', 'admin'));
        $smarty->assign("actionurl_delete", Config::get('gallerydelete', 'gallerydelete', 'admin'));
    }
    
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
    
    protected function validate($data, $redirectOnFailure, $mode = "edit")
    {
        $valid = true;
        $index = -1;
        $exception = new InvalidFormException($data, $redirectOnFailure, "The form was not valid");
        
        //check if the file did actully upload
        if($_FILES['image_image']['error'] > 0) //uh-oh error
        {
            $error = "";
            
            switch($_FILES['image_image']['error'])
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
            
            $exception->addInvalidField('image_image', $error);
            throw $exception;
        }
        
        //check if the file did actully upload
        if($data['image_tumb_generate'] != 'on' && $_FILES['image_thumb']['error'] > 0) //uh-oh error
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
        
        if($data['image_tumb_generate'] != 'on')
        {
            //check the tumb dimentions if set
            $maxDimentions = explode('x', Config::get('tumbsize', "100x50", 'gallery'));
            $tempInfo = getimagesize($_FILES['image_thumb']['tmp_name']);
            
            if($tempInfo[0] > $maxDimentions[0] || $tempInfo[1] > $maxDimentions[1])
            {
                $exception->addInvalidField('image_thumb', "Tumb image was to large");
                
                if($mode == "edit")
                {
                    foreach($this->fields as $field => $type)
                    {
                        //check for index
                        if($index == -1 && $type == 'index')
                        {
                            $index = $field;
                            break;
                        }
                    }
                    
                    $exception->setRedirectData(array($index => $data[$index]));
                }
                
                throw $exception;
            }
        }
        
        return parent::validate($data, $redirectOnFailure, $mode);
    }
    
    public function registerMenuItems($menu)
    {
        $menu->addChild(new MenuItem("gallery", "", Config::get('galleryadmin', 'galleryadmin', 'admin')));
        $menu->addChild(new MenuItem("galleryadmin", "Images", Config::get('galleryadmin', 'galleryadmin', 'admin'), "gallery"));
        $menu->addChild(new MenuItem("galleryadd", "Upload", Config::get('galleryadd', 'galleryadd', 'admin'), "gallery"));
    }
}