<?php
/**
 * Aggregates News form RSS feeds and imports them into the news component
 *
 * @author Yannick <yannick.l.88@gmail.com>
 */
class AggregatorComponent extends Component
{
    /**
     * Constructor
     */
    public function __construct()
    {
        parent::__construct('aggregator');
    }
    /**
     * (non-PHPdoc)
     * @see core/Component#execCron()
     */
    public function execCron()
    {
        $xml = new SimpleXMLElement(file_get_contents(Config::get("blogurl", false, "aggregator")));

        $lastElement = Table::init("news.news")
            ->setRequest((object) array("news_external" => 1, "news_extsite" => "hyves"))
            ->doSelect()
            ->getRow();

        foreach($xml->channel->item as $item)
        {
            $date = strtotime($item->pubDate[0]);

            $msg = trim($item->description);

            $data = array();
            $data['news_title'] = trim($item->title[0]);
            $data['news_user'] = Config::get("bloguser", 0, "hyves");
            $data['news_text'] = $msg;
            $data['news_date'] = date("c", $date);
            $data['news_tag'] = "external";
            $data['news_external'] = 1;
            $data['news_extsite'] = "aggragator";
            $data['news_extlink'] = trim($item->link[0]);

            if($lastElement != null && $lastElement->news_title == $data['news_title'])
            {
                break;
            }

            Table::init("news.news")
                ->setRecord((object) $data)
                ->doInsert();
        }
    }
}