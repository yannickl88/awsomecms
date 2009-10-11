<?php
import('/core/shared/class.RequestComponent.inc');

class UsersRequest extends RequestComponent
{
    protected function do_SELECT($data = array())
    {
        //select
        $query = SQLQuery::doSelect()
            ->table('users');
        
        if(isset($data['user_id']) && (is_numeric($data['user_id']) || is_array($data['news_id'])))
        {
            $query->where('user_id', $data['user_id']);
        }
        if(isset($data['user_name']))
        {
            $query->where('user_name', htmlentities($data['user_name']));
        }
        if(isset($data['user_pass']))
        {
            $query->where('user_pass', htmlentities($data['user_pass']));
        }
        
        $result = $query->exec();
        
        return $result->getRows();
    }
    protected function do_DELETE($data = array())
    {
        //delete
        $result = SQLQuery::doDelete()
            ->where('user_id', $data['user_id'])
            ->table('users')
            ->exec();
        
        return true;
    }
    protected function do_UPDATE($data = array())
    {
        //update
        $query = SQLQuery::doUpdate()
            ->updateWhenSet('user_name', $data['user_name'])
            ->where('user_id', $data['user_id'])
            ->table('users');
        if($data['user_pass'])
        {
            $query->update('user_pass', md5($data['user_pass']));
        }
        
        $query->exec();
        
        return true;
    }
    protected function do_INSERT($data = array())
    {
        //insert
        $result = SQLQuery::doInsert('user_name', $data['user_name'])
            ->insert('user_pass', md5($data['user_pass']))
            ->table('users')
            ->exec();
        
        return $result->insertID();
    }
}
?>