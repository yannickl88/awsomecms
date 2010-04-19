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
        return "<textarea name='{$this->getName()}' id='{$this->getName()}' class=\"htmleditor\" rows=\"20\" cols=\"20\">".htmlentities($this->getDefaultValue(), ENT_QUOTES)."</textarea>";
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_edit()
     */
    protected function toHTML_edit()
    {
        return "<textarea name='{$this->getName()}' id='{$this->getName()}' class=\"htmleditor\" rows=\"20\" cols=\"20\">".htmlentities($this->getValue(), ENT_QUOTES)."</textarea>";
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#getJavascript()
     */
    public function getJavascript()
    {
        return <<<JS
            //load the editor
            $("#{$this->getName()}").ready(function() {
                var data = {
                    buttonList: [
                        'bold',
                        'italic',
                        'underline',
                        'left',
                        'center',
                        'right',
                        'justify',
                        'ol',
                        'ul',
                        'subscript',
                        'superscript',
                        'strikethrough',
                        'removeformat',
                        'hr',
                        'image',
                        'link',
                        'unlink',
                        'upload',
                        'fontFormat',
                        'xhtml'
                    ],
                    xhtml: true
                };
                
                var element = $("#{$this->getName()}")[0];
                
                if(element)
                {
                    new nicEditor(data).panelInstance(element);
                }
            });
JS;
    }
}