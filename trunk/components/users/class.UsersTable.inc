<?php
importField("Field");
importField("IntField");
importField("PasswordField");

class UsersTable extends Table
{
    public function __construct()
    {
        parent::__construct("users");
        $this->assign("users");
        
        $this->addField(new IntField("user_id", "", Field::INDEX|Field::AUTOKEY));
        $this->addField(new Field("user_name", "Name"));
        $this->addField(new PasswordField("user_pass", "Password"));
        $this->addField(new PasswordField("user_pass2", "Again", Field::DUMMY));
        $this->addField(new IntField("user_privileges", "", Field::HIDE_ADD|Field::HIDE_VIEW));
        
        $this->setURLs(
            Config::get('usersadmin', 'usersadmin', 'admin'),
            Config::get('usersadd', 'usersadd', 'admin'),
            Config::get('usersedit', 'usersedit', 'admin'),
            Config::get('usersdelete', 'usersdelete', 'admin')
        );
    }
    
    protected function select($query, $data = array())
    {
        $query->orderby('user_name');
        
        $query->whereWhenSet('user_id', $data['user_id']);
        $query->whereWhenSet('user_name', $data['user_name']);
        $query->whereWhenSet('user_pass', $data['user_pass']);
    }
    protected function insert($query, $data = array())
    {
        $query->insert('user_privileges', 1);
    }
    protected function update($query, $data = array()) {}
    protected function delete($query, $data = array()) {}
}