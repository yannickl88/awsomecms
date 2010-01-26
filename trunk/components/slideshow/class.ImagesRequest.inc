<?php
import('/core/shared/class.RequestComponent.inc');

class ImagesRequest extends RequestComponent
{
    protected function do_SELECT($data = array())
    {
        //select
        $query = SQLQuery::doSelect()
            ->table('s_images')
            ->orderby('image_title');
        
        if(isset($data['image_id']) && (is_numeric($data['image_id']) || is_array($data['image_id'])))
        {
            $query->where('image_id', $data['image_id']);
        }
        
        if(isset($data['max']) && is_numeric($data['max']))
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
            ->where('image_id', $data['image_id'])
            ->table('s_images')
            ->exec();
        
        return true;
    }
    protected function do_UPDATE($data = array())
    {
        //update
        $query = SQLQuery::doUpdate()
            ->updateWhenSet('image_title', $data['image_title'])
            ->updateWhenSet('image_url', $data['image_url'])
            ->updateWhenSet('image_width', $data['image_width'])
            ->updateWhenSet('image_height', $data['image_height'])
            ->updateWhenSet('image_size', $data['image_size'])
            ->where('image_id', $data['image_id'])
            ->table('s_images');
        
        $query->exec();
        
        return true;
    }
    protected function do_INSERT($data = array())
    {
        //insert
        $result = SQLQuery::doInsert('image_title', $data['image_title'])
            ->insert('image_url', $data['image_url'])
            ->insertWhenSet('image_width', $data['image_width'])
            ->insertWhenSet('image_height', $data['image_height'])
            ->insertWhenSet('image_size', $data['image_size'])
            ->table('s_images')
            ->exec();
        
        return $result->insertID();
    }
}
?>