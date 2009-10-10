<?php /* Smarty version 2.6.22, created on 2009-09-12 23:04:56
         compiled from file:D:%5Cwww%5Cskimfo%5Ccomponents%5Cpage/forms/form.delete.tpl */ ?>
<form method="post" action="/" class="admin_form admin_page_add">
    <div class="admin_form_row">
        <div class="admin_form_text">
            Are you sure you want to delete this item? <br />
            <ul>
                <li><img src="/img/icons/page.png" alt=""><?php echo $this->_tpl_vars['record']->page_location; ?>
<?php echo $this->_tpl_vars['record']->page_name; ?>
</li>
            </ul>
        </div>
    </div>
    <div class="admin_form_row admin_form_submit">
        <input type="submit" value="Delete" id="page_submit" class="admin_form_submit">
    </div>
    <input type="hidden" name="page_id" value="<?php echo $this->_tpl_vars['record']->page_id; ?>
" />
    <input type="hidden" name="action" value="delete" />
    <input type="hidden" name="component" value="page" />
</form>