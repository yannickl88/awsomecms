<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.hyves
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */
import('/core/class.Component.inc');

/**
 * This is more of an link to hyves, via a cron job blog post can be imported and added to the news component
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class HyvesComponent extends Component
{
    /**
     * Constructor
     */
    public function __construct()
    {
        parent::__construct('hyves');
    }
    /**
     * (non-PHPdoc)
     * @see core/Component#execCron()
     */
    public function execCron()
    {
        $xml = new SimpleXMLElement(file_get_contents(Config::get("blogurl", false, "hyves")));
        
        $lastElement = Table::init("news.news")
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
            $data['news_tag'] = "hyves";
            $data['news_external'] = 1;
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