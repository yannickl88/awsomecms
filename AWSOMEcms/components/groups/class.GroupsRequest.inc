<?php
import('/core/shared/class.RequestComponent.inc');

class GroupsRequest extends RequestComponent
{
    protected function do_SELECT($data = array())
    {
        //select
        $query = SQLQuery::doSelect()
            ->table('groups')
            ->whereWhenSet('group_id', $data['group_id'])
            ->whereWhenSet('group_name', htmlentities($data['group_name']))
            ->orderBy("group_name");
        
        if(isset($data['user_id']) || isset($data['user_name']) || isset($data['user_pass']))
        {
            $query->join('usersgroups', 'groups.group_id', 'usersgroups.group_id')
                ->join('users', 'users.user_id', 'usersgroups.user_id')
                ->whereWhenSet('users.user_id', $data['user_id'])
                ->whereWhenSet('user_name', $data['user_name'])
                ->whereWhenSet('user_pass', $data['user_pass']);
        }
        
        $result = $query->exec();
        
        return $result->getRows();
    }
    protected function do_DELETE($data = array())
    {
        //delete
        $result = SQLQuery::doDelete()
            ->where('group_id', $data['group_id'])
            ->table('groups')
            ->exec();
        
        return true;
    }
    protected function do_UPDATE($data = array())
    {
        //update
        $query = SQLQuery::doUpdate()
            ->updateWhenSet('group_name', $data['group_name'])
            ->updateWhenSet('group_privileges', $data['group_privileges'])
            ->where('group_id', $data['group_id'])
            ->table('groups')
            ->exec();
        
        return true;
    }
    protected function do_INSERT($data = array())
    {
        //insert
        $result = SQLQuery::doInsert('group_name', $data['group_name'])
            ->insert('group_privileges ', $data['group_privileges'])
            ->table('groups')
            ->exec();
        
        return $result->insertID();
    }
}
?>