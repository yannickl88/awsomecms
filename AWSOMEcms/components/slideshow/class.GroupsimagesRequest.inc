<?php
import('/core/shared/class.RequestComponent.inc');

class GroupsimagesRequest extends RequestComponent
{
    protected function do_SELECT($data = array())
    {
        //select
        $query = SQLQuery::doSelect()
            ->table('s_groups_images')
            ->join('s_groups', 's_groups_images.group_id', 's_groups.group_id', 'left')
            ->join('s_images', 's_groups_images.image_id', 's_images.image_id', 'left');
        
        if(isset($data['group_id']) && (is_numeric($data['group_id']) || is_array($data['group_id'])))
        {
            $query->where('s_groups_images.group_id', $data['group_id']);
        }
        if(isset($data['image_id']) && (is_numeric($data['image_id']) || is_array($data['image_id'])))
        {
            $query->where('s_groups_images.image_id', $data['image_id']);
        }
        
        $result = $query->exec();
        
        return $result->getRows();
    }
    protected function do_DELETE($data = array())
    {
        //delete
        $result = SQLQuery::doDelete()
            ->whereWhenSet('group_id', $data['group_id'])
            ->whereWhenSet('image_id', $data['image_id'])
            ->table('s_groups_images')
            ->exec();
        
        return true;
    }
    protected function do_UPDATE($data = array())
    {
        //update
        $result = SQLQuery::doUpdate()
            ->updateWhenSet('group_id', $data['group_id'])
            ->updateWhenSet('image_id', $data['image_id'])
            ->where('group_id', $data['group_id'])
            ->where('image_id', $data['image_id'])
            ->table('s_groups_images')
            ->exec();
        
        return true;
    }
    protected function do_INSERT($data = array())
    {
        //insert
        $result = SQLQuery::doInsert()
            ->insert('group_id', $data['group_id'])
            ->insert('image_id', $data['image_id'])
            ->table('s_groups_images')
            ->exec();
        
        return $result->insertID();
    }
}
?>