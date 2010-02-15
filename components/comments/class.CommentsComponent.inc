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
        if($request['method'] == 'post' && isset($request['comment_hook']))
        {
            $requestObj = Table::init('comments.comments')
                ->setRecord((object) $request)
                ->doInsert();
            
            $this->redirect('/'.$request['comment_hook']);
        }
    }
}