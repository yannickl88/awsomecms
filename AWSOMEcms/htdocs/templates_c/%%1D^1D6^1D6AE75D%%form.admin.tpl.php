<?php /* Smarty version 2.6.22, created on 2009-09-12 11:53:51
         compiled from file:D:%5Cwww%5Cskimfo%5Ccomponents%5Cnews/forms/form.admin.tpl */ ?>
<table class="admin_table">
    <tr class="admin_table_row">
        <th class="admin_table_header">
            Title
        </th>
        <th class="admin_table_header">
            User
        </th>
        <th class="admin_table_header">
            Date
        </th>
        <th class="admin_table_header">
        </th>
        <th class="admin_table_header">
        </th>
    </tr>
    <?php $_from = $this->_tpl_vars['records']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }$this->_foreach['admin'] = array('total' => count($_from), 'iteration' => 0);
if ($this->_foreach['admin']['total'] > 0):
    foreach ($_from as $this->_tpl_vars['row']):
        $this->_foreach['admin']['iteration']++;
?>
    <tr class="admin_table_row <?php if (($this->_foreach['admin']['iteration']-1)%2 == 1): ?>highlighted<?php endif; ?>">
        <td class="admin_table_cell">
            <?php echo $this->_tpl_vars['row']->news_title; ?>

        </td>
        <td class="admin_table_cell">
            <?php echo $this->_tpl_vars['row']->user_name; ?>

        </td>
        <td class="admin_table_cell">
            <?php echo $this->_tpl_vars['row']->news_date; ?>

        </td>
        <td class="admin_table_cell admin_table_actions">
            <a href="/<?php echo $this->_tpl_vars['actionurl_edit']; ?>
&news_id=<?php echo $this->_tpl_vars['row']->news_id; ?>
"><img src="/img/icons/page_white_edit.png" alt="edit"></a>
            <a href="/<?php echo $this->_tpl_vars['actionurl_delete']; ?>
&news_id=<?php echo $this->_tpl_vars['row']->news_id; ?>
"><img src="/img/icons/page_white_delete.png" alt="delete"></a>
        </td>
        <td class="admin_table_cell admin_table_actions">
            <input type="checkbox" value="<?php echo $this->_tpl_vars['row']->news_id; ?>
" name="action[]" class="actioncheckbox">
        </td>
    </tr>
    <?php endforeach; else: ?>
    <tr class="admin_table_row" colspan="5">
        <td class="admin_table_cell">
            No Records
        </td>
    </tr>
    <?php endif; unset($_from); ?>
    <tr class="admin_table_row">
        <td class="admin_table_cell admin_table_actions" colspan="4">
            <a href="/<?php echo $this->_tpl_vars['actionurl_add']; ?>
">Add</a>
        </td>
        <td class="admin_table_cell">
            <a href="javascript: void(0);" onclick="deleteMultiple('/<?php echo $this->_tpl_vars['actionurl_delete']; ?>
', 'news_id', $('.actioncheckbox'));"><img src="/img/icons/page_white_delete.png" alt="deleteselected"></a>
        </td>
    </tr>
</table>