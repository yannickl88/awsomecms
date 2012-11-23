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
import('/core/class.Authentication.inc');

/**
 * User component and the actions that it can do.
 * This components gives you the option to create different users inside a
 * system, these users can be assigned a name and password and privilages. If
 * the group component is installed, privilages will be delegated there
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class UsersComponent extends Component implements AuthComponent
{
    public function __construct()
    {
        parent::__construct('users');
    }
    /**
     * (non-PHPdoc)
     * @see core/Component::register()
     *
     * @param RegisterManager $registerManager
     */
    public function register($registerManager)
    {
        $registerManager->registerAuth("users.action_add", "admin");
        $registerManager->registerAuth("users.action_edit", "admin");
        $registerManager->registerAuth("users.action_delete", "admin");
        $registerManager->registerAuth("users.action_privadd", "admin");
        $registerManager->registerAuth("users.action_privedit", "admin");
        $registerManager->registerAuth("users.action_privdelete", "admin");
        
        $registerManager->registerPage(array($this, 'add'), '{form table="users.users" form="add"}');
        $registerManager->registerPage(array($this, 'edit'), '{form table="users.users" form="edit"}');
        $registerManager->registerPage(array($this, 'delete'), '{form table="users.users" form="delete"}');
        $registerManager->registerPage(array($this, 'admin'), '{form table="users.users" form="admin"}');
        $registerManager->registerPage(array($this, 'privadmin'), '{form table="users.privileges" form="admin"}');
        $registerManager->registerPage(array($this, 'privadd'), '{form table="users.privileges" form="add"}<br />');
        $registerManager->registerPage(array($this, 'privedit'), '{form table="users.privileges" form="edit"}');
        $registerManager->registerPage(array($this, 'privdelete'), '{form table="users.privileges" form="delete"}');
    }
    /**
     * Internal function to handle login. This checks user and password and
     * if they are filled in or throws and FormException otherwise.
     * 
     * @param array $request
     * @param string $redirect
     */
    private function login($request, $redirect)
    {
        //extra custom validation
        $valid = true;
        $e = new InvalidFormException((object) $request, $redirect, "users.users");
        
        if(empty($request['user_name']))
        {
            $valid = false;
            $e->addInvalidField('user_name', Language::get("error_notfilledin"));
        }
        if(empty($request['user_pass']))
        {
            $valid = false;
            $e->addInvalidField('user_pass', Language::get("error_notfilledin"));
        }
        
        if($valid)
        {
            $loggedin = Authentication::login($request['user_name'], $request['user_pass']);
            
            if(!$loggedin || ($loggedin && isset($_SESSION['usr']['auth']['admin']) && $_SESSION['usr']['auth']['admin'] !== true))
            {
                $e->addInvalidField('user_name', Language::get("error_nomatch"));
                $e->addInvalidField('user_pass', "");
                
                throw $e;
            }
        }
        else
        {
            throw $e;
        }
        
        $this->redirect($redirect);
    }
    /**
     * Login and redirect to the admin section
     * 
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_login($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            $this->login($request, "/".$_GET['url']);
        }
    }
    /**
     * Login and redirect to the index
     * 
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_admin_login($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            $this->login($request, "/".Config::get('location', 'location', 'admin'));
        }
    }
    /**
     * Add a user
     * 
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_add($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            //extra custom validation
            if($request['user_pass'] != $request['user_pass2'])
            {
                $e = new InvalidFormException((object) $request, makeLink(array($this, "add")), "users.users", "Password do not match");
                
                $e->addInvalidField('user_pass', "Passwords do not match");
                $e->addInvalidField('user_pass2', "");
                
                throw $e;
            }
            else
            {
                Table::init('users.users')
                    ->setRecord((object) $request)
                    ->doInsert();
            }
            
            $this->redirect(array($this, "admin"));
        }
    }
    /**
     * Edit a user
     * 
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_edit($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            //extra custom validation
            if(!empty($request['user_pass']) && $request['user_pass'] != $request['user_pass2'])
            {
                $e = new InvalidFormException((object) $request, makeLink(array($this, "edit")), "users.users", "Password do not match");
                
                $e->setRedirectData(array("user_id" => $request["user_id"]));
                $e->addInvalidField('user_pass', Language::get("error_passwordsnomatch"));
                $e->addInvalidField('user_pass2', "");
                
                throw $e;
            }
            else
            {
                Table::init('users.users')
                    ->setRecord((object) $request)
                    ->doUpdate();
                
                $this->redirect(array($this, "admin"));
            }
        }
        else
        {
            if($request['user_id'])
            {
                Table::init('users.users')
                    ->setRequest((object) $request);
            }
            else
            {
                $this->redirect(array($this, "admin"));
            }
        }
    }
    /**
     * Delete a user
     * 
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_delete($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            Table::init('users.users')
                ->setRecord((object) $request)
                ->doDelete();
            
            $this->redirect(array($this, "admin"));
        }
        else
        {
            if($request['user_id'])
            {
                Table::init('users.users')
                    ->setRecord((object) $request);
            }
            else
            {
                $this->redirect(array($this, "admin"));
            }
        }
    }
    /**
     * Add a privilege
     * 
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_privadd($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            $table = Table::init('users.privileges')
                ->setRecord((object) $request);
            
            //check if name is not yet chosen, we do not wat duplicates
            if($table->setRequest((object) $request)->doSelect()->count() == 0)
            {
                $table->doInsert();
            }
            else
            {
                $e = new InvalidFormException((object) $request, makeLink(array($this, "privadd")), "users.privileges", "Privilage already exists");
                $e->addInvalidField('privilege_name', Language::get("error_selectothername"));
                
                throw $e;
            }
            
            $this->redirect(makeLink(array($this, "privadmin")));
        }
    }
    /**
     * Edit a privilege
     * 
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_privedit($smarty, $smartyLoader, $request)
    {
        $table = Table::init('users.privileges')
            ->setRecord((object) $request);
         
        if($request['method'] == 'post')
        {
            $table->doUpdate();
            
            $this->redirect(makeLink(array($this, "privadmin")));
        }
    }
    /**
     * Delete a privilege
     * 
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_privdelete($smarty, $smartyLoader, $request)
    {
        $table = Table::init('users.privileges')
            ->setRecord((object) $request);
        
        if($request['method'] == 'post')
        {
            $table->doDelete($request);
            
            $this->redirect(makeLink(array($this, "privadmin")));
        }
        elseif(!$request['privilege_id'])
        {
            $this->redirect(makeLink(array($this, "privadmin")));
        }
    }
    /**
     * Get the privileges the user has
     * 
     * @param int $userPrivs        Privlige field from the user record
     * @return array
     */
    public function getPrivilageMapping($userPrivs)
    {
        $privilages = Table::init('users.privileges')->doSelect()->getRows();
        $rights = array();
        $authGroups = array();
        
        foreach($privilages as $privilage)
        {
            $rights[$privilage->privilege_name] = Config::hasFlag($userPrivs, $privilage->privilege_int);
            if($rights[$privilage->privilege_name])
            {
                $authGroups = array_merge($authGroups, $privilage->privilege_auths);
            }
        }
        
        return array($rights, $authGroups);
    }
    /**
     * Authenticate a user
     * 
     * @param string $user_name
     * @param string $user_pass
     * @return boolean
     */
    public function auth_login($user_name, $user_pass)
    {
        $record = Table::init('users.users')
            ->setRequest((object) array("user_name" => $user_name))
            ->doSelect()
            ->getRow();
        
        if(!empty($record) && $record->user_pass['password'] == sha1($record->user_pass['salt'].$user_pass))
        {
            //set the session
            $_SESSION['usr'] = array();
            $_SESSION['usr']['user_name'] = $user_name;
            $_SESSION['usr']['user_pass'] = $user_pass;
            list($_SESSION['usr']['auth'], $_SESSION['usr']['privs']) = 
                $this->getPrivilageMapping($record->user_privileges);
            
            return true;
        }
        return false;
    }
    /**
     * Log a user out
     */
    public function auth_logout()
    {
        $_SESSION['usr'] = null;
        session_destroy();
        
        return true;
    }
    /**
     * (non-PHPdoc)
     * @see core/Component#registerMenuItems($menu)
     */
    public function registerMenuItems($menu)
    {
        $menu->addChild(new MenuItem("users", array($this, 'admin')));
        $menu->addChild(new MenuItem("usersadmin", array($this, 'admin'), "users"));
        
        if(Config::get("usersMenu", true, "menu"))
        {
            $menu->addChild(new MenuItem("privadmin", array($this, 'privadmin'), "users"));
        }
    }
}