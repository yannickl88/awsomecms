<?php
import('/core/class.Component.inc');

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
            ),
        );
    }
    
    public function action_index($smarty, $smartyLoader, $request)
    {
        
    }
    public function action_add($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            $this->setCurrentTable("groups");
            if($this->validate($request, "/".Config::get('groupsadd', 'groupsadd', 'admin'), "add"))
            {
                //sum the privlilages
                if(!isset($request['privileges']))
                {
                    $request['privileges'] = array();
                }
                $request['group_privileges'] = array_sum($request['privileges']);
                
                $userRequest = Request::init('groups.groups');
                $usersgroupsRequest = Request::init('groups.usersgroups');
                $id = $userRequest->doInsert($request);
                
                //now add the links between users
                foreach($request['users'] as $user)
                {
                    $usersgroupsRequest->doInsert(array("group_id" => $id, "user_id" => $user));
                }
            }
            
            $this->redirect("/".Config::get('groupsadmin', 'groupsadmin', 'admin'));
        }
        else
        {
            $smarty->assign("privileges", Request::init('users.privileges')->doSelect());
            $smarty->assign("users", Request::init('users.users')->doSelect());
        }
    }
    
    public function action_edit($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            $this->setCurrentTable("groups");
            if($this->validate($request, "/".Config::get('groupsedit', 'groupsedit', 'admin'), "edit"))
            {
                //sum the privlilages
                if(!isset($request['privileges']))
                {
                    $request['privileges'] = array();
                }
                $request['group_privileges'] = array_sum($request['privileges']);
                
                $userRequest = Request::init('groups.groups');
                $usersgroupsRequest = Request::init('groups.usersgroups');
                $id = $userRequest->doUpdate($request);
                
                //now add the links between users
                $usersgroupsRequest->doDelete(array("group_id" => $id));
                
                foreach($request['users'] as $user)
                {
                    $usersgroupsRequest->doInsert(array("group_id" => $id, "user_id" => $user));
                }
            }
            
            $this->redirect("/".Config::get('groupsadmin', 'groupsadmin', 'admin'));
        }
        else
        {
            if($request['group_id'])
            {
                $request['showusers'] = true;
                $record = Request::init('groups.groups')->doSelect($request);
                
                $smarty->assign("record", array_shift($record));
                $smarty->assign("privileges", Request::init('users.privileges')->doSelect());
                $smarty->assign("users", Request::init('users.users')->doSelect());
            }
            else
            {
                $this->redirect("/".Config::get('groupsadmin', 'groupsadmin', 'admin'));
            }
        }
    }
    
    public function action_delete($smarty, $smartyLoader, $request)
    {
        $groupRequest = Request::init('groups.groups');
        
        if($request['method'] == 'post')
        {
            $groupRequest->doDelete($request);
            
            $this->redirect("/".Config::get('groupsadmin', 'groupsadmin', 'admin'));
        }
        else
        {
            if($request['group_id'])
            {
                $record = $groupRequest->doSelect($request);
                
                $smarty->assign("record", $record);
            }
            else
            {
                $this->redirect("/".Config::get('groupsadmin', 'groupsadmin', 'admin'));
            }
        }
    }
    
    public function action_admin($smarty, $smartyLoader, $request)
    {
        $userRequest = Request::init('groups.groups');
        $smarty->assign("records", $userRequest->doSelect());
        
        //urls
        $smarty->assign("actionurl_add", Config::get('groupsadd', 'groupsadd', 'admin'));
        $smarty->assign("actionurl_edit", Config::get('groupsedit', 'groupsedit', 'admin'));
        $smarty->assign("actionurl_delete", Config::get('groupsdelete', 'groupsdelete', 'admin'));
    }
    
    public function auth($groupRecord)
    {
        $privilages = Request::init('users.privileges')->doSelect();
        $rights = array();
        
        foreach($privilages as $privilage)
        {
            $rights[$privilage->privilege_name] = Config::hasFlag((int) $groupRecord->group_privileges, $privilage->privilege_int);
        }
        
        return $rights;
    }
    
    public function auth_login($user_name, $user_pass)
    {
        $user_pass = md5($user_pass);
        
        $records = Request::init('groups.groups')->doSelect(array("user_name" => $user_name, "user_pass" => $user_pass));
        
        foreach($records as $record)
        {
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
        }
        
        return false;
    }
    
    public function auth_logout()
    {
        $_SESSION['usr'] = null;
        session_destroy();
    }
    
    public function hook_useradd($smarty, $request)
    {
        if($request['method'] == 'post')
        {
            $requestObj = Request::init('groups.usersgroups');
            $requestObj->doInsert(array("group_id" => Config::get('defaultgroup', 2, 'groups'), "user_id" => $request['user_id']));
        }
        return true;
    }
    
    public function hook_userdel($smarty, $request)
    {
        if($request['method'] == 'post')
        {
            $requestObj = Request::init('groups.usersgroups');
            $requestObj->doDelete(array("user_id" => $request['user_id']));
        }
        return true;
    }
    
    public function registerMenuItems($menu)
    {
        $menu->addChild(new MenuItem("groupsadmin", "Groups", Config::get('groupsadmin', 'groupsadmin', 'admin'), "users"));
    }
}