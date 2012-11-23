<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package core.fields
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */
importField("TextField");

/**
 * Text field which support multilanguagal input
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class MLangField extends Field
{
    /**
     * Constructor
     * 
     * (non-PHPdoc)
     * @see core/fields/Field::Field()
     */
    public function MLangField($name, $style = 0)
    {
        parent::__construct($name, $style|Field::NOWHERE);
    }
    
    /**
     * (non-PHPdoc)
     * @see core/fields/Field::toHTML_add()
     */
    protected function toHTML_add()
    {
        $htmlInput = "";
        $htmlTabs = "";
        
        $langs = Config::get("languages", array("en"), "page");
        $value = $this->getValue();
        
        foreach($langs as $lang)
        {
            $style = "";
            $active = "";
            if($htmlInput != "") $style = "style='display:none;'";
            if($htmlInput == "") $active = " active";
            
            $htmlTabs .= "<a href='#' class=\"{$lang}{$active}\"><img src='/img/admin/{$lang}.png' /></a>";
            
            $htmlInput .= "<div class='inputText_wrapper {$lang}' {$style}>";
            $htmlInput .= "<input type='text' class='cmsfield' name='{$this->getName()}_{$lang}' id='{$this->getName()}_{$lang}' value='".htmlentities($this->getDefaultValue(), ENT_QUOTES, "UTF-8")."'/>";
            $htmlInput .= "</div>";
        }
        
        return "<div id='{$this->getName()}_wrapper'><div class=\"admin_langTabs\">".$htmlTabs."</div>".$htmlInput."</div>";
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field::toHTML_edit()
     */
    protected function toHTML_edit()
    {
        $htmlInput = "";
        $htmlTabs = "";
        
        $langs = Config::get("languages", array("en"), "page");
        $value = $this->getValue();
        
        foreach($langs as $lang)
        {
            $style = "";
            $active = "";
            if($htmlInput != "") $style = "style='display:none;'";
            if($htmlInput == "") $active = " active";
            
            $htmlTabs .= "<a href='#' class=\"{$lang}{$active}\"><img src='/img/admin/{$lang}.png' /></a>";
            
            $htmlInput .= "<div class='inputText_wrapper {$lang}' {$style}>";
            $htmlInput .= "<input type='text' class='cmsfield' name='{$this->getName()}_{$lang}' id='{$this->getName()}_{$lang}' value='".htmlentities($value[$lang], ENT_QUOTES, "UTF-8")."'/>";
            $htmlInput .= "</div>";
        }
        
        return "<div id='{$this->getName()}_wrapper'><div class=\"admin_langTabs\">".$htmlTabs."</div>".$htmlInput."</div>";
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field::toHTML_view()
     */
    protected function toHTML_view()
    {
        $value = $this->getValue();
        return $value[Config::get("lang", "en", "lang")];
    }
    /**
     * (non-PHPdoc)
     * @see Field::toHTML_disabled()
     *
     * @return string
     */
    protected function toHTML_disabled()
    {
        $htmlInput = "";
        $htmlTabs = "";
    
        $langs = Config::get("languages", array("en"), "page");
        $value = $this->getValue();
    
        foreach($langs as $lang)
        {
            $style = "";
            $active = "";
            if($htmlInput != "") $style = "style='display:none;'";
            if($htmlInput == "") $active = " active";
    
            $htmlTabs .= "<a href='#' class=\"{$lang}{$active}\"><img src='/img/admin/{$lang}.png' /></a>";
    
            $htmlInput .= "<div class='inputText_wrapper {$lang}' {$style}>";
            $htmlInput .= "<div>".htmlentities($value[$lang], ENT_QUOTES, "UTF-8")."</div>";
            $htmlInput .= "</div>";
        }
    
        return "<div id='{$this->getName()}_wrapper'><div class=\"admin_langTabs\">".$htmlTabs."</div>".$htmlInput."</div>";
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/TextField::getJavascript()
     */
    public function getJavascript()
    {
        $js = parent::getJavascript();
        $js .= <<<JS

            function toggleLangTab(e) {
                $(e.currentTarget).parent().children("a").removeClass("active");
                var lang = $(e.currentTarget).attr("class");
                $(e.currentTarget).addClass("active");
                
                $(e.currentTarget).parent().parent().children(".inputText_wrapper").css("display", "none");
                $(e.currentTarget).parent().parent().children(".inputText_wrapper."+lang).css("display", "block");
                return false;
            }
            
            $().ready(function(e) {
                $(".admin_langTabs a").each(function(key, element) {
                    $(element).click(toggleLangTab);
                    $(element).parents(".admin_form_row").data("toggleSection", function (lang) {
                        var e = {};
                        e.currentTarget = $(element).parent().children("a."+lang);
                        
                        return toggleLangTab(e);
                    });
                });
            });
JS;
        return $js;
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field::db2value()
     */
    public function db2value($value)
    {
        if(!empty($value))
        {
            return unserialize($value);
        }
        
        return $value;
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field::value2db()
     */
    public function value2db($value)
    {
        if(!empty($value))
        {
            return serialize($value);
        }
        
        return $value;
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field::preSQL()
     */
    public function preSQL(&$record, $query)
    {
        if($query->getAction() == "update" || $query->getAction() == "insert")
        {
            $langs = Config::get("languages", array("en"), "page");
            $data = array();
            $r = $this->parent->getRecord();
            
            foreach($langs as $lang)
            {
                $data[$lang] = $r[0]->{$this->getName()."_".$lang};
            }
            
            $record[$this->getName()] = $data;
            $this->setValue($data);
        }
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field::postSQL()
     */
    public function postSQL(&$record, $query)
    {
        if($query->getAction() == "select")
        {
            $where = $query->getWhereOnField($this->getDBName());
            $conditionStr = '';
            
            foreach ($where as $w)
            {
                if(!empty($conditionStr))
                {
                    if($w[1] == "AND") $conditionStr .= " && ";
                    else $conditionStr .= " || ";
                }
                
                $conditionStr .= "\$row->{$this->getName()}['".getLang()."']";
                if($w[4] == "=") $conditionStr .= " == ";
                else $conditionStr .= " {$w[4]} ";
                
                $conditionStr .= $w[3];
            }
            
            if(!empty($conditionStr))
                $record->filter(create_function('$row', "return ({$conditionStr});"));
        }
    }
    /**
     * Validate the data for this field. This throws a FormException when not valid
     * 
     * @param int $mode
     */
    public function validate($mode)
    {
        $langs = Config::get("languages", array("en"), "page");
        $values = $this->parent->getRecord();
        
        // validate for each language
        foreach($langs as $lang)
        {
            if(isset($values[0]->{$this->getName()."_".$lang}))
                $value = $values[0]->{$this->getName()."_".$lang};
            else
                $value = "";
            
            if($mode == Field::ADD &&
                $this->isRequired($mode) && 
                !$this->hasValue($value) &&
                $this->hasFlag(Field::AUTOKEY) == false &&
                !$this->hasFlag(Field::DUMMY)
            )
            {
                throw new FormException($this, Language::get("error_notfilledin"), 0, $lang);
            }
            if($mode == Field::EDIT &&
                $this->isRequired($mode) && 
                !$this->hasValue($value) &&
                !$this->hasFlag(Field::DUMMY)
            )
            {
                throw new FormException($this, Language::get("error_notfilledin"), 0, $lang);
            }
        }
    }
}