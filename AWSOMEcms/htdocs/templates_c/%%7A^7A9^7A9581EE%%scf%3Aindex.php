<?php /* Smarty version 2.6.22, created on 2009-09-20 11:47:46
         compiled from scf:index */ ?>
<?php require_once(SMARTY_CORE_DIR . 'core.load_plugins.php');
smarty_core_load_plugins(array('plugins' => array(array('function', 'news', 'scf:index', 3, false),)), $this); ?>
<h1>Welcome</h1><br />
<p>Installation was a success</p><br />
<p><?php echo smarty_function_news(array('max' => 5), $this);?>
</p>