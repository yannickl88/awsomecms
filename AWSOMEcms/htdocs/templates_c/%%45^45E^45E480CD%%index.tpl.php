<?php /* Smarty version 2.6.22, created on 2009-09-27 14:02:05
         compiled from index.tpl */ ?>
<?php require_once(SMARTY_CORE_DIR . 'core.load_plugins.php');
smarty_core_load_plugins(array('plugins' => array(array('function', 'gallery', 'index.tpl', 2, false),)), $this); ?>
<?php echo $this->_tpl_vars['content']; ?>

<?php echo smarty_function_gallery(array(), $this);?>

<div id="flashbox"></div>
<script type="text/javascript">
<?php echo '
    $(\'#flashbox\').flash({
        src: \'/flash/PhotoGallery.swf\',
        width: 490,
        height: 350,
        wmode: "transparent",
        allowScriptAccess: "always",
        flashvars: {request: "http://data.skimfo.dev"}
    });
    '; ?>

</script>