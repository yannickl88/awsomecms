<?php
import('/core/shared/class.RequestComponent.inc');

class CommentsRequest extends RequestComponent
{
    protected function do_SELECT($data = array())
    {
        //select
        $result = SQLQuery::doSelect()
            ->orderby("comment_time", 'DESC')
            ->table('comments')
            ->whereWhenSet('comment_hook', $data['hook'])
            ->exec();
        
        return $result->getRows();
    }
    protected function do_DELETE($data = array())
    {
        //delete
        $result = SQLQuery::doDelete()
            ->where('comment_id', $data['comment_id'])
            ->table('comments')
            ->exec();
        
        return true;
    }
    protected function do_UPDATE($data = array())
    {
        //update
        $query = SQLQuery::doUpdate()
            ->updateWhenSet('comment_text', $data['comment_text'])
            ->updateWhenSet('comment_username', $data['comment_username'])
            ->updateWhenSet('comment_hook', $data['comment_hook'])
            ->updateWhenSet('comment_time', $data['comment_time'])
            ->updateWhenSet('comment_userid', $data['comment_userid'])
            ->where('comment_id', $data['comment_id'])
            ->table('comments')
            ->exec();
        
        return true;
    }
    protected function do_INSERT($data = array())
    {
        //insert
        $result = SQLQuery::doInsert()
            ->insert('comment_text', $data['comment_text'])
            ->insert('comment_username', $data['comment_username'])
            ->insert('comment_hook', $data['comment_hook'])
            ->insert('comment_time', $data['comment_time'])
            ->insertWhenSet('comment_userid', $data['comment_userid'])
            ->table('comments')
            ->exec();
        
        return $result->insertID();
    }
}
?>