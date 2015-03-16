<?php
/**
 * Connector for twitpic.com
 *
 * @author Yannick de Lange <yannick.l.88@gmail.com>
 */
class TwitpicConnector implements FileConnector
{
    /**
     * (non-PHPdoc)
     * @see components/file/util/FileConnector::fetch()
     *
     * @return array
     */
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

        $loc = Config::get("websiteroot", "/")."/";
        $folder = Config::get("uploaddir", "/", "file")."files/twitpic/";

        foreach($data->images as $image)
        {
            if(!isset($mapping[$image->id]))
            {
                //download the file and thumb
                $filename = $folder.$image->short_id.".".$image->type;
                $filenameThumb = $folder.$image->short_id."_thumb.".$image->type;

                //is there an upload folder?
                if(!file_exists($loc.$folder))
                {
                    mkdir($loc.$folder, 0777, true);
                }

                file_put_contents($loc.$filename, file_get_contents("http://twitpic.com/show/full/".$image->short_id));
                file_put_contents($loc.$filenameThumb, file_get_contents("http://twitpic.com/show/thumb/".$image->short_id));

                $row = new stdClass();
                $row->file_name = $image->short_id;
                $row->file_data = (object) array(
                    "filename" => $image->id,
                    "name" => $image->short_id,
                    "type" => "image/".$image->type,
                    "size" => $image->size,
                    "url" => $filename,
                    "thumb" => $filenameThumb
                );
                $row->file_date = $image->timestamp;
                $row->file_tag = implode(", ", $this->getTags($image->message));
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
            set_time_limit(60);
        }

        $diff = array_diff(array_keys($mapping), $foundImages);

        foreach($diff as $deleted)
        {
            $table->setRequest($mapping[$deleted])->doDelete();

            //remove the images
            if(file_exists($loc.$mapping[$deleted]->file_data->url))
                unlink($loc.$mapping[$deleted]->file_data->url);
            if(file_exists($loc.$mapping[$deleted]->file_data->thumb))
                unlink($loc.$mapping[$deleted]->file_data->thumb);
        }
        return $return;
    }
    /**
     * (non-PHPdoc)
     * @see components/file/util/FileConnector::walk()
     *
     * @param $row
     */
    public function walk($row)
    {
        $row->permissions = array(
            "view" => true,
            "edit" => false,
            "delete" => false
        );
    }
    /**
     * (non-PHPdoc)
     * @see components/file/util/FileConnector::isActive()
     *
     * @return bool
     */
    public function isActive()
    {
        return Config::get("twitpic_user", "", "connectors") !== "";
    }
    /**
     * Parse a post for tags
     *
     * @param $tweet
     * @return array
     */
    private function getTags($tweet)
    {
        $matches = array();

        preg_match_all('/#([\w]*)/', $tweet, $matches);
        return $matches[1];
    }
}