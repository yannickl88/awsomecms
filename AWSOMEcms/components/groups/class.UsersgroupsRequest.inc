<?php
import('/core/shared/class.RequestComponent.inc');

class UsersgroupsRequest extends RequestComponent
{
    protected function do_SELECT($data = array())
    {
        //select
        $result = SQLQuery::doSelect()
            ->table('usersgroups')
            ->whereWhenSet('group_id', $data['group_id'])
            ->whereWhenSet('user_id', $data['user_id'])
            ->exec();
        
        return $result->getRows();
    }
    protected function do_DELETE($data = array())
    {
        //delete
        $result = SQLQuery::doDelete()
            ->whereWhenSet('group_id', $data['group_id'])
            ->whereWhenSet('user_id', $data['user_id'])
            ->table('usersgroups')
            ->exec();
        
        return true;
    }
    protected function do_UPDATE($data = array())
    {
        //update
        $query = SQLQuery::doUpdate()
            ->updateWhenSet('group_id', $data['group_id'])
            ->updateWhenSet('user_id', $data['user_id'])
            ->where('group_id', $data['groupold_id'])
            ->where('user_id', $data['userold_id'])
            ->table('usersgroups')
            ->exec();
        
        return true;
    }
    protected function do_INSERT($data = array())
    {
        //insert
        $result = SQLQuery::doInsert('group_id', $data['group_id'])
            ->insert('user_id', $data['user_id'])
            ->table('usersgroups')
            ->exec();
        
        return $result->insertID();
    }
}
?>