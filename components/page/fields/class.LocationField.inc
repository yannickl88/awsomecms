<?php
/**
 * Field for selecting the location within the site
 *
 * @author Yannick <yannick.l.88@gmail.com>
 */
class LocationField extends Field
{
    protected $displayPages;
    protected $types;
    protected $autoOpen;

    /**
     * Constructor
     *
     * @param string $name
     * @param string $displayName
     * @param int $style
     */
    public function __construct($name, $style=0, $displayPages = false, $types = 1, $autoOpen = false)
    {
        parent::__construct($name, $style|Field::HIDE_VIEW);

        $this->setDefaultValue("/");
        $this->displayPages = $displayPages;
        $this->types = $types;
        $this->autoOpen = $autoOpen;
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_add()
     */
    protected function toHTML_add()
    {
        $smarty = Controller::getInstance()->getSmarty();
        $smarty->loadPlugin("smarty_function_admintree");

        $html = "";
        $html .= smarty_function_admintree(array("javascript" => $this->getName(), "folderonly" => !$this->displayPages, "types" => $this->types, "noCookie" => !$this->autoOpen), $smarty);
        $html .= "<input class='cmsfield' type='text' name='{$this->name}' id='{$this->name}' value='{$this->defaultValue}'/>";
        return $html;
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_edit()
     */
    protected function toHTML_edit()
    {
        $smarty = Controller::getInstance()->getSmarty();
        $smarty->loadPlugin("smarty_function_admintree");

        $html = "";
        $html .= smarty_function_admintree(array("javascript" => $this->getName(), "folderonly" => !$this->displayPages, "types" => $this->types, "noCookie" => !$this->autoOpen), $smarty);
        $html .= "<input class='cmsfield' type='text' name='{$this->name}' id='{$this->name}' value='{$this->value}'/>";

        return $html;
    }
}