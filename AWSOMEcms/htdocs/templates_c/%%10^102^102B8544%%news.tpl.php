<?php /* Smarty version 2.6.22, created on 2009-09-12 17:55:37
         compiled from news/news.tpl */ ?>
<?php require_once(SMARTY_CORE_DIR . 'core.load_plugins.php');
smarty_core_load_plugins(array('plugins' => array(array('modifier', 'truncate', 'news/news.tpl', 3, false),)), $this); ?>
<?php $_from = $this->_tpl_vars['news']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }if (count($_from)):
    foreach ($_from as $this->_tpl_vars['item']):
?>
    <h2><a href="/news?id=<?php echo $this->_tpl_vars['item']->news_id; ?>
"><?php echo $this->_tpl_vars['item']->news_title; ?>
</a></h2>
    <?php echo ((is_array($_tmp=$this->_tpl_vars['item']->news_text)) ? $this->_run_mod_handler('truncate', true, $_tmp, 50) : smarty_modifier_truncate($_tmp, 50)); ?>

    <div style="float: right;">
        <a href="/news?id=<?php echo $this->_tpl_vars['item']->news_id; ?>
">Lees meer >>></a>
    </div>
    <hr />
<?php endforeach; endif; unset($_from); ?>