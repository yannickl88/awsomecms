<?php
import('/core/shared/class.Component.inc');

class SlideshowComponent extends Component
{
    public function __construct()
    {
        parent::__construct('slideshow');
        $this->setDefaultRequest("images");
        $this->multiTable = true;
        
        $this->fields = array(
            "s_images" => array(
                "image_id" => "index",
                "image_title" => "text"
            ),
            "s_groups" => array(
                "group_id" => "index",
                "group_title" => "text"
            )
        );
    }
    
    public function action_index($smarty, $smartyLoader, $request) {}
    
    public function action_add_image($smarty, $smartyLoader, $request)
    {
        //we have special actions here, mostly because this is used as answer for post, ajax and internal use
        if($request['methode'] == 'post')
        {
            $requestObj = Request::init('slideshow.images');
            $this->setCurrentTable('s_images');
            
            if($request['ajax'] == 'true' || $request['internal'] == 'true' || $this->validate($request, "/".Config::get('slideshow_imageadd', 'slideshow_imageadd', 'admin'), "add"))
            {
                $request["image_url"] = "";
                $request["image_width"] = 0;
                $request["image_height"] = 0;
                
                $id = $requestObj->doInsert($request);
                
                $request["image_id"] = $id;
                
                list($request["image_url"], $request["image_width"], $request["image_height"]) = $this->handleFileUpload($id, 'image_image');
                
                $requestObj->doUpdate($request);
            }
            
            if($request['ajax'] == 'true')
            {
                $this->ajax($requestObj->doSelect(array('image_id' => $id)));
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
    }
    
    public function action_edit_image($smarty, $smartyLoader, $request)
    {
        if($request['methode'] == 'post')
        {
            $requestObj = Request::init('slideshow.images');
            
            if(!empty($_FILES['image_image']['name']))
            {
                $image = $requestObj->doSelect($request);
                
                if(file_exists(Config::get('websiteroot', '.').$image[0]->image_url))
                {
                    unlink(Config::get('websiteroot', '.').$image[0]->image_url);
                }
                
                //upload the image
                list($request["image_url"], $request["image_width"], $request["image_height"]) = $this->handleFileUpload($request["image_id"], 'image_image');
            }
            
            $requestObj->doUpdate($request);
            
            if($request['ajax'] == 'true')
            {
                $this->ajax($requestObj->doSelect(array('image_id' => $id)));
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
                $record = Request::init('slideshow.images')->doSelect($request);
                
                $smarty->assign("record", $record[0]);
            }
            else
            {
                $this->redirect("/".Config::get('slideshow_imageadmin', 'slideshow_imageadmin', 'admin'));
            }
        }
    }
    
    public function action_delete_image($smarty, $smartyLoader, $request)
    {
        $requestObj = Request::init('slideshow.images');
        
        if($request['methode'] == 'post')
        {
            $images = $requestObj->doSelect($request);
            
            foreach($images as $image)
            {
                if(file_exists(Config::get('websiteroot', '.').$image->image_url))
                {
                    @unlink(Config::get('websiteroot', '.').$image->image_url);
                }
            }
            
            $requestObj->doDelete($request);
            
            $this->redirect("/".Config::get('slideshow_imageadmin', 'slideshow_imageadmin', 'admin'));
        }
        else
        {
            if($request['image_id'])
            {
                $smarty->assign("record", $requestObj->doSelect($request));
            }
            else
            {
                $this->redirect("/".Config::get('slideshow_imageadmin', 'slideshow_imageadmin', 'admin'));
            }
        }
    }
    
    public function action_admin_image($smarty, $smartyLoader, $request)
    {
        $smarty->assign("records", Request::init('slideshow.images')->doSelect());
        
        //urls
        $smarty->assign("actionurl_add", Config::get('slideshow_imageadd', 'slideshow_imageadd', 'admin'));
        $smarty->assign("actionurl_edit", Config::get('slideshow_imageedit', 'slideshow_imageedit', 'admin'));
        $smarty->assign("actionurl_delete", Config::get('slideshow_imagedelete', 'slideshow_imagedelete', 'admin'));
    }
    
    public function action_add_group($smarty, $smartyLoader, $request)
    {
        if($request['methode'] == 'post')
        {
            $requestObj = Request::init('slideshow.groups');
            $this->setCurrentTable('s_groups');
            
            if($this->validate($request, "/".Config::get('slideshow_groupadd', 'slideshow_groupadd', 'admin'), "add"))
            {
                //upload the image
                $request['internal'] = 'true';
                $request['image_title'] = $request['group_title']." Header";
                $request['group_header'] = $this->action_add_image($smarty, $smartyLoader, $request);
                
                //add the group
                $id = $requestObj->doInsert($request);
                
                //now the images
                foreach($request['images'] as $image)
                {
                    Request::init('slideshow.groupsimages')->doInsert(array('group_id' => $id, 'image_id' => $image));
                }
            }
            
            $this->redirect("/".Config::get('slideshow_groupadmin', 'slideshow_groupadmin', 'admin'));
        }
        else
        {
            $smarty->assign("images", Request::init('slideshow.images')->doSelect());
        }
    }
    
    public function action_edit_group($smarty, $smartyLoader, $request)
    {
        if($request['methode'] == 'post')
        {
            $requestObj = Request::init('slideshow.groups');
            $this->setCurrentTable('s_groups');
            
            if($this->validate($request, "/".Config::get('slideshow_groupadd', 'slideshow_groupadd', 'admin'), "add"))
            {
                $id = $request['group_id'];
                
                if(!empty($_FILES['image_image']['name']))
                {
                    $group = $requestObj->doSelect($request);
                    
                    //upload the image
                    $request['internal'] = 'true';
                    $request['image_title'] = $request['group_title']." Header";
                    $request['image_id'] = $group[0]->group_header;
                    $request['group_header'] = $this->action_edit_image($smarty, $smartyLoader, $request);
                }
                
                //update the group
                $requestObj->doUpdate($request);
                
                //now the images
                Request::init('slideshow.groupsimages')->doDelete(array('group_id' => $id));
                
                foreach($request['images'] as $image)
                {
                    Request::init('slideshow.groupsimages')->doInsert(array('group_id' => $id, 'image_id' => $image));
                }
            }
            
            $this->redirect("/".Config::get('slideshow_groupadmin', 'slideshow_groupadmin', 'admin'));
        }
        else
        {
            if($request['group_id'])
            {
                $record = Request::init('slideshow.groups')->doSelect($request);
                $images = Request::init('slideshow.images')->doSelect();
                $groupimages = Request::init('slideshow.groupsimages')->doSelect($request);
                
                $images_ids = array();
                
                foreach($groupimages as $image)
                {
                    $images_ids[] = $image->image_id;
                }
                
                foreach($images as $key => $image)
                {
                    if(in_array($image->image_id, $images_ids))
                    {
                        unset($images[$key]);
                    }
                }
                
                $smarty->assign("record", $record[0]);
                $smarty->assign("images", $images);
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
        
        if($request['methode'] == 'post')
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
        
        if(($maxSize == -1 || $fileSize <= $maxSize) && in_array($fileExtention, $allowedExtentions))
        {
            move_uploaded_file($_FILES[$name]['tmp_name'], $destFile);
            
            $info = getimagesize($destFile);
            
            return array('/'.Config::get('uploaddir', 'img/slideshow/', 'slideshow').$destName, $info[0], $info[1]);
        }
        else
        {
            echo "Could not upload file";
            
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