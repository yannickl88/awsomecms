<?php
import("file.util.FileConnector");

class TwitpicConnector implements FileConnector
{
    public function fetch()
    {
        $mapping = array();
        $lastID = null;
        $table = Table::init("file.files");
        $component = Component::init("file");
        
        //fetch all images
        $rows = $table
        ->setRequest((object) array("file_reference" => "twitpic"))
        ->doSelect();
        
        while($row = $rows->getRow())
        {
            $mapping[$row->file_data->filename] = $row;
            
            if($lastID == null || strtotime($lastID->file_date) < strtotime($row->file_date))
                $lastID = $row;
        }
        
        $data = json_decode(file_get_contents("http://api.twitpic.com/2/users/show.json?username=".Config::get("twitpic_user", "", "connectors")));
        $return = array();
        $foundImages = array();
        $locMapping = $component->getLocationMap();
        
        foreach($data->images as $image)
        {
            if(!isset($mapping[$image->id]))
            {
                $row = new stdClass();
                $row->file_name = $image->short_id;
                $row->file_data = (object) array(
                    "filename" => $image->id,
                    "name" => $image->short_id,
                    "type" => "image/".$image->type,
                    "size" => $image->size,
                    "url" =>  "http://twitpic.com/show/full/".$image->short_id,
                    "thumb" => "http://twitpic.com/show/thumb/".$image->short_id
                );
                $row->file_date = $image->timestamp;
                $row->file_tag = "";
                $row->file_description = $image->message;
                $row->file_reference = "twitpic";
                
                $row = $table->setRecord($row)
                ->doInsert()
                ->getRow();
                
                $row->file_id = $table->getInsertedID();
                $row->file_location = $locMapping[$component->getType($row->file_data->type)];
                $node = new FileItem($row);
                
                array_push($return, $node->toData());
            }
            array_push($foundImages, $image->id);
        }
        
        $diff = array_diff(array_keys($mapping), $foundImages);
        
        foreach($diff as $deleted)
        {
            $table->setRequest($mapping[$deleted])->doDelete();
        }
        return $return;
    }
    public function walk($row)
    {
        $row->permissions = array(
            "view" => true, 
            "edit" => false,
            "delete" => false
        );
    }
    public function isActive()
    {
        return Config::get("twitpic_user", "", "connectors") !== "";
    }
}