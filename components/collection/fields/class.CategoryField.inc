<?php
/**
 * Field for uploading files
 *
 * @author Yannick <yannick.l.88@gmail.com>
 */
class CategoryField extends Field
{
    protected $cats;

    /**
     * Constructor
     *
     * @param string $name
     * @param int $style
     */
    public function __construct($name, $style=0)
    {
        parent::__construct($name, $style|Field::HIDE_VIEW);
    }
    /**
     * (non-PHPdoc)
     * @see Field::select()
     *
     * @param SQLquery $query
     * @param array $data
     */
    public function select($query, $data = array())
    {
        if($this->hasFlag(Field::DUMMY) && !$this->hasFlag(Field::FORCESELECT))
        {
            return;
        }

        $query->select($this->getDBName()."1");
        $query->select($this->getDBName()."2");
        $query->select($this->getDBName()."3");
        $query->select($this->getDBName()."4");
    }

    /**
     * (non-PHPdoc)
     * @see core/fields/Field::insert()
     */
    public function insert($query, $data)
    {
        $parts = $this->getValue();

        $query->insert($this->getDBName()."1", $parts[0]);
        $query->insert($this->getDBName()."2", $parts[1]);
        $query->insert($this->getDBName()."3", $parts[2]);
        $query->insert($this->getDBName()."4", $parts[3]);
        $query->insert($this->getDBName()."_url", Component::init("collection")->makeURL((object) array(
                "model_cat1" => $parts[0],
                "model_cat2" => $parts[1],
                "model_cat3" => $parts[2],
                "model_cat4" => $parts[3],
                )));
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field::update()
     */
    public function update($query, $data)
    {
        $parts = $this->getValue();

        $query->update($this->getDBName()."1", $parts[0]);
        $query->update($this->getDBName()."2", $parts[1]);
        $query->update($this->getDBName()."3", $parts[2]);
        $query->update($this->getDBName()."4", $parts[3]);

        $query->update($this->getDBName()."_url", Component::init("collection")->makeURL((object) array(
                "model_cat1" => $parts[0],
                "model_cat2" => $parts[1],
                "model_cat3" => $parts[2],
                "model_cat4" => $parts[3],
                )));
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_add()
     */
    protected function toHTML_add()
    {
        $html = "<div id='{$this->getName()}'></div>";
        $html .= "<script type=\"text/javascript\">";
        $html .= $this->makeJavascript();
        $html .= "cField.update(1);\n";
        $html .= "</script>";

        return $html;
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_edit()
     */
    protected function toHTML_edit()
    {
        $html = "<div id='{$this->getName()}'></div>";
        $html .= "<script type=\"text/javascript\">";
        $html .= $this->makeJavascript();
        $html .= "cField.setValue('".implode(";", $this->getValue())."');\n";
        $html .= "cField.update(1);\n";
        $html .= "</script>";

        return $html;
    }

    public function setRequestData($record)
    {
        $name1 = $this->getName()."1";
        $name2 = $this->getName()."2";
        $name3 = $this->getName()."3";
        $name4 = $this->getName()."4";

        if(isset($record->$name1) && isset($record->$name2) && isset($record->$name3) && isset($record->$name4))
        {
            $this->setRequestValue(array($record->$name1, $record->$name2, $record->$name3, $record->$name4));
        }

        return $this;
    }

    public function setData($record)
    {
        $name1 = $this->getName()."1";
        $name2 = $this->getName()."2";
        $name3 = $this->getName()."3";
        $name4 = $this->getName()."4";

        if(isset($record->$name1) && isset($record->$name2) && isset($record->$name3) && isset($record->$name4))
        {
            $this->setValue(array($record->$name1, $record->$name2, $record->$name3, $record->$name4));
        }

        // check if a single name was set
        if(isset($record->{$this->getName()}))
        {
            $this->setValue(explode(";", $record->{$this->getName()}));
        }


        return $this;
    }

    public function makeJavascript()
    {
        $js = "var cField = new cField($('#{$this->getName()}'));\n";
        $cats = Table::init("collection.mcats")
            ->doSelect();

        while($cat = $cats->getRow())
        {
            $lang = Config::get("lang", "en", "lang");
            $js .= "cField.add(\"". htmlentities($cat->mcat_name[$lang]) . "\", {$cat->mcat_id}, {$cat->mcat_type}, false);\n";
        }

        return $js;
    }

    /**
     * (non-PHPdoc)
     * @see core/fields/Field::validate()
     *
     * @param int $mode
     */
    public function validate($mode)
    {
        parent::validate($mode);

        if($mode == Field::EDIT || $mode == Field::ADD)
        {
            $val = $this->getValue();

            if(
                !$this->hasValue($val[0]) ||
                !$this->hasValue($val[1]) ||
                !$this->hasValue($val[2]) ||
                !$this->hasValue($val[3])
            )
            {
                throw new FormException($this, Language::get("error_notfilledin"));
            }

            $record = Table::init("collection.models")
                ->setRequest((object) array(
                        "model_cat1" => $val[0],
                        "model_cat2" => $val[1],
                        "model_cat3" => $val[2],
                        "model_cat4" => $val[3]
                    ))
                ->doSelect();

            if($record->count() > 0)
            {
                $data = $this->parent->getRecord();

                //check if it is not the same record when editing
                if($mode == Field::ADD || ($mode == Field::EDIT && $data[0]->model_id != $record->getRow()->model_id))
                    throw new FormException($this, Language::get("error_samemodelcat"));
            }
        }
    }
}