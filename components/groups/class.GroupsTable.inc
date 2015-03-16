<?php
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
    }
    /**
     * (non-PHPdoc)
     * @see core/Table#select($query, $data)
     */
    protected function select($query, $data = null)
    {
        $query->orderby('group_name');

        if(isset($data->user_id) || isset($data->showusers))
        {
            $query->join('usersgroups', 'groups.group_id', 'usersgroups.group_id', 'inner');
            $query->join('users', 'users.user_id', 'usersgroups.user_id', 'inner');
            $query->select("users.user_id");
            $query->select("users.user_name");

            $query->whereWhenSet('users.user_id', $data->user_id);
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