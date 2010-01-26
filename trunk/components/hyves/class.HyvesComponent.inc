<?php
import('/core/shared/class.Component.inc');

class HyvesComponent extends Component
{
    public function __construct()
    {
        parent::__construct('hyves');
    }
    
    public function execCron()
    {
        $xml = new SimpleXMLElement(file_get_contents(Config::get("blogurl", false, "hyves")));
        
        $lastElement = Request::init("hyves.blog")->doSelect();
        $lastElement = $lastElement[0];
        
        foreach($xml->channel->item as $item)
        {
            $date = strtotime($item->pubDate[0]);
            
            $msg = trim($item->description);
            
            $data = array();
            $data['news_title'] = trim($item->title[0]);
            $data['news_user'] = Config::get("bloguser", 0, "hyves");
            $data['news_text'] = $msg;
            $data['news_date'] = date("c", $date);
            $data['news_tag'] = "hyves";
            $data['news_external'] = 1;
            $data['news_extlink'] = trim($item->link[0]);
            
            if($lastElement != null && $lastElement->news_title == $data['news_title'])
            {
                break;
            }
            
            Request::init("news")->doInsert($data);
        }
    }
}