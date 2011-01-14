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
 * Textarea field
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class TextField extends Field
{
    /**
     * Constructor
     * 
     * @param string $name
     * @param string $displayName
     * @param int $style
     */
    public function __construct($name, $style=0)
    {
        parent::__construct($name, $style|Field::HIDE_VIEW);
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_add()
     */
    protected function toHTML_add()
    {
        return "<textarea name='{$this->getName()}' id='{$this->getName()}' class=\"htmleditor\" rows=\"20\" cols=\"20\">".htmlentities(utf8_decode($this->getDefaultValue()), ENT_QUOTES)."</textarea>";
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_edit()
     */
    protected function toHTML_edit()
    {
        return "<textarea name='{$this->getName()}' id='{$this->getName()}' class=\"htmleditor\" rows=\"20\" cols=\"20\">".htmlentities(utf8_decode($this->getValue()), ENT_QUOTES)."</textarea>";
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#getJavascript()
     */
    public function getJavascript()
    {
        if(Config::get("type", "wysiwig", "input") == "bbcode")
        {
            return <<<JS
            //load the editor
            $().ready(function(e) {
                $(".htmleditor").each(function(key, element) {
                    new Editor($(element), {
                        width: 626,
                        height: 175
                    });
                });
            });
JS;
        }
        else
        {
            return <<<JS
            function bindEditor(element)
            {
                element.get(0).editor = new TINY.editor.edit('editor_'+element.attr("name"),{
                    id:element.attr("name"),
                    width:626,
                    height:175,
                    cssclass:'te',
                    controlclass:'tecontrol',
                    rowclass:'teheader',
                    dividerclass:'tedivider',
                    controls:['bold','italic','underline','strikethrough','|','style','|',
                              'orderedlist','unorderedlist','|','outdent','indent','|','leftalign',
                              'centeralign','rightalign','blockjustify','|','unformat','|','undo','redo','n',
                              'image','hr','link','unlink','|','cut','copy','paste','print'],
                    footer:true,
                    fonts:['Verdana','Arial','Georgia','Trebuchet MS'],
                    xhtml:true,
                    cssfile:'/css/style.css',
                    bodyid:'editor',
                    footerclass:'tefooter',
                    toggle:{text:'source',activetext:'wysiwyg',cssclass:'toggle'},
                    resize:{cssclass:'resize'}
                });
                
                element.parents("form").submit(function(e) {
                    $(e.currentTarget).find(".htmleditor").each(function(key, value) {
                        value.editor.post()
                    });
                });
            }
            $().ready(function(e) {
                $(".htmleditor").each(function(key, element) {
                     bindEditor($(element));
                });
            });
JS;
        }
    }
}