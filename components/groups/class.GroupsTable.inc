<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.groups
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */
importField("Field");
importField("IntField");
importField("users.PrivlilegesField");
importField("users.UsersselectField");

/**
 * User table, this handels all interaction with the user table in the database
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class GroupsTable extends Table
{
    /**
     * Constructor
     */
    public function __construct()
    {
        parent::__construct("groups");
        $this->assign("groups");
        
        $this->addField(new IntField("groups.group_id", Field::INDEX|Field::AUTOKEY));
        $this->addField(new Field("group_name"), 1);
        $privField = new PrivlilegesField("group_privileges", Field::HIDE_VIEW);
        $privField->setDefaultValue(0);
        $this->addField($privField, 2);
        $this->addField(new UsersselectField("group_users", Field::DUMMY|Field::HIDE_VIEW), 3);
        $this->setTitle("[group_name]");
        
        $this->setURLs(
            Config::get('groupsadmin', 'groupsadmin', 'admin'),
            Config::get('groupsadd', 'groupsadd', 'admin'),
            Config::get('groupsedit', 'groupsedit', 'admin'),
            Config::get('groupsdelete', 'groupsdelete', 'admin')
        );
    }
    /**
     * (non-PHPdoc)
     * @see core/Table#select($query, $data)
     */
    protected function select($query, $data = null)
    {
        $query->orderby('group_name');
        
        if(isset($data->user_id) || isset($data->user_name) || isset($data->user_pass) || isset($data->showusers))
        {
            $query->join('usersgroups', 'groups.group_id', 'usersgroups.group_id', 'inner');
            $query->join('users', 'users.user_id', 'usersgroups.user_id', 'inner');
            $query->select("users.user_id");
            $query->select("users.user_name");
            
            $query->whereWhenSet('users.user_id', $data->user_id);
            $query->whereWhenSet('user_name', $data->user_name);
            $query->whereWhenSet('user_pass', $data->user_pass);
        }
    }
    /**
     * (non-PHPdoc)
     * @see core/Table#insert($query, $data)
     */
    protected function insert($query, $data = null) {}
    /**
     * (non-PHPdoc)
     * @see core/Table#update($query, $data)
     */
    protected function update($query, $data = null) {}
    /**
     * (non-PHPdoc)
     * @see core/Table#delete($query, $data)
     */
    protected function delete($query, $data = null) {}
}