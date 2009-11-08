<?php
import('/core/shared/class.Component.inc');

class GroupsComponent extends Component
{
    public function __construct()
    {
        parent::__construct('groups');
        
        $this->multiTable = true;
        $this->fields = array(
            "groups" => array(
                "group_id" => "index",
                "group_name" => "text",
                "group_privilages" => "int",
            ),
        );
    }
    
    public function action_index($smarty, $smartyLoader, $request)
    {
        
    }
    
    public function auth($user_id)
    {
        $privilages = Request::init('users.privileges')->doSelect();
        $user = Request::init('groups.groups')->doSelect(array('user_id' => $user_id));
        $rights = array();
        
        foreach($privilages as $privilage)
        {
            $rights[$privilage->privilege_name] = Config::hasFlag((int) $user[0]->group_privileges, $privilage->privilege_int);
        }
        
        return $rights;
    }
    
    public function auth_login($user_name, $user_pass)
    {
        $user_pass = md5($user_pass);
        
        $record = Request::init('groups.groups')->doSelect(array("user_name" => $user_name, "user_pass" => $user_pass));
        $privilages = $this->auth($record[0]->user_id);
        
        if(count($record) == 1 && $privilages['admin'] === true)
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
    
    public function registerMenuItems($menu)
    {
    }
}