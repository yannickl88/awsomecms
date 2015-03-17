<?php
/**
 * Field for uploading files
 *
 * @author Yannick <yannick.l.88@gmail.com>
 */
class GridSelectField extends IntField
{
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_add()
     */
    protected function toHTML_add()
    {
        $grids = Table::init("imagegrid.imagegrid")
            ->doSelect();

        $html = "<select id=\"{$this->getName()}\" name=\"{$this->getName()}\" >";
        while($row = $grids->getRow())
        {
            $html .= "<option value=\"{$row->grid_id}\">{$row->grid_name}</option>";
        }
        $html .= "</select>";
        return $html;
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_edit()
     */
    protected function toHTML_edit()
    {
        $grids = Table::init("imagegrid.imagegrid")
            ->doSelect();

        $html = "<select id=\"{$this->getName()}\" name=\"{$this->getName()}\" >";
        while($row = $grids->getRow())
        {
            if($row->grid_id == $this->getValue())
            {
                $html .= "<option selected=\"selected\" value=\"{$row->grid_id}\">{$row->grid_name}</option>";
            }
            else
            {
                $html .= "<option value=\"{$row->grid_id}\">{$row->grid_name}</option>";
            }
        }
        $html .= "</select>";
        return $html;
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_edit()
     */
    protected function toHTML_view()
    {
        $grid = Table::init("imagegrid.imagegrid")
            ->setRequest((object) array("grid_id" => $this->getValue()))
            ->doSelect()
            ->getRow();

        return $grid->grid_name;
    }
}