<?php
import('/core/class.Component.inc');

class NewsComponent extends Component
{
    public function __construct()
    {
        parent::__construct('news');
    }
    
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
    
    public function registerMenuItems($menu)
    {
        $menu->addChild(new MenuItem("news", "", Config::get('newsadmin', 'newsadmin', 'admin')));
        $menu->addChild(new MenuItem("newsadmin", "Admin", Config::get('newsadmin', 'newsadmin', 'admin'), "news"));
        $menu->addChild(new MenuItem("newsadd", "Add", Config::get('newsadd', 'newsadd', 'admin'), "news"));
    }
}