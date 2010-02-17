<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" 
   "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
    <head>
        <title>Install</title>
        <link type="text/css" rel="stylesheet" href="/install/css/css.css" media="screen" />
        <script type="text/javascript" src="http://www.google.com/jsapi"></script>
        <script type="text/javascript">
            {literal}
            // <![CDATA[ 
            // Load jQuery
            google.load("jquery", "1.3.1", {uncompressed:false});
            google.load("jqueryui", "1.5.3");
            // ]]> 
            {/literal}
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
                    <img src='/install/img/loader.gif' id="check4"/> Apache 2.0 <br />
                    <img src='/install/img/loader.gif' id="check1"/> PHP 5 or higher <br />
                    <img src='/install/img/loader.gif' id="check2"/> MySQL 5.0 or higher <br />
                    <img src='/install/img/loader.gif' id="check3"/> GDMod <br />
                    <br />
                    <a href="#" onclick="action_step2(); return false;">Re-check</a>
                </div>
                <div id="step3" style="display: none;">
                    Selected the components you want to install: <br />
                    <br />
                    <div id="componentsWrapper">
                        {foreach from=$components item=component key=key}
                            <div class="componentSelectWrapper">
                                <div class="checkboxWrapper">
                                    <input type="checkbox" id="component_{$key}" onclick="checkDependecies('{$key}');" class="input_components" value="{$key}" {if $component.required}checked="checked" disabled="disabled"{/if} />
                                </div>
                                <div class="descWrapper">
                                    <label id="label_{$key}" for="component_{$key}">{$component.name}</label>
                                    {if $component.hasdependencies}
                                        <span class="dependson">(Depends on: {$component.dependson_string|ucfirst:true})</span>
                                        <script type="text/javascript">
                                            {foreach from=$component.dependson item=dependson}
                                                markDependecies('{$key}', '{$dependson}');
                                            {/foreach}
                                        </script>
                                    {/if}
                                    <div class="desc">{$component.info.desc|nl2br}</div>
                                </div>
                            </div>
                            {if $component.canauth}
                                <script type="text/javascript">
                                    addAuthComponent('{$key}');
                                </script>
                            {/if}
                        {/foreach}
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
                    <label for="setting_adminlocation">Admin Location</label><input type="text" id="setting_adminlocation" value="{$setting_adminlocation}"/><br/>
                    <label for="setting_adminlogin">Admin Login Page</label><input type="text" id="setting_adminlogin" value="{$setting_adminlogin}"/><br/>
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
                <span id="copyright">&copy; 2009 - 2010 <a href="http://www.yannickl88.nl/cms" target="_blank">A.W.S.O.M.E. cms</a></span>
                <button type="button" id="prev" disabled="disabled" onclick="prevStep();">Back</button>
                <button type="button" id="next" onclick="nextStep();">Next</button>
            </div>
        </div>
    </body>
</html>