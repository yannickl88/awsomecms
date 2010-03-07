<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.news
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */
import('/core/class.Component.inc');

/**
 * The News component provides a simple news system that allows for new messages. 
 * News items can be grouped using a tag if needed.
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class NewsComponent extends Component
{
    /**
     * Constructor
     */
    public function __construct()
    {
        parent::__construct('news');
    }
    /**
     * Index action, this is used when a page is shown by the cms
     *
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_index($smarty, $smartyLoader, $request)
    {
        $smarty->register_resource("scf", array(
            array($smartyLoader, "getTemplate"),
            array($smartyLoader, "getTimestamp"),
            array($smartyLoader, "getSecure"),
            array($smartyLoader, "getTrusted"),
        ));
        
        if(!empty($request['urlmapmatch']))
        {
            $record = Table::init("news.news")
                ->setRequest((object) array("news_titleurl" => $request['urlmapmatch'][3]))
                ->doSelect();
            
            if($record->count() == 1)
            {
                //check the date given
                $news = $record->getRow();
                $data = strtotime($news->news_date);
                
                if(date("m", $data) === $request['urlmapmatch'][2] && date("Y", $data) === $request['urlmapmatch'][1])
                {
                    $template = "news/news_item.tpl";
                    $smarty->assign("news", $news);
                    
                    //fetch the next and previous items
                    $news2 = Table::init("news.news")
                        ->setRequest((object) array())
                        ->doSelect()
                        ->getRows();
                    
                    $previtem = -1;
                    $nextitem = -1;
                    
                    foreach($news2 as $key => $newsitem)
                    {
                        if($newsitem->news_id == $news->news_id)
                        {
                            if($key < count($news2))
                            {
                                $previtem = $news2[$key + 1];
                            }
                            if($key > 0)
                            {
                                $nextitem = $news2[$key - 1];
                            }
                            break;
                        }
                    }
                    
                    $smarty->assign("nextnews", $nextitem);
                    $smarty->assign("prevnews", $previtem);
                    
                    echo $smartyLoader->getRenderedPage($smarty->fetch($template), $_GET['url'], $smarty);
                    return;
                }
            }
        }
        
        echo $smartyLoader->getPage($_GET['url'], $smarty);
    }
    /**
     * Action for adding a news item
     * 
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_add($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            Table::init("news.news")
                ->setRecord((object) $request)
                ->doInsert();
            
            $this->redirect("/".Config::get('newsadmin', 'newsadmin', 'admin'));
        }
    }
    /**
     * Action for editing a news item
     * 
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_edit($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            Table::init("news.news")
                ->setRecord((object) $request)
                ->doUpdate();
            
            $this->redirect("/".Config::get('newsadmin', 'newsadmin', 'admin'));
        }
        else
        {
            if($request['news_id'])
            {
                Table::init('news.news')
                    ->setRecord((object) $request);
            }
            else
            {
                $this->redirect("/".Config::get('newsadd', 'newsadd', 'admin'));
            }
        }
    }
    /**
     * Action for deleting a news item
     * 
     * @param Smarty $smarty
     * @param SmartyPageLoader $smartyLoader
     * @param array $request
     */
    public function action_delete($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            Table::init("news.news")
                ->setRecord((object) $request)
                ->doDelete();
            
            $this->redirect("/".Config::get('newsadmin', 'newsadmin', 'admin'));
        }
        else
        {
            if($request['news_id'])
            {
                Table::init('news.news')
                    ->setRecord((object) $request);
            }
            else
            {
                $this->redirect("/".Config::get('newsadd', 'newsadd', 'admin'));
            }
        }
    }
    /**
     * (non-PHPdoc)
     * @see core/Component#registerMenuItems($menu)
     */
    public function registerMenuItems($menu)
    {
        $menu->addChild(new MenuItem("news", Config::get('newsadmin', 'newsadmin', 'admin')));
        $menu->addChild(new MenuItem("newsadmin", Config::get('newsadmin', 'newsadmin', 'admin'), "news"));
        $menu->addChild(new MenuItem("newsadd", Config::get('newsadd', 'newsadd', 'admin'), "news"));
    }
    /**
     * (non-PHPdoc)
     * @see core/Component#registerTreeItems($tree)
     */
    public function registerTreeItems($tree)
    {
        $table = Table::init("news.news", "admintree");
        
        $result = $table
            ->doSelect()
            ->getRows();

        foreach($result as $row)
        {
            $title = $row->news_title;
            
            if(strlen($title) > 17)
            {
                $title = substr($title, 0, 15)."...";
            }
            
            $date = strtotime($row->news_date);
            
            $tree->addChild(new TreeItem(
                $title, 
                "/".Config::get("newsedit", "newsedit", "admin")."?news_id={$row->news_id}", 
                "/".Config::get("newsdelete", "newsdelete", "admin")."?news_id={$row->news_id}", 
                "/news/".date("Y/m", $date)."/".$row->news_titleurl
                ), 
                "news/".date("Y/m", $date)
            );
        }
    }
}