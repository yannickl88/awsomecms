<?php
import('/core/class.RequestComponent.inc');

class PageRequest extends RequestComponent
{
    //CORE ACTIONS
    protected function do_SELECT($data = array())
    {
        //select
        $query = SQLQuery::doSelect()
            ->table('pages')
            ->orderby('page_name');
        
        if(isset($data['page_id']) && is_numeric($data['page_id']))
        {
            $query->where('page_id', $data['page_id']);
        }
        else if(isset($data['location']))
        {
            $query->where('page_location', htmlentities($data['location']));
        }
        else if(isset($data['where']))
        {
            $query->where($data['where'][0], $data['where'][1], $data['where'][2]);
        }
        
        $result = $query->exec();
        
        return $result->getRows();
    }
    protected function do_DELETE($data = array())
    {
        //delete
        $result = SQLQuery::doDelete()
            ->where('page_id', $data['page_id'])
            ->table('pages')
            ->exec();
        
        return true;
    }
    protected function do_UPDATE($data = array())
    {
        //update
        $query = SQLQuery::doUpdate()
            ->where('page_id', $data['page_id'])
            ->table('pages');
        
        $query->updateWhenSet('page_name', $data['page_name']);
        $query->updateWhenSet('page_location', $data['page_location']);
        $query->updateWhenSet('page_template', $data['page_template']);
        
        if(isset($data['page_timestamp']))
        {
            $query->update('page_timestamp', $data['page_timestamp']);
        }
        else
        {
            $query->update('page_timestamp', date('c'));
        }
        
        $query->exec();
        
        return true;
    }
    protected function do_INSERT($data = array())
    {
        //insert
        $result = SQLQuery::doInsert('page_location', $data['page_location'])
            ->insert('page_name', $data['page_name'])
            ->insert('page_template', $data['page_template'])
            ->insert('page_timestamp', date('c'))
            ->table('pages')
            ->exec();
        
        return $result->insertID();
    }
}
?>