<?php
import('/core/shared/class.Component.inc');

class PageComponent extends Component
{
    public function __construct()
    {
        parent::__construct('page');
        
        $this->fields = array(
            "page_id" => "index",
            "page_name" => "text",
            "page_location" => "text",
            "page_template" => "text"
        );
    }
    
    public function action_index($smarty, $smartyLoader, $request)
    {
        
    }
    
    public function action_add($smarty, $smartyLoader, $request)
    {
        if($request['methode'] == 'post')
        {
            if($this->validate($request, "/".Config::get('pageadd', 'pageadd', 'admin'), "add"))
            {
                $id = $this->insert($request);
            }
            
            $this->redirect("/".Config::get('pageedit', 'pageedit', 'admin'), array("page_id" => $id));
        }
    }
    
    public function action_edit($smarty, $smartyLoader, $request)
    {
        if($request['methode'] == 'post')
        {
            if($this->validate($request, "/".Config::get('pageedit', 'pageedit', 'admin'), "edit"))
            {
                $this->update($request);
            }
            
            $this->redirect("/".Config::get('pageedit', 'pageedit', 'admin'), array("page_id" => $request['page_id']));
        }
        else
        {
            if($request['page_id'])
            {
                $record = $this->select($request);
                
                $smarty->assign("record", $record[0]);
            }
            else
            {
                $this->redirect("/".Config::get('pageadd', 'pageadd', 'admin'));
            }
        }
    }
    
    public function action_delete($smarty, $smartyLoader, $request)
    {
        if($request['methode'] == 'post')
        {
            $this->delete($request);
            
            $this->redirect("/".Config::get('pageadd', 'pageadd', 'admin'));
        }
        else
        {
            if($request['page_id'])
            {
                $record = $this->select($request);
                
                $smarty->assign("record", $record[0]);
            }
            else
            {
                $this->redirect("/".Config::get('pageadd', 'pageadd', 'admin'));
            }
        }
    }
    public function registerMenuItems($menu)
    {
        $menu->addChild(new MenuItem("page", "Pages", ""));
        $menu->addChild(new MenuItem("addpage", "Add Page", Config::get('pageadd', 'pageadd', 'admin'), "page"));
    }
}