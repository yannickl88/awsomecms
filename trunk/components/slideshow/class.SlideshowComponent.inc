<?php
import('/core/class.Component.inc');

class SlideshowComponent extends Component
{
    public function __construct()
    {
        parent::__construct('slideshow');
    }
    
    public function action_index($smarty, $smartyLoader, $request) {}
    
    public function action_add_image($smarty, $smartyLoader, $request)
    {
        //we have special actions here, mostly because this is used as answer for post, ajax and internal use
        if($request['method'] == 'post')
        {
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
                
                if($request['ajax'] == 'true')
                {
                    $this->ajax(array("error" => $error));
                }
                else if($request['internal'] == 'true')
                {
                    return false;
                }
                else
                {
                    $e = new InvalidFormException($request, "/".Config::get('slideshow_imageadd', 'slideshow_imageadd', 'admin'));
                    $e->addInvalidField('image_image', $error);
                    throw $e;
                }
            }
            
            $request["image_url"] = "";
            $request["image_width"] = 0;
            $request["image_height"] = 0;
            
            $id = Table::init('slideshow.images')
                ->setRecord((object) $request)
                ->doInsert();
            
            $request["image_id"] = $id;
            
            list($request["image_url"], $request["image_width"], $request["image_height"]) = $this->handleFileUpload($id, 'image_image');
            
            Table::init('slideshow.images')
                ->setRecord((object) $request)
                ->doUpdate();
            
            if($request['ajax'] === 'true')
            {
                $this->ajax(Table::init('slideshow.images')->doSelect(array('image_id' => $id))->getRows());
            }
            else if($request['internal'] === 'true')
            {
                return $id;
            }
            else
            {
                $this->redirect("/".Config::get('slideshow_imageadmin', 'slideshow_imageadmin', 'admin'));
            }
        }
    }
    
    public function action_edit_image($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            if(!empty($_FILES['image_image']['name']))
            {
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
                    
                    if($request['ajax'] == 'true')
                    {
                        $this->ajax(array("error" => $error));
                    }
                    else if($request['internal'] == 'true')
                    {
                        return false;
                    }
                    else
                    {
                        $e = new InvalidFormException($request, "/".Config::get('slideshow_imageadd', 'slideshow_imageadd', 'admin'));
                        $e->addInvalidField('image_image', $error);
                        throw $e;
                    }
                }
                
                $image = Table::init('slideshow.images')
                    ->setRequest((object) $request)
                    ->doSelect()
                    ->getRows();
                
                if(file_exists(Config::get('websiteroot', '.').$image[0]->image_url))
                {
                    unlink(Config::get('websiteroot', '.').$image[0]->image_url);
                }
                
                //upload the image
                list($request["image_url"], $request["image_width"], $request["image_height"]) = $this->handleFileUpload($request["image_id"], 'image_image');
            }
            
            Table::init('slideshow.images')
                ->setRecord((object) $request)
                ->doUpdate();
            
            if($request['ajax'] == 'true')
            {
                $this->ajax(Table::init('slideshow.images')->doSelect(array('image_id' => $id))->getRows());
            }
            else if($request['internal'] == 'true')
            {
                return $id;
            }
            else
            {
                $this->redirect("/".Config::get('slideshow_imageadmin', 'slideshow_imageadmin', 'admin'));
            }
        }
        else
        {
            if($request['image_id'])
            {
                $smarty->assign("record", Table::init('slideshow.images')->setRequest((object) $request)->doSelect()->getRow());
            }
            else
            {
                $this->redirect("/".Config::get('slideshow_imageadmin', 'slideshow_imageadmin', 'admin'));
            }
        }
    }
    
    public function action_delete_image($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            $images = Table::init('slideshow.images')
                ->setRequest((object) $request)
                ->doSelect()
                ->getRows();
            
            foreach($images as $image)
            {
                if(file_exists(Config::get('websiteroot', '.').$image->image_url))
                {
                    @unlink(Config::get('websiteroot', '.').$image->image_url);
                }
            }
            
            Table::init('slideshow.images')
                ->setRequest((object) $request)
                ->doDelete();
            
            $this->redirect("/".Config::get('slideshow_imageadmin', 'slideshow_imageadmin', 'admin'));
        }
        else
        {
            if($request['image_id'])
            {
                Table::init('slideshow.images')
                    ->setRequest((object) $request);
            }
            else
            {
                $this->redirect("/".Config::get('slideshow_imageadmin', 'slideshow_imageadmin', 'admin'));
            }
        }
    }
    
    public function action_add_group($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            //upload the image
            $request['internal'] = 'true';
            $request['image_title'] = $request['group_title']." Header";
            //is there an upload?
            if(!empty($_FILES['image_image']['temp_name']))
            {
                $request['group_header'] = $this->action_add_image($smarty, $smartyLoader, $request);
            }
            else //check dropdown
            {
                $request['group_header'] = $request['image_image_select'];
            }
            
            //add the group
            $id = Table::init('slideshow.groups')
                ->setRecord((object) $request)
                ->doInsert();
            
            //now the images
            foreach($request['images'] as $image)
            {
                Table::init('slideshow.groupsimages')
                    ->setRecord((object) array('group_id' => $id, 'image_id' => $image))
                    ->doInsert();
            }
            
            $this->redirect("/".Config::get('slideshow_groupadmin', 'slideshow_groupadmin', 'admin'));
        }
        else
        {
            $smarty->assign("images", Table::init('slideshow.images')->doSelect()->getRows());
        }
    }
    
    public function action_edit_group($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            $id = $request['group_id'];
            
            
            if(!empty($_FILES['image_image']['name']))
            {
                $group = Table::init('slideshow.groups')
                    ->setRequest((object) $request)
                    ->doSelect();
                
                //upload the image
                $request['internal'] = 'true';
                $request['image_title'] = $request['group_title']." Header";
                $request['group_header'] = (int) $this->action_add_image($smarty, $smartyLoader, $request);
            }
            else //check dropdown
            {
                $request['group_header'] = (int) $request['image_image_select'];
            }
            //update the group
            Table::init('slideshow.groups')
                ->setRequest((object) $request)
                ->setRecord((object) $request)
                ->doUpdate();
            
            //now the images
            Table::init('slideshow.groupsimages')
                ->setRecord((object) array('group_id' => $id))
                ->doDelete();
            
            foreach($request['images'] as $image)
            {
                Table::init('slideshow.groupsimages')
                    ->setRecord((object) array('group_id' => $id, 'image_id' => $image))
                    ->doInsert();
            }
            
            $this->redirect("/".Config::get('slideshow_groupadmin', 'slideshow_groupadmin', 'admin'));
        }
        else
        {
            if($request['group_id'])
            {
                $record = Table::init('slideshow.groups')
                    ->setRequest((object) $request)
                    ->doSelect()
                    ->getRow();
                $images = Table::init('slideshow.images')
                    ->doSelect()
                    ->getRows();
                $groupimages = Table::init('slideshow.groupsimages')
                    ->setRequest((object) $request)
                    ->doSelect()
                    ->getRows();
                
                $images_ids = array();
                
                foreach($groupimages as $image)
                {
                    $images_ids[] = $image->image_id;
                }
                
                $smarty->assign("record", $record);
                $smarty->assign("images", $images);
                $smarty->assign("images_ids", $images_ids);
                $smarty->assign("groupimages", $groupimages);
            }
            else
            {
                $this->redirect("/".Config::get('slideshow_groupadmin', 'slideshow_groupadmin', 'admin'));
            }
        }
    }
    
    public function action_delete_group($smarty, $smartyLoader, $request)
    {
        $requestObj = Request::init('slideshow.groups');
        
        if($request['method'] == 'post')
        {
            $record = $requestObj->doSelect($request);
            
            $id = $request['group_id'];
            
            //remove images linked to this group
            Request::init('slideshow.groupsimages')->doDelete(array("group_id" => $id));
            
            $requestObj->doDelete(array("group_id" => $id));
            
            $this->redirect("/".Config::get('slideshow_groupadmin', 'slideshow_groupadmin', 'admin'));
        }
        else
        {
            if($request['group_id'])
            {
                $smarty->assign("record", $requestObj->doSelect($request));
            }
            else
            {
                $this->redirect("/".Config::get('slideshow_groupadmin', 'slideshow_groupadmin', 'admin'));
            }
        }
    }
    
    public function action_admin_group($smarty, $smartyLoader, $request)
    {
        $smarty->assign("records", Request::init('slideshow.groups')->doSelect());
        
        //urls
        $smarty->assign("actionurl_add", Config::get('slideshow_groupadd', 'slideshow_groupadd', 'admin'));
        $smarty->assign("actionurl_edit", Config::get('slideshow_groupedit', 'slideshow_groupedit', 'admin'));
        $smarty->assign("actionurl_delete", Config::get('slideshow_groupdelete', 'slideshow_groupdelete', 'admin'));
    }
    
    private function handleFileUpload($id, $name)
    {
        $file = $_FILES[$name]['name'];
        $fileExtention = strtolower(substr($file, strrpos($file, '.')+1));
        $fileName = basename($file, '.'.$fileExtention);
        $fileSize = $_FILES[$name]['size'];
        $maxSize = Config::get('maxsize', -1, 'slideshow');
        $allowedExtentions = explode(",", Config::get('extentions', 'jpg,gif,png', 'gallery'));
        $destName = $id.'-'.$fileName.'.'.$fileExtention;
        $destFile = realpath(Config::get('websiteroot', '.').'/'.Config::get('uploaddir', 'img/slideshow/', 'slideshow')).'/'.$destName;
        
        if(($maxSize == -1 || $fileSize <= $maxSize) && in_array($fileExtention, $allowedExtentions) && $_FILES[$name]['error'] == 0)
        {
            if(!file_exists(realpath(Config::get('websiteroot', '.').'/'.Config::get('uploaddir', 'img/slideshow/', 'slideshow'))))
            {
                mkdir(realpath(Config::get('websiteroot', '.').'/'.Config::get('uploaddir', 'img/slideshow/', 'slideshow')), 777, true);
            }
            
            if(!move_uploaded_file($_FILES[$name]['tmp_name'], $destFile))
            {
                return false;
            }
            
            $info = getimagesize($destFile);
            
            return array('/'.Config::get('uploaddir', 'img/slideshow/', 'slideshow').$destName, $info[0], $info[1]);
        }
        else
        {
            //echo "Could not upload file";
            
            return false;
        }
    }
    public function registerMenuItems($menu)
    {
        $menu->addChild(new MenuItem("slideshow", "", ""));
        $menu->addChild(new MenuItem("slideshowimg", "Images", Config::get('slideshow_imageadmin', 'slideshow_imageadmin', 'admin'), "slideshow"));
        $menu->addChild(new MenuItem("slideshowgrp", "Groups", Config::get('slideshow_groupadmin', 'slideshow_groupadmin', 'admin'), "slideshow"));
    }
}