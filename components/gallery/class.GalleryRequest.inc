<?php
import('/core/shared/class.RequestComponent.inc');

class GalleryRequest extends RequestComponent
{
    protected function do_SELECT($data = array())
    {
        //select
        $query = SQLQuery::doSelect()
            ->table('images')
            ->orderby('image_id', 'DESC');
        
        if(isset($data['image_id']) && (is_numeric($data['image_id']) || is_array($data['image_id'])))
        {
            $query->where('image_id', $data['image_id']);
        }
        
        if(isset($data['image_tag']))
        {
            $query->where('image_tag', $data['image_tag']);
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
            ->table('images')
            ->exec();
        
        return true;
    }
    protected function do_UPDATE($data = array())
    {
        //update
        $query = SQLQuery::doUpdate()
            ->updateWhenSet('image_title', $data['image_title'])
            ->updateWhenSet('image_url', $data['image_url'])
            ->updateWhenSet('image_urlthumb', $data['image_urlthumb'])
            ->updateWhenSet('image_width', $data['image_width'])
            ->updateWhenSet('image_height', $data['image_height'])
            ->updateWhenSet('image_thumbwidth', $data['image_thumbwidth'])
            ->updateWhenSet('image_thumbheight', $data['image_thumbheight'])
            ->updateWhenSet('image_tag', $data['image_tag'])
            ->where('image_id', $data['image_id'])
            ->table('images');
        
        $query->exec();
        
        return true;
    }
    protected function do_INSERT($data = array())
    {
        //insert
        $result = SQLQuery::doInsert('image_title', $data['image_title'])
            ->insert('image_url', $data['image_url'])
            ->insert('image_urlthumb', $data['image_urlthumb'])
            ->insert('image_tag', $data['image_tag'])
            ->insertWhenSet('image_width', $data['image_width'])
            ->insertWhenSet('image_height', $data['image_height'])
            ->insertWhenSet('image_thumbwidth', $data['image_thumbwidth'])
            ->insertWhenSet('image_thumbheight', $data['image_thumbheight'])
            ->table('images')
            ->exec();
        
        return $result->insertID();
    }
}
?>