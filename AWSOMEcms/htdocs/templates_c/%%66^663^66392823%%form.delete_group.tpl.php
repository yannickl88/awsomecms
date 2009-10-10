<?php /* Smarty version 2.6.22, created on 2009-09-20 00:21:29
         compiled from file:D:%5Cwww%5Cskimfo%5Ccomponents%5Cslideshow/forms/form.delete_group.tpl */ ?>
<form method="post" action="/" class="admin_form admin_group_delete">
    <div class="admin_form_row">
        <div class="admin_form_text">
            Are you sure you want to delete this group? <br />
            <ul>
                <?php $_from = $this->_tpl_vars['record']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }if (count($_from)):
    foreach ($_from as $this->_tpl_vars['row']):
?>
                    <li><img src="/img/icons/image.png" alt=""><?php echo $this->_tpl_vars['row']->group_title; ?>
</li>
                    <input type="hidden" name="group_id[]" value="<?php echo $this->_tpl_vars['row']->group_id; ?>
" />
                <?php endforeach; endif; unset($_from); ?>
            </ul>
        </div>
    </div>
    <div class="admin_form_row admin_form_submit">
        <input type="submit" value="Delete" id="page_submit" class="admin_form_submit">
    </div>
    <input type="hidden" name="action" value="delete_group" />
    <input type="hidden" name="component" value="slideshow" />
</form>