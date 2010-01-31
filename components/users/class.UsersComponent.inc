<?php
import('/core/class.Component.inc');

class UsersComponent extends Component
{
    public function __construct()
    {
        parent::__construct('users');
    }
    
    public function action_index($smarty, $smartyLoader, $request)
    {
        
    }
    
    public function action_add($smarty, $smartyLoader, $request)
    {
        if($request['methode'] == 'post')
        {
            //extra custom validation
            if($request['user_pass'] != $request['user_pass2'])
            {
                $e = new InvalidFormException($request, "/".Config::get('usersadd', 'usersadd', 'admin'), "Password do not match");
                $e->addInvalidField('user_pass', "Passwords do not match");
                $e->addInvalidField('user_pass2', "");
                
                throw $e;
            }
            else
            {
                //sum the privlilages
                if(!isset($request['privileges']))
                {
                    $request['privileges'] = array();
                }
                $request['user_privileges'] = array_sum($request['privileges']);
                
                $userRequest = Request::init('users.users');
                $request["user_id"] = $userRequest->doInsert($request);
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
        if($request['methode'] == 'post')
        {
            unset($this->fields['users']['user_pass']);
            
            if($request['user_pass'] == '' || $request['user_pass'] != $request['user_pass2'])
            {
                unset($request['user_pass']);
                unset($request['user_pass2']);
            }
            
            if($this->validate($request, "/".Config::get('usersedit', 'usersedit', 'admin'), "edit"))
            {
                //sum the privlilages
                if(!isset($request['privileges']))
                {
                    $request['privileges'] = array();
                }
                $request['user_privileges'] = array_sum($request['privileges']);
                
                $userRequest = Request::init('users.users');
                $userRequest->doUpdate($request);
            }
            
            $this->redirect("/".Config::get('usersadmin', 'usersadmin', 'admin'));
        }
        else
        {
            if($request['user_id'])
            {
                Table::init('users.users');
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
        $userRequest = Request::init('users.users');
        
        if($request['methode'] == 'post')
        {
            $userRequest->doDelete($request);
            
            $this->redirect("/".Config::get('usersadmin', 'usersadmin', 'admin'));
        }
        else
        {
            if($request['user_id'])
            {
                $record = $userRequest->doSelect($request);
                
                $smarty->assign("record", $record);
            }
            else
            {
                $this->redirect("/".Config::get('usersadmin', 'usersadmin', 'admin'));
            }
        }
        
        return $request;
    }
    
    public function action_admin($smarty, $smartyLoader, $request)
    {
        $userRequest = Table::init('users.users');
        $userRequest->setRecord($userRequest->doSelect()->getRows());
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
        if($request['methode'] == 'post')
        {
            $this->login($request, "/".$_GET['url']);
        }
    }
    
    public function action_admin_login($smarty, $smartyLoader, $request)
    {
        if($request['methode'] == 'post')
        {
            $this->login($request, "/".Config::get('location', 'location', 'admin'));
        }
    }
    
    public function action_privadmin($smarty, $smartyLoader, $request)
    {
        $requestObj = Request::init('users.privileges');
        $smarty->assign("records", $requestObj->doSelect());
        
        //urls
        $smarty->assign("actionurl_add", Config::get('privadd', 'privadd', 'admin'));
        $smarty->assign("actionurl_edit", Config::get('privedit', 'privedit', 'admin'));
        $smarty->assign("actionurl_delete", Config::get('privdelete', 'privdelete', 'admin'));
    }
    
    public function action_privadd($smarty, $smartyLoader, $request)
    {
        if($request['methode'] == 'post')
        {
            $this->setCurrentTable('privileges');
            
            //select the privlilages
            if($this->validate($request, "/".Config::get('privadd', 'privadd', 'admin'), "add"))
            {
                $requestObj = Request::init('users.privileges');
                
                //check if name is not yet chosen, we do not wat duplicates
                if(!$requestObj->doSelect($request))
                {
                    $requestObj->doInsert($request);
                }
                else
                {
                    $e = new InvalidFormException($request, "/".Config::get('privadd', 'privadd', 'admin'), "Privilage already exists");
                    $e->addInvalidField('privilege_name', "Please select another name");
                    
                    throw $e;
                }
            }
            
            $this->redirect("/".Config::get('privadmin', 'privadmin', 'admin'));
        }
    }
    
    public function action_privedit($smarty, $smartyLoader, $request)
    {
        $requestObj = Request::init('users.privileges');
        
        if($request['methode'] == 'post')
        {
            $this->setCurrentTable('privileges');
            
            //select the privlilages
            if($this->validate($request, "/".Config::get('privedit', 'privedit', 'admin'), "edit"))
            {
                //check if name is not yet chosen, we do not wat duplicates
                if(!$requestObj->doSelect($request))
                {
                    $requestObj->doUpdate($request);
                }
                else
                {
                    $e = new InvalidFormException($request, "/".Config::get('privedit', 'privedit', 'admin'), "Privilage already exists");
                    $e->addInvalidField('privilege_name', "Please select another name");
                    
                    throw $e;
                }
            }
            
            $this->redirect("/".Config::get('privadmin', 'privadmin', 'admin'));
        }
        else
        {
            $records = $requestObj->doSelect($request);
            
            $smarty->assign("record", $records[0]);
        }
    }
    
    public function action_privdelete($smarty, $smartyLoader, $request)
    {
        $requestObj = Request::init('users.privileges');
        
        if($request['methode'] == 'post')
        {
            $requestObj->doDelete($request);
            
            $this->redirect("/".Config::get('privadmin', 'privadmin', 'admin'));
        }
        else
        {
            if($request['privilege_name'])
            {
                $record = $requestObj->doSelect($request);
                
                $smarty->assign("record", $record);
            }
            else
            {
                $this->redirect("/".Config::get('privadmin', 'privadmin', 'admin'));
            }
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