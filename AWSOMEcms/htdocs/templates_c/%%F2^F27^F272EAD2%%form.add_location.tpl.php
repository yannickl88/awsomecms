<?php /* Smarty version 2.6.22, created on 2009-09-12 23:15:06
         compiled from file:D:%5Cwww%5Cskimfo%5Ccomponents%5Cskimfo/forms/form.add_location.tpl */ ?>
<?php if ($this->_tpl_vars['FORMERROR']): ?>
    <div class="form_error">
        The form was not filled in correctly. Please check the following fields:
        <ul id="errorlist">
        </ul>
    </div>
    <script>
        <?php $_from = $this->_tpl_vars['FORMERROR']['fields']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }if (count($_from)):
    foreach ($_from as $this->_tpl_vars['field'] => $this->_tpl_vars['error']):
?>
            addFieldError('<?php echo $this->_tpl_vars['field']; ?>
', '<?php echo $this->_tpl_vars['error']; ?>
');
        <?php endforeach; endif; unset($_from); ?>
    </script>
<?php endif; ?>
<form method="post" action="/" class="admin_form admin_skimfo_location_add">
    <div class="admin_form_row">
        <div class="admin_form_label">
            <label for="location_name">Title:</label>
        </div>
        <div class="admin_form_field">
            <input type="text" name="location_name" id="location_name" value="<?php echo $this->_tpl_vars['FORMERROR']['record']['spot_title']; ?>
"/>
        </div>
    </div>
    <div class="admin_form_row admin_form_submit">
        <input type="submit" value="Save" id="skimspot_submit" class="admin_form_submit">
    </div>
    <input type="hidden" name="action" value="add_location" />
    <input type="hidden" name="component" value="skimfo" />
</form>