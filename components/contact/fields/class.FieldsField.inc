<?php
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
        $lang = Language::getInstance();
        $add = ucfirst($lang->getText("add"));

        $html = <<<HTML
            <table id="{$this->getName()}">
                <tr>
                    <th>{$lang->getText("field_name")}</th>
                    <th>{$lang->getText("field_code")}</th>
                    <th>{$lang->getText("field_type")}</th>
                    <th>{$lang->getText("field_required")}</th>
                    <th>{$lang->getText("field_order")}</th>
                    <th></th>
                </tr>
            </table>
            <a href="#" onclick="return addFieldsRow();">{$add}</a>
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
        $lang = Language::getInstance();

        $fields = $this->getRequestValue();
        $add = ucfirst($lang->getText("add"));

        $html = <<<HTML
            <table id="{$this->getName()}">
                <tr>
                    <th>{$lang->getText("field_name")}</th>
                    <th>{$lang->getText("field_code")}</th>
                    <th>{$lang->getText("field_type")}</th>
                    <th>{$lang->getText("field_required")}</th>
                    <th>{$lang->getText("field_order")}</th>
                    <th></th>
                </tr>
HTML;
        $count = 0;
        $count2 = 1;
        foreach($fields as $field)
        {
            $fieldTypeSelected = array("", "", "", "");
            switch($field->field_type)
            {
                case 1:
                    $fieldTypeSelected[0] = 'selected="selected"';
                    break;
                case 2:
                    $fieldTypeSelected[1] = 'selected="selected"';
                    break;
                case 3:
                    $fieldTypeSelected[2] = 'selected="selected"';
                    break;
                case 4:
                    $fieldTypeSelected[3] = 'selected="selected"';
                    break;
            }
            $fieldRequired = "";

            if($field->field_required == 1)
            {
                $fieldRequired = 'checked="checked"';
            }

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
                                <option value='1' {$fieldTypeSelected[0]}>{$lang->getText("field_type_text")}</option>
                                <option value='2' {$fieldTypeSelected[1]}>{$lang->getText("field_type_pass")}</option>
                                <option value='3' {$fieldTypeSelected[2]}>{$lang->getText("field_type_area")}</option>
                                <option value='4' {$fieldTypeSelected[3]}>{$lang->getText("field_type_gend")}</option>
                            </select>
                        </td>
                        <td>
                            <input type="checkbox" class="field_required" value="1" {$fieldRequired}  {/if}/>
                        </td>
                        <td>
                            <input type="text" class="field_sort tinyfield" value="{$count2}"/>
                        </td>
                        <td>
                            <a href="#" onclick="return deleteFieldsRow({$count});"><img src="/img/icons/cross.png" id="deleteIcon"/></a>
                        </td>
                    </tr>
HTML;
            $count++;
            $count2++;
        }
        $html .= <<<HTML
            </table>
            <a href="#" onclick="return addFieldsRow();">{$add}</a>
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
        $lang = Language::getInstance();

        //TODO: fix this so that it can be used multiple times on the same page. Now it is depended on the field name.
        return <<<JS
            $("#{$this->getName()}").ready(function(e) {
                $("#{$this->getName()}").parents("form").submit(function(e) {
                    prossesFields();
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

                return false;
            }
            function addFieldsRow()
            {
                var html = '\
                    <tr class="fieldrow">\
                        <td>\
                            <input type="text" class="field_name smallfield" value=""/>\
                        <\/td>\
                        <td>\
                            <input type="text" class="field_code smallfield" value=""/>\
                        <\/td>\
                        <td>\
                            <select class="field_type smallfield">\
                                <option value="1" >{$lang->getText("field_type_text")}<\/option>\
                                <option value="2" >{$lang->getText("field_type_pass")}<\/option>\
                                <option value="3" >{$lang->getText("field_type_area")}<\/option>\
                                <option value="4" >{$lang->getText("field_type_gend")}<\/option>\
                            <\/select>\
                        <\/td>\
                        <td>\
                            <input type="checkbox" class="field_required" value="1" />\
                        <\/td>\
                        <td>\
                            <input type="text" class="field_sort tinyfield" value="'+$("#{$this->getName()} tr").length+'"/>\
                        <\/td>\
                        <td>\
                            <a href="#" onclick="return deleteFieldsRow('+($("#{$this->getName()} tr.fieldrow").length)+')"><img src="/img/icons/cross.png" id="deleteIcon"/><\/a>\
                        <\/td>\
                    <\/tr>';

                $("#{$this->getName()}").append(html);

                return false;
            }
            function deleteFieldsRow(id)
            {
                $($("#{$this->getName()} tr.fieldrow")[id]).remove();

                resetFieldsRowIds();

                return false;
            }
            function resetFieldsRowIds()
            {
                $("#{$this->getName()} tr.fieldrow").each(function(key, value) {
                    $(value).find("#deleteIcon").removeAttr("onclick");
                    $(value).find("#deleteIcon").click(function(e) { deleteFieldsRow(key); });
                });

                return false;
            }
JS;
    }
}