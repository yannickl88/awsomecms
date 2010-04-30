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
importField("Field");

/**
 * Tag field, this allows for quick search and assignment of tags
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class TagField extends Field
{
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#insert()
     */
    public function insert($query, $data = array())
    {
        Cache::clear($this->getDBName()."_tags");
        parent::insert($query, $data);
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#update()
     */
    public function update($query, $data = array())
    {
        Cache::clear($this->getDBName()."_tags");
        parent::update($query, $data);
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#delete()
     */
    public function delete($query, $data = array())
    {
        Cache::clear($this->getDBName()."_tags");
        parent::delete($query, $data);
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#getJavascript()
     */
    public function getJavascript()
    {
        $tags = json_encode($this->getAllTags());
        
        return <<<JS
            var tags = {$tags};
            var dropdown = null;
            
            function tagField_handelKeys(e)
            {
                if(dropdown != null && (e.keyCode == 40 || e.keyCode == 38))
                {
                    var select = dropdown[0];
                    var index = -1;
                    
                    for(var i in select.options)
                    {
                        if(select.options[i].selected == true)
                        {
                            index = parseInt(i);
                            break;
                        }
                    }
                    
                    if(index == -1 && select.options.length > 0)
                    {
                        select.options[0].selected = true;
                    }
                    else if((e.keyCode == 40 && index < select.options.length - 1) || (e.keyCode == 38 && index > 0))
                    {
                        select.options[index].selected = false;
                        select.options[index + (e.keyCode - 39)].selected = true;
                    }
                    e.preventDefault();
                    e.stopPropagation();
                }
                if(dropdown != null && e.keyCode == 13)
                {
                    e.preventDefault();
                    e.stopPropagation();
                    tagField_removeTags(e);
                }
            }
            function tagField_showTags(e)
            {
                if(e.keyCode == 13) //just make sure that the field doesn't trigger on the return key.
                {
                    return false;
                }
                
                var value = $(e.currentTarget).val();
                
                if(dropdown == null)
                {
                    dropdown = $("<select size='5' multiple='multiple'></select>");
                    dropdown.css({position: 'absolute', 'z-index': 1000, top: Math.round($(e.currentTarget).outerHeight() + $(e.currentTarget).position().top), left: Math.round($(e.currentTarget).position().left), width: $(e.currentTarget).outerWidth()});
                    $("#{$this->getDBName()}").after(dropdown);
                }
                
                var index = false;
                
                if(dropdown[0].options.length > 0)
                {
                    for(var i in dropdown[0].options)
                    {
                        if(dropdown[0].options[i].selected == true)
                        {
                            index = dropdown[0].options[i].value;
                            break;
                        }
                    }
                }
                
                dropdown.empty();
                var showtags = 0;
                
                if(tags.lenght > 0)
                {
                    for(var key in tags)
                    {
                        var tag = tags[key];
                        if(tag.indexOf(value) >= 0)
                        {
                            var option = $("<option>"+tag+"</option>");
                            option.mouseover(function(e){ this.selected = true});
                            option.mouseout(function(e){ this.selected = false});
                            dropdown.append(option);
                            
                            if(index == tag)
                            {
                                option[0].selected = true;
                            }
                            
                            showtags++;
                        }
                    }
                }
                
                if(showtags == 0)
                {
                    tagField_removeTags(null);
                }
            }
            
            function tagField_removeTags(e)
            {
                if(dropdown != null)
                {
                    if(dropdown.val() != null)
                    {
                        $("#{$this->getDBName()}").val(dropdown.val());
                    }
                    
                    dropdown.remove();
                    dropdown = null;
                }
            }
            
            $().ready(function(e) {
                $("#{$this->getDBName()}").keydown(tagField_handelKeys);
                $("#{$this->getDBName()}").keyup(tagField_showTags);
                $("#{$this->getDBName()}").attr("autocomplete", "off");
                $("#{$this->getDBName()}").parent().focusout(tagField_removeTags);
            });
JS;
    }
    
    private function getAllTags()
    {
        if(Cache::has($this->getDBName()."_tags"))
        {
            return Cache::get($this->getDBName()."_tags");
        }
        else
        {
            $result = $this->parent->doSelect()->getRows();
            $fieldName = $this->getDBName();
            $tags = array();
            
            foreach($result as $row)
            {
                if(!in_array($row->$fieldName, $tags))
                {
                    $tags[] = $row->$fieldName;
                }
            }
            
            sort($tags);
            
            Cache::set($this->getDBName()."_tags", $tags);
            
            return $tags;
        }
    }
}