{if $FORMERROR}
    <div class="form_error">
        The form was not filled in correctly. Please check the following fields:
        <ul id="errorlist">
        </ul>
    </div>
    <script>
        {foreach from=$FORMERROR.fields key=field item=error}
            addFieldError('{$field}', '{$error}');
        {/foreach}
    </script>
{/if}
<form method="post" action="/" class="admin_form admin_contact_add" onsubmit="return prossesFields();">
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="form_name">Name:</label>
        </div>
        <div class="admin_form_field">
            <input type="text" name="form_name" id="form_name" value="{$FORMERROR.record.form_name}"/>
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="form_to">To:</label>
        </div>
        <div class="admin_form_field">
            <input type="text" name="form_to" id="form_to" value="{$FORMERROR.record.form_to}"/>
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="form_bcc">Bcc:</label>
        </div>
        <div class="admin_form_field">
            <input type="text" name="form_bcc" id="form_bcc" value="{$FORMERROR.record.form_bcc}"/>
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="form_subject">Subject:</label>
        </div>
        <div class="admin_form_field">
            <input type="text" name="form_subject" id="form_subject" value="{$FORMERROR.record.form_subject}"/>
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="form_thanks">Thanks Page:</label>
        </div>
        <div class="admin_form_field">
            {admintree javascript="form_thanks"}
            <input type="text" name="form_thanks" id="form_thanks" value="{$FORMERROR.record.form_thanks}"/>
        </div>
    </div>
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="news_title">Fields:</label>
        </div>
        <div class="admin_form_field">
            <table id="form_fields">
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
        </div>
    </div>
    <div class="admin_form_row admin_form_submit">
        <input type="submit" value="Save" id="news_submit" class="admin_form_submit">
    </div>
    <input type="hidden" id="fields" name="fields" value="" />
    <input type="hidden" name="action" value="add" />
    <input type="hidden" name="component" value="contact" />
</form>
<script>
    {literal}
    function prossesFields()
    {
        var string = '';
        
        var fieldsTable = $("#form_fields");
        
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
        
        $("#fields").val(string);
        
        return true;
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
                    <input type="text" class="field_sort tinyfield" value="'+$("#form_fields tr").length+'"/>\
                </td>\
                <td>\
                    <a href="javascript: void(0)"><img src="/img/icons/cross.png" id="deleteIcon" onclick="deleteFieldsRow('+($("#form_fields tr.fieldrow").length)+')"/></a>\
                </td>\
            </tr>';
        
        $("#form_fields").append(html);
    }
    function deleteFieldsRow(id)
    {
        $($("#form_fields tr.fieldrow")[id]).remove();
        
        resetFieldsRowIds();
    }
    function resetFieldsRowIds()
    {
        $("#form_fields tr.fieldrow").each(function(key, value) {
            $(value).find("#deleteIcon").removeAttr("onclick");
            $(value).find("#deleteIcon").click(function(e) { deleteFieldsRow(key); });
        });
    }
    {/literal}
</script>