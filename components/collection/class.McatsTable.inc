<?php
/**
 * Lcat table, this handels all interaction with the links categories table in the database
 *
 * @author Yannick <yannick.l.88@gmail.com>
 */
class McatsTable extends Table
{
    /**
     * constructor
     */
    public function __construct()
    {
        parent::__construct("mcats");
        $this->assign("collection");

        $componentObj = Component::init($this->component);

        $this->adminURL = makeLink(array($componentObj, "mcadmin"));
        $this->addURL = null;
        $this->editURL = makeLink(array($componentObj, "mcedit"));
        $this->deleteURL = makeLink(array($componentObj, "mcdelete"));

        $this->addField(new IntField("mcat_id", Field::INDEX|Field::AUTOKEY));
        $this->addField(new TypeField("mcat_type", Field::HIDE_ADD|Field::HIDE_EDIT|Field::OPTIONAL_EDIT), 2);
        $this->addField(new MLangField("mcat_name"), 3);
        $this->addField(new MLangTextField("mcat_description", Field::HIDE_ADD|Field::OPTIONAL), 7);
        $this->addField(new Field("mcat_urlname", Field::HIDE|Field::OPTIONAL));
        $iconField = new UploadField("mcat_icon", Config::get("uploaddir", "/", "file"), Field::HIDE_ADD|Field::OPTIONAL_EDIT);
        $iconField->setResize(array(100, 50));
        $this->addField($iconField, 5);
        $this->addField(new UploadField("mcat_image", Config::get("uploaddir", "/", "file"), Field::HIDE_ADD|Field::OPTIONAL_EDIT), 6);
        $this->setTitle("[mcat_name]");

        $this->setOrder('mcat_name');
        $this->setGrouping("mcat_type");
    }
    /**
     * (non-PHPdoc)
     * @see core/Table#select($query, $data)
     */
    protected function select($query, $data = null)
    {
        $query->orderBy('mcat_type');

        if($data)
        {
            $query->whereWhenSet("mcat_id", $data->mcat_id);
            $query->whereWhenSet("mcat_type", $data->mcat_type);
            $query->whereWhenSet("mcat_urlname", $data->mcat_urlname);
        }
    }
    /**
     * (non-PHPdoc)
     * @see core/Table::insert()
     */
    protected function insert($query, $data = null)
    {
        $query->insert("mcat_urlname", urlEscape($data['mcat_name']["en"]));
    }
    protected function update($query, $data = null)
    {
        $query->update("mcat_urlname", urlEscape($data['mcat_name']["en"]));
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
                return "mcedit";
                break;
            case Field::DELETE:
                return "mcdelete";
                break;
            case Field::ADD:
            default:
                return "mcadd";
                break;
        }
    }
}