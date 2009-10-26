<?php
import('/core/shared/class.Component.inc');

class UsersComponent extends Component
{
    public function __construct()
    {
        parent::__construct('users');
        
        $this->fields = array(
            "user_id" => "index",
            "user_name" => "text",
            "user_pass" => "text",
        );
    }
    
    public function action_index($smarty, $smartyLoader, $request)
    {
        
    }
    
    public function action_add($smarty, $smartyLoader, $request)
    {
        if($request['methode'] == 'post')
        {
            if($this->validate($request, "/".Config::get('usersadd', 'usersadd', 'admin'), "add"))
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
                    $id = $this->insert($request);
                }
            }
            
            $this->redirect("/".Config::get('usersadmin', 'usersadmin', 'admin'));
        }
    }
    
    public function action_edit($smarty, $smartyLoader, $request)
    {
        if($request['methode'] == 'post')
        {
            unset($this->fields['user_pass']);
            
            if($request['user_pass'] == '' || $request['user_pass'] != $request['user_pass2'])
            {
                unset($request['user_pass']);
                unset($request['user_pass2']);
            }
            
            if($this->validate($request, "/".Config::get('usersedit', 'usersedit', 'admin'), "edit"))
            {
                $this->update($request);
            }
            
            $this->redirect("/".Config::get('usersadmin', 'usersadmin', 'admin'));
        }
        else
        {
            if($request['user_id'])
            {
                $record = $this->select($request);
                
                $smarty->assign("record", $record[0]);
            }
            else
            {
                $this->redirect("/".Config::get('usersadmin', 'usersadmin', 'admin'));
            }
        }
    }
    
    public function action_delete($smarty, $smartyLoader, $request)
    {
        if($request['methode'] == 'post')
        {
            $this->delete($request);
            
            $this->redirect("/".Config::get('usersadmin', 'usersadmin', 'admin'));
        }
        else
        {
            if($request['user_id'])
            {
                $record = $this->select($request);
                
                $smarty->assign("record", $record);
            }
            else
            {
                $this->redirect("/".Config::get('usersadmin', 'usersadmin', 'admin'));
            }
        }
    }
    
    public function action_admin($smarty, $smartyLoader, $request)
    {
        $smarty->assign("records", $this->select());
        
        //urls
        $smarty->assign("actionurl_add", Config::get('usersadd', 'usersadd', 'admin'));
        $smarty->assign("actionurl_edit", Config::get('usersedit', 'usersedit', 'admin'));
        $smarty->assign("actionurl_delete", Config::get('usersdelete', 'usersdelete', 'admin'));
    }
    
    public function action_login($smarty, $smartyLoader, $request)
    {
        if($request['methode'] == 'post')
        {
            //extra custom validation
            $valid = true;
            $e = new InvalidFormException($request, "/".Config::get('location', 'location', 'admin'));
            
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
                if(Controller::getInstance()->auth_login($request['user_name'], $request['user_pass']))
                {
                    $_SESSION['usr'] = array();
                    $_SESSION['usr']['user_name'] = $request['user_name'];
                    $_SESSION['usr']['user_pass'] = $request['user_pass'];
                    $_SESSION['usr']['auth'] = Controller::getInstance()->auth($record['user_id']);
                }
                else
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
            
            $this->redirect("/".Config::get('location', 'location', 'admin'));
        }
    }
    
    public function auth($user_id)
    {
        $privilages = Request::init('privileges')->doSelect();
        $user = Request::init('users')->doSelect(array('user_id' => $user_id));
        $rights = array();
        
        foreach($privilages as $privilage)
        {
            $rights[$privilage->privilege_name] = Config::hasFlag($user[0]->user_privileges, $privilage->privilege_int);
        }
        
        return $rights;
    }
    
    public function auth_login($user_name, $user_pass)
    {
        $user_pass = md5($user_pass);
        
        $record = $this->select(array("user_name" => $user_name, "user_pass" => $user_pass));
        
        return (count($record) > 0);
    }
    
    public function registerMenuItems($menu)
    {
        $menu->addChild(new MenuItem("users", "", Config::get('usersadmin', 'usersadmin', 'admin')));
        $menu->addChild(new MenuItem("usersadmin", "Users", Config::get('usersadmin', 'usersadmin', 'admin'), "users"));
    }
}