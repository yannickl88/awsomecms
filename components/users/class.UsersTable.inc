<?php
/**
 * User table, this handels all interaction with the user table in the database
 *
 * @author Yannick <yannick.l.88@gmail.com>
 */
class UsersTable extends Table
{
    /**
     * Constructor
     */
    public function __construct()
    {
        parent::__construct("users");
        $this->assign("users");

        $this->addField(new IntField("user_id", Field::INDEX|Field::AUTOKEY));
        $this->addField(new Field("user_name"));
        $this->addField(new PasswordField("user_pass", Field::OPTIONAL_EDIT));
        $this->addField(new PasswordField("user_pass2", Field::DUMMY));
        $style = Field::HIDE_VIEW;
        if(RegisterManager::getInstance()->hasComponent("groups") && Config::get("component", "users", "auth") == "groups")
        {
            $style = Field::HIDE;
        }
        $privField = new PrivlilegesField("user_privileges", $style);
        $privField->setDefaultValue(0);
        $this->addField($privField);
        $this->setTitle("[user_name]");
    }
    /**
     * (non-PHPdoc)
     * @see core/Table#select($query, $data)
     */
    protected function select($query, $data = null)
    {
        $query->orderby('user_name');

        if($data)
        {
            $query->whereWhenSet('user_name', $data->user_name);
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