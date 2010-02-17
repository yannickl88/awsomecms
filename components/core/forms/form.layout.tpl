<h2>{"layoutmapping"|text}</h2>
<form method="post" action="/" class="admin_form admin_core_layout">
    <table>
        <tr class="header">
            <th>{"url"|text}</th>
            <th>{"layout"|text}</th>
        </tr>
        {foreach from=$maping item=map}
            <tr class="row">
                <td><input type="text" value="{$map[0]}"/></td>
                <td><input type="text" value="{$map[1]}"/></td>
            </tr>
        {/foreach}
    </table>
    <div class="admin_form_row admin_form_submit">
        <input type="submit" value="{"save"|text|ucfirst}" id="user_submit" class="admin_form_submit" />
    </div>
    <input type="hidden" name="action" value="layout" />
    <input type="hidden" name="component" value="core" />
</form>
<script type="text/javascript">
    {literal}
    function isEmpty(row)
    {
        var empty = true;
        
        //is there no elements?
        if(row.length == 0)
        {
            return false;
        }
        
        row.find('input').each(function(key, value) {
            if($(value).val() != "")
            {
                empty = false;
            }
        });
        
        return empty;
    }
    
    function checkField(e) {
        //remove the row if it was empty
        var row = $(this).parent().parent();
        if(isEmpty(row) == true)
        {
            row.remove();
        }
        
        //now check if the last row is empty
        insertEmptyRow();
    }
    
    function insertEmptyRow() {
        var row = $(".admin_core_layout .row:last");
        
        if(isEmpty(row) == false)
        {
            var newrow = $("<tr class=\"row\"><td><input type=\"text\" /><\/td><td><input type=\"text\" /><\/td><\/tr>");
            newrow.find("input[type=text]").blur(checkField);
            $(".admin_core_layout table").append(newrow);
        }
    }
    
    $('.admin_core_layout input[type=text]').blur(checkField);
    $('.admin_core_layout').submit(function(e) {
        $(".admin_core_layout .row").each(function(key, value) {
            if(!isEmpty($(value)))
            {
                var map = $(value).find('input:first').val() + " " + $(value).find('input:last').val();
                $(".admin_core_layout").append("<input type=\"hidden\" name=\"map[]\" value=\"" + encodeURI(map) + "\"/>");
            }
        });
    });
    insertEmptyRow();
    {/literal}
</script>