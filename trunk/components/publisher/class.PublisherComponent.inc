<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.publish
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */
import('Component');
import('publisher.util.PublishItem');
/**
 * A way to publish items onto social networks
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class PublisherComponent extends Component
{
    private $publishers;
    private $published;
    
    /**
     * Constructor
     */
    public function __construct()
    {
        parent::__construct('publisher');
    }
    /**
     * (non-PHPdoc)
     * @see core/Component::register()
     *
     * @param RegisterManager $registerManager
     */
    public function register($registerManager)
    {
        //register stuff 
        $registerManager->registerPage(array($this, 'admin'), '{form component="publisher" form="admin"}');
    }
    /**
     * (non-PHPdoc)
     * @see core/Component::registerMenuItems()
     *
     * @param Menu $menu
     */
    public function registerMenuItems($menu)
    {
        $menu->addChild(new MenuItem("publish", array($this, 'admin')));
        $menu->addChild(new MenuItem("publishcontent", array($this, 'admin'), "publish"));
    }
    /**
     * Show a list of items you can publish
     * 
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_admin($smarty, $smartyLoader, $request)
    {
        $elements = array();
        
        //news
        if(RegisterManager::getInstance()->hasComponent("news"))
        {
            $news = Table::init("news.news")->doSelect();
            while($row = $news->getRow())
            {
                $elements[] = $this->wrap("news.news", $row);
            }
        }
        //images
        if(RegisterManager::getInstance()->hasComponent("file"))
        {
            $files = Table::init("file.files")->doSelect();
            while($row = $files->getRow())
            {
                $elements[] = $this->wrap("file.files", $row);
            }
        }
        
        usort($elements, array($this, "sort"));
        
        $smarty->assign("publishers", $this->getPublishers());
        $smarty->assign("items", $elements);
    }
    /**
     * publish action
     * 
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_publish($smarty, $smartyLoader, $request)
    {
        $idInfo = explode("::", $request['id']);
    
        $table = Table::init($idInfo[1]);
        $keys = $table->getKeys();
        $result = $table->setRequest((object) array($keys[0]->getName() => $idInfo[2]))
            ->doSelect()
            ->getRow();
        
        $publishers = $this->getPublishers();
        $item = $this->wrap($idInfo[1], $result);
        $succes = $publishers[$request['to']]->publish($request, $item);
        
        if($succes)
        {
            //insert record
            Table::init("publisher.published")
                ->setRecord((object) array(
                    "publish_link" => $item->id,
                    "publish_to" => $request['to'],
                    "publish_date" => date('c')
                ))
                ->doInsert();
            
            $html = '<html><head><script type="text/javascript"> window.opener.publish_done(window, "'.$item->id.'"); </script></head><body></body></html>';
        }
        else 
        {
            $html = '<html><head><script type="text/javascript"> window.close(); </script></head><body></body></html>';
        }
        Controller::getInstance()->setOutput($html);
    }
    /**
     * Authentication action, this is needed for oauth calls
     * 
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param Array $request
     */
    public function action_auth($smarty, $smartyLoader, $request)
    {
        $publishers = $this->getPublishers();
        $this->redirect($publishers[$request['to']]->auth($request));
    }
    /**
     * Get all published items
     * 
     * @return array
     */
    private function getPublishedItems()
    {
        if(!$this->published)
        {
            $this->published = array();
            $items = Table::init("publisher.published")->doSelect()->getRows();
            
            foreach($items as $item)
            {
                $this->published[$item->publish_link] = $item;
            }
        }
        
        return $this->published;
    }
    /**
     * Wrap a row in a PublishItem
     * 
     * @param String $table
     * @param Object $row
     * @return PublishItem
     */
    private function wrap($table, $row)
    {
        $published = $this->getPublishedItems();
        
        switch($table)
        {
            case "news.news":
                $newsComponent = Component::init("news");
                $item = new PublishItem(
                    $table.'::'.$row->news_id, 
                    $row->news_title, 
                    $row->news_text, 
                    Config::get("baseurl", "http://google.com")."/".$newsComponent->makeURL($row).$row->news_titleurl, 
                    strtotime($row->news_date), 
                    "", 
                    PublishItem::NEWS
                );
                break;
            case "file.files":
                $item = new PublishItem(
                    $table.'::'.$row->file_id, 
                    $row->file_name, 
                    $row->file_description." ".Config::get("baseurl", "http://www.google.com").$row->file_data->url, 
                    Config::get("baseurl", "http://google.com").$row->file_data->url, 
                    strtotime($row->file_date), 
                    Config::get("baseurl", "http://google.com").$row->file_data->thumb, 
                    PublishItem::IMAGE
                );
                break;
            default:
                return false;
                break;
        }
        list($item->publishers, $item->published) = $this->getPublishSettings($item->type, $item);
        return $item;
    }
    /**
     * Get the publish settings for an type and item
     * 
     * @param String $type
     * @param PublishItem $item
     * @return array
     */
    private function getPublishSettings($type, $item)
    {
        $published = new DataContainer();
        $publishers = new DataContainer();
        
        $publishersList = $this->getPublishers();
        $publishedList = $this->getPublishedItems();
        
        foreach($publishersList as $name => $publisher)
        {
            $publishers->$name = $publisher->canPublish($type);
            
            if(isset($publishedList[$item->id]) && $publishedList[$item->id]->publish_to == $name)
            {
                $published->$name = true;
            }
            else
            {
                $published->$name = false;
            }
        }
        
        return array($publishers, $published);
    }
    /**
     * Get a list of publishers found in the publishers folder
     * 
     * @return array
     */
    private function getPublishers()
    {
        if(!$this->publishers)
        {
            $this->publishers = array();
            
            $files = scandir($this->location."/publishers/");
            
            foreach($files as $file)
            {
                $matches = array();
                if(preg_match('/^class\.(.*)Publisher\.inc$/', $file, $matches) === 1)
                {
                    $className = $matches[1]."Publisher";
                    
                    import($this->location."/publishers/".$file);
                    
                    $this->publishers[strtolower($matches[1])] = new $className();
                }
            }
        }
        
        return $this->publishers;
    }
    /**
     * sort function
     * 
     * @param PublishItem $a
     * @param PublishItem $b
     * @return int
     */
    private function sort($a, $b)
    {
        if ($a->date == $b->date)
            return 0;
        
        return ($a->date < $b->date) ? 1 : -1;
    }
}