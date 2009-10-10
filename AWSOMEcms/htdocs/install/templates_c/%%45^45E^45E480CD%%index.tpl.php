<?php /* Smarty version 2.6.22, created on 2009-09-20 18:09:35
         compiled from index.tpl */ ?>
<?php require_once(SMARTY_CORE_DIR . 'core.load_plugins.php');
smarty_core_load_plugins(array('plugins' => array(array('modifier', 'capitalize', 'index.tpl', 49, false),)), $this); ?>
<html>
    <head>
        <title>Install</title>
        <link type="text/css" rel="stylesheet" href="/install/css/css.css" media="screen" />
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
        <script type="text/javascript" src="/install/js/install.js"></script>
    </head>
    <body>
        <div id="installWrapper">
            <div id="headWrapper">
                <ul>
                    <li id="step1_menu"><span class="selected">1. Welcome</span></li>
                    <li id="step2_menu"><span>2. Requirements</span></li>
                    <li id="step3_menu"><span>3. Components</span></li>
                    <li id="step4_menu"><span>4. Settings</span></li>
                    <li id="step5_menu"><span>5. Finish</span></li>
                </ul>
            </div>
            <div id="contentWrapper">
                <div id="step1">
                    Welcome to the CMS install wizard, this will guide you through the installation of the database and components you like.
                </div>
                <div id="step2" style="display: none;">
                    In order to use the SlyFox Component Framework you system needs to support to following items:<br />
                    <br />
                    <img src='/install/img/loader.gif' id="check1"/> PHP 5 or higher <br />
                    <img src='/install/img/loader.gif' id="check2"/> MySQL 5.0 or higher <br />
                    <img src='/install/img/loader.gif' id="check3"/> CURL extention <br />
                    <img src='/install/img/loader.gif' id="check4"/> GDMod <br />
                    <br />
                    <a href="javascript: void(0)" onclick="action_step2();">Re-check</a>
                </div>
                <div id="step3" style="display: none;">
                    Selected the components you want to install: <br />
                    <br />
                    <div id="componentsWrapper">
                        <?php $_from = $this->_tpl_vars['components']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }if (count($_from)):
    foreach ($_from as $this->_tpl_vars['key'] => $this->_tpl_vars['component']):
?>
                            <input type="checkbox" id="component_<?php echo $this->_tpl_vars['key']; ?>
" onclick="checkDependecies('<?php echo $this->_tpl_vars['key']; ?>
');" class="input_components" value="<?php echo $this->_tpl_vars['key']; ?>
" <?php if ($this->_tpl_vars['component']['required']): ?>checked="checked" disabled="disabled"<?php endif; ?> /> <span id="label_<?php echo $this->_tpl_vars['key']; ?>
"><?php echo $this->_tpl_vars['component']['name']; ?>
</span>
                            <?php if ($this->_tpl_vars['component']['hasdependencies']): ?>
                                <span class="dependson">(Depends on: <?php echo ((is_array($_tmp=$this->_tpl_vars['component']['dependson_string'])) ? $this->_run_mod_handler('capitalize', true, $_tmp, true) : smarty_modifier_capitalize($_tmp, true)); ?>
)</span>
                                <script type="text/javascript">
                                    <?php $_from = $this->_tpl_vars['component']['dependson']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }if (count($_from)):
    foreach ($_from as $this->_tpl_vars['dependson']):
?>
                                        markDependecies('<?php echo $this->_tpl_vars['key']; ?>
', '<?php echo $this->_tpl_vars['dependson']; ?>
')
                                    <?php endforeach; endif; unset($_from); ?>
                                </script>
                            <?php endif; ?>
                            <br />
                        <?php endforeach; endif; unset($_from); ?>
                    </div>
                </div>
                <div id="step4" style="display: none;">
                    Database: <br/>
                    <label for="setting_dbhost">Database Host</label><input type="text" id="setting_dbhost" value="localhost"/><br/>
                    <label for="setting_dbdatabase">Database</label><input type="text" id="setting_dbdatabase" /><br/>
                    <label for="setting_dbuser">Database Username</label><input type="text" id="setting_dbuser" /><br/>
                    <label for="setting_dbpass">Database Password</label><input type="text" id="setting_dbpass" /><br/>
                    <br />
                    Debug: <br/>
                    <input type="checkbox" id="settings_debug" /> Enable debug <br/>
                    <br />
                    Authentication: <br/>
                    <label for="setting_auth">Authentication Component</label><select id="setting_auth"></select><br/>
                    <br />
                    Proxy: (Site options will be accessable external)<br/>
                    <input type="checkbox" id="settings_proxy" onchange="toggelProxyFields(this);"/> Enable proxy <br/>
                    <label for="setting_proxyurl">URL</label><input type="text" id="setting_proxyurl" disabled="disabled" value="http://"/><br/>
                    <label for="setting_proxyuser">Username</label><input type="text" id="setting_proxyuser" disabled="disabled"/><br/>
                    <label for="setting_proxypass">Password</label><input type="password" id="setting_proxypass" disabled="disabled"/><br/>
                    <br />
                    Administation:<br/>
                    <label for="setting_adminlocation">Admin Location</label><input type="text" id="setting_adminlocation" value="admin/"/><br/>
                    <label for="setting_adminlogin">Admin Login Page</label><input type="text" id="setting_adminlogin" value="login"/><br/>
                    <input type="checkbox" id="setting_needslogin" checked="checked"/> Need login to access Admin area <br/>
                    <input type="checkbox" id="setting_default" checked="checked"/> Install default admin pages <br/>
                </div>
                <div id="step5" style="display: none;">
                    The following components will be installed:
                    <ul id="tobeinstalledcomponents"></ul>
                    The following components will <b>not</b> be installed:
                    <ul id="nottobeinstalledcomponents"></ul>
                    <input type="checkbox" id="deletenotinstalled" onchange="confirmDeletion(this);"/> Delete not installed components <br />
                    <br />
                    The following settings will be applied:
                    <ul id="appliedsettings"></ul>
                </div>
                <div id="finalstep" style="display: none;">
                    The installation went succesfull, you can now browse to your site.<br />
                    <br />
                    Please make sure you remove the install directory and the install.php. This will ensure an more secure website.<br />
                    <br />
                    <br />
                    <br />
                    <a href="/">Site home</a>
                </div>
                <div id="errorstep" style="display: none;">
                    The installation encountered a problem.<br />
                    <br />
                    The following error was retured:
                    <div id="errorWrapper"></div>
                    <br />
                    Please check your settings and try again.
                </div>
            </div>
            <div id="buttonWrapper">
                <span id="copyright">&copy; 2009 <a href="http://www.slyfoxdesign.co.uk/" target="_blank">SlyFox Design</a></span>
                <button type="button" id="prev" disabled="disabled" onclick="prevStep();">Back</button>
                <button type="button" id="next" onclick="nextStep();">Next</button>
            </div>
        </div>
    </body>
</html>