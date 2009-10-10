<?php
include_once Config::get('websiteroot', '.').'/../core/shared/class.Component.inc';

class CommentsComponent extends Component
{
    public function __construct()
    {
        parent::__construct('comments');
        
        $this->fields = array(
            "comment_id" => "index",
            "comment_text" => "text",
            "comment_username" => "text",
            "comment_hook" => "text",
            "comment_time" => "datetime"
        );
    }
    
    public function action_index($smarty, $smartyLoader, $request)
    {
        
    }
    
    public function action_add($smarty, $smartyLoader, $request)
    {
        if($request['methode'] == 'post')
        {
            if($this->validate($request, "/".Config::get('galleryadd', 'galleryadd', 'admin'), "add"))
            {
                $request["image_url"] = "";
                $request["image_urlthumb"] = "";
                $request["image_width"] = 0;
                $request["image_height"] = 0;
                
                $id = $this->insert($request);
                
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
                
                $id = $this->update($request);
            }
            
            $this->redirect("/".Config::get('galleryadmin', 'galleryadmin', 'admin'));
        }
    }
    
    public function action_edit($smarty, $smartyLoader, $request)
    {
        if($request['methode'] == 'post')
        {
            if($this->validate($request, "/".Config::get('galleryedit', 'galleryedit', 'admin'), "edit"))
            {
                $record = $this->select($request);
                
                $id = $request['image_id'];
                
                if($_FILES['image_image'])
                {
                    //remove old one
                    if($record[0]->image_url != '' && file_exists(Config::get('websiteroot', '.').$record[0]->image_url))
                    {
                        unlink(Config::get('websiteroot', '.').'/'.$record[0]->image_url);
                    }
                    if($record[0]->image_urlthumb != '' && file_exists(Config::get('websiteroot', '.').$record[0]->image_urlthumb))
                    {
                        unlink(Config::get('websiteroot', '.').$record[0]->image_urlthumb);
                    }
                    
                    //upload new one
                    list($request["image_url"], $request["image_width"], $request["image_height"]) = $this->handleFileUpload($id, 'image_image');
                    
                    if($request['image_tumb_generate'] == 'on')
                    {
                        list($request["image_urlthumb"], $request["image_thumbwidth"], $request["image_thumbheight"]) = $this->createThumb($id, $request["image_url"]);
                    }
                    else
                    {
                        list($request["image_urlthumb"], $request["image_thumbwidth"], $request["image_thumbheight"]) = $this->handleThumbUpload($id, 'image_thumb');
                    }
                }
                else if($_FILES['image_thumb'])
                {
                    //remove old one
                    if(file_exists(Config::get('websiteroot', '.').'/'.Config::get('uploaddir', 'img/upload/', 'gallery').$record[0]->image_urlthumb))
                    {
                        unlink(Config::get('websiteroot', '.').'/'.Config::get('uploaddir', 'img/upload/', 'gallery').$record[0]->image_urlthumb);
                    }
                    
                    //upload new one
                    list($request["image_urlthumb"], $request["image_thumbwidth"], $request["image_thumbheight"]) = $this->handleThumbUpload($id, 'image_thumb');
                }
                
                $id = $this->update($request);
            }
            
            $this->redirect("/".Config::get('galleryadmin', 'galleryadmin', 'admin'), array("image_id" => $request['image_id']));
        }
        else
        {
            if($request['image_id'])
            {
                $record = $this->select($request);
                
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
        if($request['methode'] == 'post')
        {
            $record = $this->select($request);
            
            $id = $request['image_id'];
            
            //remove old one
            if(file_exists(Config::get('websiteroot', '.').$record[0]->image_url))
            {
                unlink(Config::get('websiteroot', '.').'/'.$record[0]->image_url);
            }
            if(file_exists(Config::get('websiteroot', '.').$record[0]->image_urlthumb))
            {
                unlink(Config::get('websiteroot', '.').$record[0]->image_urlthumb);
            }
            $this->delete($request);
            
            $this->redirect("/".Config::get('galleryadmin', 'galleryadmin', 'admin'));
        }
        else
        {
            if($request['image_id'])
            {
                $record = $this->select($request);
                
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
        $smarty->assign("records", $this->select());
        
        //urls
        $smarty->assign("actionurl_add", Config::get('galleryadd', 'galleryadd', 'admin'));
        $smarty->assign("actionurl_edit", Config::get('galleryedit', 'galleryedit', 'admin'));
        $smarty->assign("actionurl_delete", Config::get('gallerydelete', 'gallerydelete', 'admin'));
    }
    
    public function action_reply($smarty, $smartyLoader, $request)
    {
        if($request['methode'] == 'post' && isset($request['comment_hook']))
        {
            $request['comment_time'] = date('c');
            
            $this->validate($request, '/'.$request['comment_hook'], 'add');
            $this->insert($request);
            $this->redirect('/'.$request['comment_hook']);
        }
    }
}