<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.users
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */
importField("Field");
importField("IntField");
importField("PasswordField");
importField("users.PrivlilegesField");

class UsersTable extends Table
{
    public function __construct()
    {
        parent::__construct("users");
        $this->assign("users");
        
        $this->addField(new IntField("user_id", "", Field::INDEX|Field::AUTOKEY));
        $this->addField(new Field("user_name", "Name"));
        $this->addField(new PasswordField("user_pass", "Password", Field::OPTIONAL_EDIT));
        $this->addField(new PasswordField("user_pass2", "Again", Field::DUMMY));
        $this->addField(new PrivlilegesField("user_privileges", "Privliveges", Field::HIDE_VIEW));
        $this->setTitle("[user_name]");
        
        $this->setURLs(
            Config::get('usersadmin', 'usersadmin', 'admin'),
            Config::get('usersadd', 'usersadd', 'admin'),
            Config::get('usersedit', 'usersedit', 'admin'),
            Config::get('usersdelete', 'usersdelete', 'admin')
        );
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
            $query->whereWhenSet('user_id', $data->user_id);
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