<?php
include_once Config::get('websiteroot', '.').'/../core/shared/class.RequestComponent.inc';

class NewsRequest extends RequestComponent
{
    //CORE ACTIONS
    protected function do_SELECT($data = array())
    {
        //select
        $query = SQLQuery::doSelect()
            ->table('news')
            ->join('users', 'news_user', 'user_id') //relation with users table
            ->orderby('news_date', 'DESC')
            ->orderby('news_id', 'DESC');
        
        if(isset($data['news_id']) && (is_numeric($data['news_id']) || is_array($data['news_id'])))
        {
            $query->where('news_id', $data['news_id']);
        }
        else if(isset($data['news_user']) && is_numeric($data['news_user']))
        {
            $query->where('news_user', $data['news_user']);
        }
        
        if(isset($data['max']))
        {
            $query->limit($data['max']);
        }
        
        $result = $query->exec();
        
        return $result->getRows();
    }
    protected function do_DELETE($data = array())
    {
        //delete
        $result = SQLQuery::doDelete()
            ->where('news_id', $data['news_id'])
            ->table('news')
            ->exec();
        
        return true;
    }
    protected function do_UPDATE($data = array())
    {
        //update
        $query = SQLQuery::doUpdate()
            ->where('news_id', $data['news_id'])
            ->table('news')
            ->updateWhenSet('news_title', $data['news_title'])
            ->updateWhenSet('news_user', $data['news_user'])
            ->updateWhenSet('news_text', $data['news_text'])
            ->updateWhenSet('news_date', $data['news_date'])
            ->exec();
        
        return true;
    }
    protected function do_INSERT($data = array())
    {
        //insert
        $result = SQLQuery::doInsert('news_title', $data['news_title'])
            ->insert('news_user', $data['news_user'])
            ->insert('news_text', $data['news_text'])
            ->insert('news_date', $data['news_date'])
            ->table('news')
            ->exec();
        
        return $result->insertID();
    }
}
?>