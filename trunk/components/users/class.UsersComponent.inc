<?php
import('/core/class.Component.inc');

class UsersComponent extends Component
{
    public function __construct()
    {
        parent::__construct('users');
    }
    
    private function login($request, $redirect)
    {
        //extra custom validation
        $valid = true;
        $e = new InvalidFormException($request, $redirect, "users.users");
        
        if(empty($request['user_name']))
        {
            $valid = false;
            $e->addInvalidField('user_name', "Field was not filled in");
        }
        if(empty($request['user_pass']))
        {
            $valid = false;
            $e->addInvalidField('user_pass', "Field was not filled in");
        }
        
        if($valid)
        {
            $loggedin = Controller::getInstance()->auth_login($request['user_name'], $request['user_pass']);
            
            if(!$loggedin || ($loggedin && $_SESSION['usr']['auth']['admin'] !== true))
            {
                $e->addInvalidField('user_name', "Username and Password do not match");
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
    
    public function action_login($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            $this->login($request, "/".$_GET['url']);
        }
    }
    
    public function action_admin_login($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            $this->login($request, "/".Config::get('location', 'location', 'admin'));
        }
    }
    
    public function action_add($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            //extra custom validation
            if($request['user_pass'] != $request['user_pass2'])
            {
                $e = new InvalidFormException(array((object) $request), "/".Config::get('usersadd', 'usersadd', 'admin'), "users.users", "Password do not match");
                
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
        else
        {
            if(!Config::getInstance()->isComponentInstalled("groups"))
            {
                $privRequest = Request::init('users.privileges');
                $smarty->assign("privileges", $privRequest->doSelect());
            }
        }
        
        return $request;
    }
    
    public function action_edit($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            //extra custom validation
            if(!empty($request['user_pass']) && $request['user_pass'] != $request['user_pass2'])
            {
                $e = new InvalidFormException(array((object) $request), "/".Config::get('usersedit', 'usersedit', 'admin'), "users.users", "Password do not match");
                
                $e->setRedirectData(array("user_id" => $request["user_id"]));
                $e->addInvalidField('user_pass', "Passwords do not match");
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
                    ->setRecord((object) $request);
            }
            else
            {
                $this->redirect("/".Config::get('usersadmin', 'usersadmin', 'admin'));
            }
        }
        
        return $request;
    }
    
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
    
    public function action_privadd($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            $table = Table::init('users.privileges')
                ->setRecord((object) $request);
            
            //check if name is not yet chosen, we do not wat duplicates
            if($table->doSelect((object) $request)->count() == 0)
            {
                $table->doInsert();
            }
            else
            {
                $e = new InvalidFormException($request, "/".Config::get('privadd', 'privadd', 'admin'), "users.privileges", "Privilage already exists");
                $e->addInvalidField('privilege_name', "Please select another name");
                
                throw $e;
            }
            
            $this->redirect("/".Config::get('privadmin', 'privadmin', 'admin'));
        }
    }
    
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
    
    public function auth($userRow)
    {
        $privilages = Table::init('users.privileges')->doSelect();
        $rights = array();
        
        foreach($privilages as $privilage)
        {
            $rights[$privilage->privilege_name] = Config::hasFlag($userRow->user_privileges, $privilage->privilege_int);
        }
        
        return $rights;
    }
    
    public function auth_login($user_name, $user_pass)
    {
        $user_pass = md5($user_pass);
        
        $record = Table::init('users.users')
            ->doSelect(array("user_name" => $user_name, "user_pass" => $user_pass))
            ->getRow();
        $privilages = $this->auth($record);
        
        if(count($record) == 1)
        {
            //set the session
            $_SESSION['usr'] = array();
            $_SESSION['usr']['user_name'] = $user_name;
            $_SESSION['usr']['user_pass'] = $user_pass;
            $_SESSION['usr']['auth'] = $privilages;
            
            return true;
        }
        else
        {
            return false;
        }
    }
    
    public function auth_logout()
    {
        $_SESSION['usr'] = null;
        session_destroy();
    }
    
    public function registerMenuItems($menu)
    {
        $menu->addChild(new MenuItem("users", "", Config::get('usersadmin', 'usersadmin', 'admin')));
        $menu->addChild(new MenuItem("usersadmin", "Users", Config::get('usersadmin', 'usersadmin', 'admin'), "users"));
        $menu->addChild(new MenuItem("privadmin", "Privileges", Config::get('privadmin', 'privadmin', 'admin'), "users"));
    }
}