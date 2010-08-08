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
        $registerManager->registerPage('add', '/admin/users/', '{form table="users.users" form="add"}');
        $registerManager->registerPage('edit', '/admin/users/', '{form table="users.users" form="edit"}');
        $registerManager->registerPage('delete', '/admin/users/', '{form table="users.users" form="delete"}');
        $registerManager->registerPage('admin', '/admin/users/', '{form table="users.users" form="admin"}');
        $registerManager->registerPage('privadmin', '/admin/users/', '{form table="users.privileges" form="admin"}');
        $registerManager->registerPage('privadd', '/admin/users/', '{form table="users.privileges" form="add"}<br />');
        $registerManager->registerPage('privedit', '/admin/users/', '{form table="users.privileges" form="edit"}');
        $registerManager->registerPage('privdelete', '/admin/users/', '{form table="users.privileges" form="delete"}');
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
                $e = new InvalidFormException((object) $request, "/".Config::get('usersadd', 'usersadd', 'admin'), "users.users", "Password do not match");
                
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
            
            $this->redirect("/".Config::get('usersadmin', 'usersadmin', 'admin'));
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
                $e = new InvalidFormException((object) $request, "/".Config::get('usersedit', 'usersedit', 'admin'), "users.users", "Password do not match");
                
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
                
                $this->redirect("/".Config::get('usersadmin', 'usersadmin', 'admin'));
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
                $this->redirect("/".Config::get('usersadmin', 'usersadmin', 'admin'));
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
            
            $this->redirect("/".Config::get('usersadmin', 'usersadmin', 'admin'));
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
                $this->redirect("/".Config::get('usersadmin', 'usersadmin', 'admin'));
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
                $e = new InvalidFormException((object) $request, "/".Config::get('privadd', 'privadd', 'admin'), "users.privileges", "Privilage already exists");
                $e->addInvalidField('privilege_name', Language::get("error_selectothername"));
                
                throw $e;
            }
            
            $this->redirect("/".Config::get('privadmin', 'privadmin', 'admin'));
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
            
            $this->redirect("/".Config::get('privadmin', 'privadmin', 'admin'));
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
            
            $this->redirect("/".Config::get('privadmin', 'privadmin', 'admin'));
        }
        elseif(!$request['privilege_id'])
        {
            $this->redirect("/".Config::get('privadmin', 'privadmin', 'admin'));
        }
    }
    /**
     * Get the privileges the user has
     * 
     * @param int $privileges       Privlige field from the user record
     * @return array
     */
    public function getPrivilageMapping($privileges)
    {
        $privilages = Table::init('users.privileges')->doSelect()->getRows();
        $rights = array();
        
        foreach($privilages as $privilage)
        {
            $rights[$privilage->privilege_name] = Config::hasFlag($privileges, $privilage->privilege_int);
        }
        
        return $rights;
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
            $_SESSION['usr']['auth'] = $this->getPrivilageMapping($record->user_privileges);
            
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
        $menu->addChild(new MenuItem("users", Config::get('usersadmin', 'usersadmin', 'admin')));
        $menu->addChild(new MenuItem("usersadmin", Config::get('usersadmin', 'usersadmin', 'admin'), "users"));
        $menu->addChild(new MenuItem("privadmin", Config::get('privadmin', 'privadmin', 'admin'), "users"));
    }
}