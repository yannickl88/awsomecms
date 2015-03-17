<?php
/**
 * @author Yannick <yannick.l.88@gmail.com>
 */
class ImagegridTable extends Table
{
    /**
     * constructor
     */
    public function __construct()
    {
        parent::__construct("imagegrid");
        $this->assign("imagegrid");

        $this->addField(new IntField("grid_id", Field::INDEX|Field::AUTOKEY));
        $this->addField(new Field("grid_name"), 1);
        $this->addField(new CollumnSelectField("grid_columns"), 2);
        $this->addField(new BooleanField("grid_fill"), 3);
        $this->setTitle("[grid_name]");
    }
    /**
     * (non-PHPdoc)
     * @see core/Table#select($query, $data)
     */
    protected function select($query, $data = null)
    {
        $query->orderby('grid_name', "ASC");

        if($data)
        {
            $query->whereWhenSet("grid_id", $data->grid_id);
            $query->whereWhenSet("grid_name", $data->grid_name);
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
}