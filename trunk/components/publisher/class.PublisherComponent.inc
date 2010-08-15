<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.users
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */
import('/core/class.Component.inc');

/**
 * User component and the actions that it can do.
 * This components gives you the option to create different users inside a
 * system, these users can be assigned a name and password and privilages. If
 * the group component is installed, privilages will be delegated there
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class PublisherComponent extends Component
{
    /**
     * Constructor
     */
    public function __construct()
    {
        parent::__construct('publisher');
    }
    /**
     * (non-PHPdoc)
     * @see core/Component::register()
     *
     * @param RegisterManager $registerManager
     */
    public function register($registerManager)
    {
        //register stuff 
        $registerManager->registerPage(array($this, 'admin'), '{form component="publisher" form="admin"}');
    }
    /**
     * (non-PHPdoc)
     * @see core/Component::registerMenuItems()
     *
     * @param Menu $menu
     */
    public function registerMenuItems($menu)
    {
        $menu->addChild(new MenuItem("publish", array($this, 'admin')));
    }
    
    public function action_admin($smarty, $smartyLoader, $request)
    {
        
    }
    public function action_publish($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            
        }
        $this->redirect(array($this, "admin"));
    }
}