<?php
import('/core/class.Component.inc');

class GroupsComponent extends Component
{
    public function __construct()
    {
        parent::__construct('groups');
    }
    
    public function action_add($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            $id = Table::init('groups.groups')
                ->setRecord((object) $request)
                ->doInsert();
            
            foreach($request['users'] as $user)
            {
                Table::init('groups.usersgroups')->setRecord((object) array("user_id" => $user, "group_id" => $id))->doInsert();
            }
            
            $this->redirect("/".Config::get('groupsadmin', 'groupsadmin', 'admin'));
        }
    }
    
    public function action_edit($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            Table::init('groups.groups')
                ->setRecord((object) $request)
                ->doUpdate();
            
            //update the usersgroups references
            $usersgroups = Table::init('groups.usersgroups');
            $usersgroups->doDelete((object) $request);
            
            foreach($request['users'] as $user)
            {
                $usersgroups->setRecord((object) array("user_id" => $user, "group_id" => $request['group_id']))->doInsert();
            }
            
            $this->redirect("/".Config::get('groupsadmin', 'groupsadmin', 'admin'));
        }
        else
        {
            if($request['group_id'])
            {
                $users = Table::init('groups.groups')
                    ->doSelect((object) array("showusers" => true, 'group_id' => $request['group_id']))
                    ->getRows();
                    
                $request['group_users'] = array();
                foreach($users as $user)
                {
                    $request['group_users'][$user->user_id] = $user;
                }
                
                Table::init('groups.groups')
                    ->setRequest((object) $request);
            }
            else
            {
                $this->redirect("/".Config::get('groupsadmin', 'groupsadmin', 'admin'));
            }
        }
    }
    
    public function action_delete($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            Table::init('groups.groups')
                ->doDelete((object) $request);
            
            Table::init('groups.usersgroups')
                ->doDelete((object) array("group_id" => $request["group_id"]));
            
            $this->redirect("/".Config::get('groupsadmin', 'groupsadmin', 'admin'));
        }
        else
        {
            if($request['group_id'])
            {
                Table::init('groups.groups')
                    ->setRequest((object) $request);
            }
            else
            {
                $this->redirect("/".Config::get('groupsadmin', 'groupsadmin', 'admin'));
            }
        }
    }
    
    public function auth($groupRecord)
    {
        $privilages = Table::init('users.privileges')
            ->doSelect()
            ->getRows();
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
        
        $records = Table::init('groups.groups')
            ->doSelect((object) array("user_name" => $user_name, "user_pass" => $user_pass))
            ->getRows();
        
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
            Table::init('groups.usersgroups')
                ->setRecord((object) array(
                    "group_id" => Config::get('defaultgroup', 2, 'groups'), 
                    "user_id" => Table::init('users.users')->getInsertedID()
                ))
                ->doInsert();
        }
        return true;
    }
    
    public function hook_userdel($smarty, $request)
    {
        if($request['method'] == 'post')
        {
            Table::init('groups.usersgroups')
                ->doDelete((object) $request);
        }
        return true;
    }
    
    public function registerMenuItems($menu)
    {
        $menu->addChild(new MenuItem("groupsadmin", "Groups", Config::get('groupsadmin', 'groupsadmin', 'admin'), "users"));
    }
}