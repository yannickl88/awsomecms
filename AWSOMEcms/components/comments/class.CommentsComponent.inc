<?php
import('/core/shared/class.Component.inc');

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
    
    public function action_admin($smarty, $smartyLoader, $request)
    {
        $requestObj = Request::init('comments');
        
        $smarty->assign("records", $requestObj->doSelect());
        
        //urls
        $smarty->assign("actionurl_add", Config::get('galleryadd', 'galleryadd', 'admin'));
        $smarty->assign("actionurl_edit", Config::get('galleryedit', 'galleryedit', 'admin'));
        $smarty->assign("actionurl_delete", Config::get('gallerydelete', 'gallerydelete', 'admin'));
    }
    
    public function action_reply($smarty, $smartyLoader, $request)
    {
        if($request['methode'] == 'post' && isset($request['comment_hook']))
        {
            $requestObj = Request::init('comments');
            $request['comment_time'] = date('c');
            
            $this->validate($request, '/'.$request['comment_hook'], 'add');
            $requestObj->doInsert($request);
            $this->redirect('/'.$request['comment_hook']);
        }
    }
}