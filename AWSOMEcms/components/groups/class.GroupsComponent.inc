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
            ),
        );
    }
    
    public function action_index($smarty, $smartyLoader, $request)
    {
        
    }
    public function action_add($smarty, $smartyLoader, $request)
    {
        if($request['methode'] == 'post')
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
                $id = $userRequest->doInsert($request);
            }
            
            $this->redirect("/".Config::get('groupsadmin', 'groupsadmin', 'admin'));
        }
        else
        {
            $privRequest = Request::init('users.privileges');
            $smarty->assign("privileges", $privRequest->doSelect());
        }
    }
    
    public function action_edit($smarty, $smartyLoader, $request)
    {
        if($request['methode'] == 'post')
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
                $id = $userRequest->doUpdate($request);
            }
            
            $this->redirect("/".Config::get('groupsadmin', 'groupsadmin', 'admin'));
        }
        else
        {
            if($request['group_id'])
            {
                $groupRequest = Request::init('groups.groups');
                $privRequest = Request::init('users.privileges');
                
                $record = $groupRequest->doSelect($request);
                
                $smarty->assign("record", $record[0]);
                $smarty->assign("privileges", $privRequest->doSelect());
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
        
        if($request['methode'] == 'post')
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
    
    public function hook_useradd($smarty, $request)
    {
        if($request['methode'] == 'post')
        {
            $requestObj = Request::init('groups.usersgroups');
            $requestObj->doInsert(array("group_id" => Config::get('defaultgroup', 1, 'groups'), "user_id" => $request['user_id']));
        }
        return true;
    }
    
    public function hook_userdel($smarty, $request)
    {
        if($request['methode'] == 'post')
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