<?php
/**
 * Selection list of all the users
 *
 * @author Yannick <yannick.l.88@gmail.com>
 */
class UsersField extends Field
{
    private $username;

    /**
     * (non-PHPdoc)
     * @see core/fields/Field#setData($record)
     */
    public function setData($record)
    {
        $this->username = $record->user_name;

        return parent::setData($record);
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#getValue($data)
     */
    public function getValue($data = null)
    {
        if($data != null && isset($data->user_name))
        {
            return ucfirst($data->user_name);
        }
        else
        {
            return parent::getValue($data);
        }
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_add()
     */
    protected function toHTML_add()
    {
        $users = Table::init("users.users")
            ->doSelect()
            ->getRows();

        $html = "";
        $html .= "<select class='cmsfield' name=\"{$this->getName()}\" id=\"{$this->getName()}\" >";
        foreach($users as $user)
        {
            $html .= "    <option value=\"{$user->user_id}\">{$user->user_name}</option>";
        }
        $html .= "</select>";
        return $html;
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_edit()
     */
    protected function toHTML_edit()
    {
        $users = Table::init("users.users")
            ->doSelect()
            ->getRows();

        $html = "";
        $html .= "<select class='cmsfield' name=\"{$this->getName()}\" id=\"{$this->getName()}\" >";
        foreach($users as $user)
        {
            $checked = "";
            if($this->getValue() == $user->user_id)
            {
                $checked = " selected=\"selected\"";
            }

            $html .= "    <option value=\"{$user->user_id}\"{$checked}>{$user->user_name}</option>";
        }
        $html .= "</select>";
        return $html;
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_view()
     */
    protected function toHTML_view()
    {
        return $this->username;
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#select($query, $data)
     */
    public function select($query, $data = array()) //@var $query SQLQuery
    {
        parent::select($query, $data);
        $query->join("users", $this->getName(), "user_id")
            ->select("user_name")
            ->select("user_id");

    }
}