<?php
/**
 * @author Yannick <yannick.l.88@gmail.com>
 */
class BlockTable extends Table
{
    /**
     * constructor
     */
    public function __construct()
    {
        parent::__construct("block");
        $this->assign("imagegrid");

        $componentObj = Component::init($this->component);

        $this->adminURL = makeLink(array($componentObj, "adminblock"));
        $this->addURL = makeLink(array($componentObj, "addblock"));
        $this->editURL = makeLink(array($componentObj, "editblock"));
        $this->deleteURL = makeLink(array($componentObj, "deleteblock"));

        $this->addField(new IntField("block_id", Field::INDEX|Field::AUTOKEY));
        $this->addField(new MLangField("block_title"), 1);
        $this->addField(new MLangTextField("block_text", Field::OPTIONAL|Field::FORCEUPDATE), 2);

        $file_select = new FileSelectField("block_image", array("jpg", "jpeg", "png", "gif"));
        $file_select->setMultiSelect(false);

        $this->addField($file_select, 3);
        $this->addField(new GridSelectField("block_grid"), 4);
        $this->addField(new LocationField("block_url", 0, true), 5);
        $this->setTitle("[block_title]");
        $this->setGrouping('block_grid');
    }
    /**
     * (non-PHPdoc)
     * @see core/Table#select($query, $data)
     */
    protected function select($query, $data = null)
    {
        $query->orderby('block_title', "DESC");

        if($data)
        {
            $query->whereWhenSet("block_id", $data->block_id);
            $query->whereWhenSet("block_grid", $data->block_grid);
        }
    }
    /**
     * @see Table::update()
     */
    protected function update($query, $data = null) { }
    /**
     * @see Table::insert()
     */
    protected function insert($query, $data = null) { }
    /**
     * @see Table::delete()
     */
    protected function delete($query, $data = null) { }

    /**
     * @see core/Table#getAction($mode)
     *
     * Since this tabel is assigned to the users, we need a different set of actions.
     * So we override this function to accomodate for that.
     */
    public function getAction($mode)
    {
        if(!is_numeric($mode))
        {
            $mode = $this->string2action($mode);
        }

        switch($mode)
        {
        	case Field::EDIT:
        	    return "editblock";
        	    break;
        	case Field::DELETE:
        	    return "deleteblock";
        	    break;
        	case Field::ADD:
        	    return "addblock";
        	    break;
        	default:
        	    return "adminblock";
        	    break;
        }
    }
}