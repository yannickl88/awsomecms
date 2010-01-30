<?php
import('/core/class.Component.inc');

/**
* Comments compontent class, this includes all logic for an component
*/
class CommentsComponent extends Component
{
    /**
    * Constructor for the comments component
    */
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
    
    /**
    * Admin action, this is called when the admin page is shown
    * 
    * @param Smarty $smarty
    * @param SmartyPageLoader $smartyLoader
    * @param array $request
    */
    public function action_admin($smarty, $smartyLoader, $request)
    {
        $requestObj = Request::init('comments');
        
        $smarty->assign("records", $requestObj->doSelect());
        
        //urls
        $smarty->assign("actionurl_add", Config::get('galleryadd', 'galleryadd', 'admin'));
        $smarty->assign("actionurl_edit", Config::get('galleryedit', 'galleryedit', 'admin'));
        $smarty->assign("actionurl_delete", Config::get('gallerydelete', 'gallerydelete', 'admin'));
    }
    
    /**
    * Repley action, this is called when a reply is posted
    * 
    * @param Smarty $smarty
    * @param SmartyPageLoader $smartyLoader
    * @param array $request
    */
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