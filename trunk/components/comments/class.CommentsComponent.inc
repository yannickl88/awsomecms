<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.comments
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */
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
     * (non-PHPdoc)
     * @see core/Component::register()
     *
     * @param RegisterManager $registerManager
     */
    public function register($registerManager)
    {
        $registerManager->registerAuth("comments.action_delete", "admin");
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
        if($request['method'] == 'post')
        {
            $requestObj = Table::init('comments.comments')
                ->setRecord((object) $request)
                ->doInsert();
            
            $this->redirect($request['comment_hook']);
        }
    }
    public function action_delete($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            $requestObj = Table::init('comments.comments')
                ->setRecord((object) $request)
                ->doDelete();
            $this->ajax(array('id' => (int) $request['comment_id']));
        }
    }
}