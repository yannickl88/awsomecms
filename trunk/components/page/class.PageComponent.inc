<?php
import('/core/class.Component.inc');

class PageComponent extends Component
{
    public function __construct()
    {
        parent::__construct('page');
    }

    public function action_add($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            $id = Table::init('page.pages')
                ->setRecord((object) $request)
                ->doInsert();

            $this->redirect("/".Config::get('pageedit', 'pageedit', 'admin'), array("page_id" => $id));
        }
    }

    public function action_edit($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            Table::init('page.pages')
                ->setRecord((object) $request)
                ->doUpdate();

            $this->redirect("/".Config::get('pageedit', 'pageedit', 'admin'), array("page_id" => $request['page_id']));
        }
        else
        {
            if($request['page_id'])
            {
                Table::init('page.pages')
                    ->setRecord((object) $request);
            }
            else
            {
                $this->redirect("/".Config::get('pageadd', 'pageadd', 'admin'));
            }
        }
    }

    public function action_delete($smarty, $smartyLoader, $request)
    {
        if($request['method'] == 'post')
        {
            Table::init('page.pages')
                ->setRecord((object) $request)
                ->doDelete();

            $this->redirect("/admin/");
        }
        else
        {
            if($request['page_id'])
            {
                $table = Table::init('page.pages')
                    ->setRecord((object) $request);
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