<?php
/**
 * Field for selecting the link category
 *
 * @author Yannick <yannick.l.88@gmail.com>
 */
class LinkcatField extends IntField
{
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_add()
     */
    protected function toHTML_add()
    {
        $table = Table::init("linker.lcats");
        $cats = $table
            ->doSelect()
            ->getRows();

        $field = $table->getField("lcat_name");
        $html = "";
        $html .= "<select class='cmsfield' type='text' name='{$this->name}' id='{$this->name}'>";
        foreach($cats as $cat)
        {
            $field->setValue($cat->lcat_name);
            $html .= "<option value='{$cat->lcat_id}'>{$field->toHTML(Field::VIEW)}</option>";
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
        $table = Table::init("linker.lcats");
        $cats = $table
            ->doSelect()
            ->getRows();

        $field = $table->getField("lcat_name");

        $html = "";
        $html .= "<select class='cmsfield' type='text' name='{$this->name}' id='{$this->name}'>";
        foreach($cats as $cat)
        {
            $field->setValue($cat->lcat_name);
            if($this->getValue() == $cat->lcat_id)
                $html .= "<option value='{$cat->lcat_id}' selected='selected'>{$field->toHTML(Field::VIEW)}</option>";
            else
                $html .= "<option value='{$cat->lcat_id}'>{$field->toHTML(Field::VIEW)}</option>";
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
        if($this->getValue() > 0)
        {
            $table = Table::init("linker.lcats");
            $field = $table->getField("lcat_name");
            $cat = $this->parent->getRecord();
            $field->setValue(unserialize($cat[0]->lcat_name));

            return $field->toHTML(Field::VIEW);
        }
        return "";
    }
}