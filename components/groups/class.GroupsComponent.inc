<?php
/**
 * Component that allowes for group based authentication, rather then user based
 *
 * @author Yannick <yannick.l.88@gmail.com>
 */
class GroupsComponent extends Component implements AuthComponent
{
    /**
     * Constructor
     *
     * @return unknown_type
     */
    public function __construct()
    {
        parent::__construct('groups');
    }
    /**
     * (non-PHPdoc)
     * @see core/Component::register()
     *
     * @param RegisterManager $registerManager
     */
    public function register($registerManager)
    {
        $registerManager->registerAuth("groups.action_add", "admin");
        $registerManager->registerAuth("groups.action_edit", "admin");
        $registerManager->registerAuth("groups.action_delete", "admin");

        $registerManager->registerHook($this->component, "users", "post", "add", "useradd");
        $registerManager->registerHook($this->component, "users", "post", "delete", "userdel");

        //admin pages
        $registerManager->registerPage(array($this, 'admin'), '{form table="groups.groups" form="admin"}');
        $registerManager->registerPage(array($this, 'add'), '{form table="groups.groups" form="add"}');
        $registerManager->registerPage(array($this, 'edit'), '{form table="groups.groups" form="edit"}');
        $registerManager->registerPage(array($this, 'delete'), '{form table="groups.groups" form="delete"}');
    }
    /**
     * Action to add an group
     *
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
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

            $this->redirect(array($this, "admin"));
        }
    }
    /**
     * Action to edit an group
     *
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_edit($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            Table::init('groups.groups')
            ->setRecord((object) $request)
            ->doUpdate();

            //update the usersgroups references
            Table::init('groups.usersgroups')
            ->setRequest((object) $request)
            ->doDelete();

            foreach($request['users'] as $user)
            {
                Table::init('groups.usersgroups')
                ->setRecord((object) array("user_id" => $user, "group_id" => $request['group_id']))
                ->doInsert();
            }

            $this->redirect(array($this, "admin"));
        }
        else
        {
            if($request['group_id'])
            {
                $users = Table::init('groups.groups')
                ->setRequest((object) array("showusers" => true, 'group_id' => $request['group_id']))
                ->doSelect()
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
                $this->redirect(array($this, "admin"));
            }
        }
    }
    /**
     * Action to delete an group
     *
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_delete($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            Table::init('groups.groups')
            ->setRequest((object) $request)
            ->doDelete();

            Table::init('groups.usersgroups')
            ->setRequest((object) (object) array("group_id" => $request["group_id"]))
            ->doDelete();

            $this->redirect(array($this, "admin"));
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
                $this->redirect(array($this, "admin"));
            }
        }
    }
    /**
     * Request a list of privilages a given groups has
     *
     * @param int $userPriv
     * @param array $preset     Set of privilages already assigned
     * @return array
     */
    public function getPrivilageMapping($userPriv, $preset = array())
    {
        $privilages = Table::init('users.privileges')
        ->doSelect()
        ->getRows();
        $rights = $preset;
        $authGroups = array();

        foreach($privilages as $privilage)
        {
            if(!isset($preset[$privilage->privilege_name]) || !$preset[$privilage->privilege_name])
            {
                $rights[$privilage->privilege_name] = Config::hasFlag((int) $userPriv, $privilage->privilege_int);
            }
            if(Config::hasFlag($userPriv, $privilage->privilege_int))
            {
                $authGroups = array_merge($authGroups, $privilage->privilege_auths);
            }
        }

        return array($rights, $authGroups);
    }
    /**
     * Log a user in with crendtials
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
            $_SESSION['usr']['auth'] = array();
            $_SESSION['usr']['privs'] = array();

            $records = Table::init('groups.groups')
            ->setRequest((object) array("user_id" => $record->user_id))
            ->doSelect()
            ->getRows();

            foreach($records as $record)
            {
                list($rights, $authGroups) = $this->getPrivilageMapping($record->group_privileges, $_SESSION['usr']['auth']);
                $_SESSION['usr']['auth'] = $rights;
                $_SESSION['usr']['privs'] = array_merge($_SESSION['usr']['privs'], $authGroups);
            }
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
     * Hook to add a reference of a user when it is deleted in the user component
     *
     * @param Smarty $smarty
     * @param array $request
     * @return boolean
     */
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
    /**
     * Hook to delete a reference of a user when it is deleted in the user component
     *
     * @param Smarty $smarty
     * @param array $request
     * @return boolean
     */
    public function hook_userdel($smarty, $request)
    {
        if($request['method'] == 'post')
        {
            Table::init('groups.usersgroups')
            ->setRequest((object) $request)
            ->doDelete();
        }
        return true;
    }
    /**
     * (non-PHPdoc)
     * @see core/Component#registerMenuItems($menu)
     */
    public function registerMenuItems($menu)
    {
        $menu->addChild(new MenuItem("groupsadmin", array($this, "admin"), "users"));
    }
}