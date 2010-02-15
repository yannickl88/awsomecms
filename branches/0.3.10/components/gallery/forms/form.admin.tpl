<table class="admin_table">
    <tr class="admin_table_row">
        <th class="admin_table_header">
            Title
        </th>
        <th class="admin_table_header">
            Thumb
        </th>
        <th class="admin_table_header">
        </th>
        <th class="admin_table_header">
        </th>
    </tr>
    {foreach from=$records item=row name="admin"}
    <tr class="admin_table_row {if $smarty.foreach.admin.index%2 == 1}highlighted{/if}">
        <td class="admin_table_cell">
            {$row->image_title}
        </td>
        <td class="admin_table_cell">
            <img src="{$row->image_urlthumb}" alt="" />
        </td>
        <td class="admin_table_cell admin_table_actions">
            <a href="/{$actionurl_edit}&image_id={$row->image_id}"><img src="/img/icons/image_edit.png" alt="edit"></a>
            <a href="/{$actionurl_delete}&image_id={$row->image_id}"><img src="/img/icons/image_delete.png" alt="delete"></a>
        </td>
        <td class="admin_table_cell admin_table_actions">
            <input type="checkbox" value="{$row->image_id}" name="action[]" class="actioncheckbox">
        </td>
    </tr>
    {foreachelse}
    <tr class="admin_table_row" colspan="4">
        <td class="admin_table_cell">
            No Records
        </td>
    </tr>
    {/foreach}
    <tr class="admin_table_row">
        <td class="admin_table_cell admin_table_actions" colspan="3">
            <a href="/{$actionurl_add}">Add</a>
        </td>
        <td class="admin_table_cell">
            <a href="javascript: void(0);" onclick="deleteMultiple('/{$actionurl_delete}', 'image_id', $('.actioncheckbox'));"><img src="/img/icons/page_white_delete.png" alt="deleteselected"></a>
        </td>
    </tr>
</table>