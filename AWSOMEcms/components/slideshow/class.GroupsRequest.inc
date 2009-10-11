<?php
import('/core/shared/class.RequestComponent.inc');

class GroupsRequest extends RequestComponent
{
    protected function do_SELECT($data = array())
    {
        //select
        $query = SQLQuery::doSelect()
            ->join('s_images', 's_groups.group_header', 's_images.image_id')
            ->table('s_groups')
            ->orderby('group_title');
        
        if(isset($data['group_id']) && (is_numeric($data['group_id']) || is_array($data['group_id'])))
        {
            $query->where('group_id', $data['group_id']);
        }
        
        if(isset($data['max']) && is_numeric($data['max']))
        {
            $query->limit($data['max']);
        }
        
        $result = $query->exec();
        
        $rows = $result->getRows();
        
        foreach($rows as &$row)
        {
            $row->images = Request::init('groupsimages')->doSelect(array("group_id" => $row->group_id));
        }
        
        return $rows;
    }
    protected function do_DELETE($data = array())
    {
        //delete
        $result = SQLQuery::doDelete()
            ->where('group_id', $data['group_id'])
            ->table('s_groups')
            ->exec();
        
        return true;
    }
    protected function do_UPDATE($data = array())
    {
        //update
        $result = SQLQuery::doUpdate()
            ->updateWhenSet('group_title', $data['group_title'])
            ->updateWhenSet('group_header', $data['group_header'])
            ->where('group_id', $data['group_id'])
            ->table('s_groups')
            ->exec();
        
        return true;
    }
    protected function do_INSERT($data = array())
    {
        //insert
        $result = SQLQuery::doInsert()
            ->insert('group_title', $data['group_title'])
            ->insert('group_header', $data['group_header'])
            ->table('s_groups')
            ->exec();
        
        return $result->insertID();
    }
}
?>