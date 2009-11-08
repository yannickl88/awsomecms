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
        return true;
    }
    protected function do_UPDATE($data = array())
    {
        return true;
    }
    protected function do_INSERT($data = array())
    {
        return 0;
    }
}
?>