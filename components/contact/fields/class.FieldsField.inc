<?php
/**
 * This file is part of the A.W.S.O.M.E.cms distribution.
 * Detailed copyright and licensing information can be found
 * in the doc/COPYRIGHT and doc/LICENSE files which should be
 * included in the distribution.
 *
 * @package components.contact.fields
 *
 * @copyright (c) 2009-2010 Yannick de Lange
 * @license http://www.gnu.org/licenses/gpl.txt
 *
 * @version $Revision$
 */
importField("Field");

/**
 * Field for selecting the location within the site
 * 
 * @author Yannick <yannick.l.88@gmail.com>
 */
class FieldsField extends Field
{
    /**
     * Constructor
     * 
     * @param string $name
     * @param string $displayName
     * @param int $style
     */
    public function __construct($name, $displayName = "", $style=0)
    {
        parent::__construct($name, $displayName, $style|Field::HIDE_VIEW);
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_add()
     */
    protected function toHTML_add()
    {
        $html = <<<HTML
            <table id="{$this->getName()}">
                <tr>
                    <th>Name</th>
                    <th>Code</th>
                    <th>Type</th>
                    <th>Required</th>
                    <th>Order</th>
                    <th></th>
                </tr>
            </table>
            <a href="javascript: void(0);" onclick="addFieldsRow();">Add</a>
            <input type="hidden" id="{$this->getName()}_fields" name="{$this->getName()}" value="" />
HTML;
        return $html;
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#toHTML_edit()
     */
    protected function toHTML_edit()
    {
        $fields = $this->getRequestValue();
        
        $html = <<<HTML
            <table id="{$this->getName()}">
                <tr>
                    <th>Name</th>
                    <th>Code</th>
                    <th>Type</th>
                    <th>Required</th>
                    <th>Order</th>
                    <th></th>
                </tr>
HTML;
        $count = 0;
        $count2 = 1;
        foreach($fields as $field)
        {
            $html .= <<<HTML
                    <tr class="fieldrow">
                        <td>
                            <input type="text" class="field_name smallfield" value="{$field->field_name}"/>
                        </td>
                        <td>
                            <input type="text" class="field_code smallfield" value="{$field->field_code}"/>
                        </td>
                        <td>
                            <select class="field_type smallfield">
                                <option value='1' {if $field->field_type == 1} selected="selected" {/if}>Text</option>
                                <option value='2' {if $field->field_type == 2} selected="selected" {/if}>Password</option>
                                <option value='3' {if $field->field_type == 3} selected="selected" {/if}>Textarea</option>
                                <option value='4' {if $field->field_type == 4} selected="selected" {/if}>Gender</option>
                            </select>
                        </td>
                        <td>
                            <input type="checkbox" class="field_required" value="1" {if $field->field_required == 1} checked="checked" {/if}/>
                        </td>
                        <td>
                            <input type="text" class="field_sort tinyfield" value="{$count2}"/>
                        </td>
                        <td>
                            <a href="javascript: void(0)"><img src="/img/icons/cross.png" id="deleteIcon" onclick="deleteFieldsRow({$count})"/></a>
                        </td>
                    </tr>
HTML;
            $count++;
            $count2++;
        }
        $html .= <<<HTML
            </table>
            <a href="javascript: void(0);" onclick="addFieldsRow();">Add</a>
            <input type="hidden" id="{$this->getName()}_fields" name="{$this->getName()}" value="" />
HTML;
        return $html;
    }
    /**
     * (non-PHPdoc)
     * @see core/fields/Field#getJavascript()
     */
    public function getJavascript()
    {
        //TODO: fix this so that it can be used multiple times on the same page. Now it is depended on the field name.
        return <<<JS
            $("#{$this->getName()}").ready(function(e) {
                $("#{$this->getName()}").parents("form").submit(function(e) {
                    return prossesFields();
                });
            });
        
            function prossesFields()
            {
                var string = '';
                
                var fieldsTable = $("#{$this->getName()}");
                
                fieldsTable.find(".fieldrow").each(function(key, value) {
                    if(string != '')
                        string += "&";
                    
                    var required = ($(value).find(".field_required").attr("checked"))? 1 : 0;
                    
                    string += "field_name[]="+ encodeURIComponent($(value).find(".field_name").val());
                    string += "&field_code[]="+ encodeURIComponent($(value).find(".field_code").val());
                    string += "&field_type[]="+ encodeURIComponent($(value).find(".field_type").val());
                    string += "&field_required[]="+ required;
                    string += "&field_sort[]="+ encodeURIComponent($(value).find(".field_sort").val());
                });
                
                $("#{$this->getName()}_fields").val(string);
            }
            function addFieldsRow()
            {
                var html = '\
                    <tr class="fieldrow">\
                        <td>\
                            <input type="text" class="field_name smallfield" value=""/>\
                        </td>\
                        <td>\
                            <input type="text" class="field_code smallfield" value=""/>\
                        </td>\
                        <td>\
                            <select class="field_type smallfield">\
                                <option value="1" >Text</option>\
                                <option value="2" >Password</option>\
                                <option value="3" >Textarea</option>\
                                <option value="4" >Gender</option>\
                            </select>\
                        </td>\
                        <td>\
                            <input type="checkbox" class="field_required" value="1" />\
                        </td>\
                        <td>\
                            <input type="text" class="field_sort tinyfield" value="'+$("#{$this->getName()} tr").length+'"/>\
                        </td>\
                        <td>\
                            <a href="javascript: void(0)"><img src="/img/icons/cross.png" id="deleteIcon" onclick="deleteFieldsRow('+($("#{$this->getName()} tr.fieldrow").length)+')"/></a>\
                        </td>\
                    </tr>';
                
                $("#{$this->getName()}").append(html);
            }
            function deleteFieldsRow(id)
            {
                $($("#{$this->getName()} tr.fieldrow")[id]).remove();
                
                resetFieldsRowIds();
            }
            function resetFieldsRowIds()
            {
                $("#{$this->getName()} tr.fieldrow").each(function(key, value) {
                    $(value).find("#deleteIcon").removeAttr("onclick");
                    $(value).find("#deleteIcon").click(function(e) { deleteFieldsRow(key); });
                });
            }
JS;
    }
}