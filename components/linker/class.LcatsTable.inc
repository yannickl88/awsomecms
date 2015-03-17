<?php
/**
 * Lcat table, this handels all interaction with the links categories table in the database
 *
 * @author Yannick <yannick.l.88@gmail.com>
 */
class LcatsTable extends Table
{
    /**
     * constructor
     */
    public function __construct()
    {
        parent::__construct("lcats");
        $this->assign("linker");

        $componentObj = Component::init($this->component);

        $this->adminURL = makeLink(array($componentObj, "lcadmin"));
        $this->addURL = makeLink(array($componentObj, "lcadd"));
        $this->editURL = makeLink(array($componentObj, "lcedit"));
        $this->deleteURL = makeLink(array($componentObj, "lcdelete"));

        $this->addField(new IntField("lcat_id", Field::INDEX|Field::AUTOKEY));
        $this->addField(new MLangField("lcat_name"), 1);
        $this->addField(new Field("lcat_urlname", Field::HIDE|Field::OPTIONAL));
        $this->addField(new UploadField("lcat_image", Config::get("uploaddir", "/", "file")."catlogos/"), 2);
        $this->addField(new MLangTextField("lcat_description"), 3);
        $this->addField(new UploadField("lcat_bg", Config::get("uploaddir", "/", "file").'bg/', Field::OPTIONAL), 4);
        $this->addField(new BooleanField("lcat_hidden"), 5);
        $this->setTitle("[lcat_name]");
    }
    /**
     * (non-PHPdoc)
     * @see core/Table#select($query, $data)
     */
    protected function select($query, $data = null)
    {
        $query->orderby('lcat_id', "ASC");

        if($data)
        {
            if(isset($data->hideHidden) && $data->hideHidden)
                $query->where("lcat_hidden", 0, "=", "AND", true);

            $query->whereWhenSet("lcat_id", $data->lcat_id);
            $query->whereWhenSet("lcat_urlname", $data->lcat_urlname);
        }
    }
    /**
     * (non-PHPdoc)
     * @see core/Table::insert()
     */
    protected function insert($query, $data = null)
    {
        $query->insert("lcat_urlname", urlEscape($data['lcat_name']["en"]));
    }
    protected function update($query, $data = null)
    {
        $query->update("lcat_urlname", urlEscape($data['lcat_name']["en"]));
    }
    protected function delete($query, $data = null) {}

    public function getAction($mode)
    {
        if(!is_numeric($mode))
        {
            $mode = $this->string2action($mode);
        }

        switch($mode)
        {
            case Field::EDIT:
                return "lcedit";
                break;
            case Field::DELETE:
                return "lcdelete";
                break;
            case Field::ADD:
            default:
                return "lcadd";
                break;
        }
    }
}