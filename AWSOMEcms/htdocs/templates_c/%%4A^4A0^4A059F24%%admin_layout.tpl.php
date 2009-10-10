<?php /* Smarty version 2.6.22, created on 2009-09-20 22:57:51
         compiled from admin_layout.tpl */ ?>
<?php require_once(SMARTY_CORE_DIR . 'core.load_plugins.php');
smarty_core_load_plugins(array('plugins' => array(array('function', 'admintree', 'admin_layout.tpl', 61, false),array('function', 'rendertime', 'admin_layout.tpl', 63, false),)), $this); ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" 
   "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
    <head>
        <title>Admin</title>
        <meta http-equiv="Content-type" content="text/html;charset=UTF-8" />
        <meta name="description" content="SlyFox Design - We provide professional web solutions to all clients, that are cost effective in all aspects. With full control, our clients can maintain their own websites with ease and without having to know complicated computer launguages!"/>
        <meta name="keywords" content="web, design, website, uk, cheap, professional, effective" />
        <meta name="country" content="United Kingdom" />
        <meta name="geo.country" content="GB" />
        <link type="text/css" rel="stylesheet" href="/css/css.css" media="screen" />
        <link type="text/css" rel="stylesheet" href="/css/admin.css" media="screen" />
        <link type="text/css" rel="stylesheet" href="/css/lightbox.css" media="screen" />
        <link type="text/css" rel="stylesheet" href="/css/wysiwyg.css" media="screen" />
        <script type="text/javascript" src="http://www.google.com/jsapi"></script>
        <script type="text/javascript">
            <?php echo '
            // <![CDATA[ 
            // Load jQuery
            google.load("jquery", "1.3.1", {uncompressed:false});
            google.load("jqueryui", "1.5.3");
            // ]]> 
            '; ?>

        </script>
        <script type="text/javascript" src="/js/site.js"></script>
        <script type="text/javascript" src="/js/tree.js"></script>
        <script type="text/javascript" src="/js/jquery.lightbox.js"></script>
        <script type="text/javascript" src="/js/ajaxupload.3.5.js"></script>
        <script type="text/javascript" src="/js/jquery.wysiwyg.js"></script>
        <script type="text/javascript">
            <?php echo '
            $(document).ready(function(){
                // load lightbox
                $(".lightbox").lightbox({
                    fitToScreen: true,
                    imageClickClose: false,
                    disableNavbarLinks: true
                });
                $(".htmleditor").wysiwyg({
                    
                });
            });
            '; ?>

        </script>
    </head>
    <body>
        <ul>
            <li><a href="/admin/page/add">Add Page</a></li>
            <li><a href="/admin/users/admin">Users</a></li>
            <li><a href="/admin/news/admin">News</a></li>
            <li><a href="/admin/gallery/admin">Gallery</a></li>
            <li><a href="/admin/contact/admin">Contact</a></li>
            <li><a href="/admin/skimfo/admin">Skimfo</a></li>
            <li>Slideshow</li>
            <ul>
                <li><a href="/admin/slideshow/group/admin">Groups</a></li>
                <li><a href="/admin/slideshow/image/admin">Images</a></li>
            </ul>
            <li><a href="/">Back to Site</a></li>
        </ul>
        <?php echo smarty_function_admintree(array(), $this);?>

        <?php echo $this->_tpl_vars['content']; ?>
<br />
        Rendered in <?php echo smarty_function_rendertime(array('start' => $this->_tpl_vars['SCRIPT_START']), $this);?>
 sec
    </body>
</html>