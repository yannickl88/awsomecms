<h2>{"compoverview"|text}</h2>
{if $framework->SVN}
    {"svnfoundexp"|text}
{/if}
<h3>{"installedcomp"|text}:</h3>
<table class="admin_table admin_core_components">
    <tr class="header">
        <th style="width: 100px;">{"framework"|text}</th>
        <th style="width: 100px;">{"status"|text}</th>
        <th>{"description"|text}</th>
        <th style="width: 100px;"></th>
    </tr>
    <tr class="row" id="row_framework">
        <td style="vertical-align: top;">
        </td>
        <td style="vertical-align: top;" class="updateCell">
            {if $framework->disable}
                {if $framework->SVN}
                    <a href="#"><img src="/img/icons/package_disabled.png" alt="{"svnfound"|text|ucfirst}" title="{"svnfound"|text|ucfirst}"/></a>
                {else}
                    <a href="#"><img src="/img/icons/package_disabled.png" alt="{"update"|text|ucfirst}" title="{"update"|text|ucfirst}"/></a>
                {/if}
            {else}
                <img src="/img/admin/{if $framework->U2D}ok{else}fail{/if}-icon.png" alt="status"/> 
                {if !$framework->U2D}
                <a href="#" onclick="updateFramework(); return false;">
                    <img src="/img/icons/package_go.png" alt="{"update"|text|ucfirst}" title="{"update"|text|ucfirst}"/>
                </a>
                {/if}
            {/if}
        </td>
        <td style="vertical-align: top;">
            This is the framework that makes everything come together. It is what handels the actions and calls the components when needed.
        </td>
        <td style="vertical-align: top;" class="accessCell">
        </td>
    </tr>
</table>
<br />
<form method="post" action="/" class="admin_form admin_core_components">
    <table class="admin_table">
        <tr class="header">
            <th style="width: 100px;">{"component"|text}</th>
            <th style="width: 100px;">{"actions"|text}</th>
            <th>{"description"|text}</th>
            <th style="width: 100px;">{"pubaccess"|text}</th>
        </tr>
        {foreach from=$components item=component name=components}
            <tr class="row{if $smarty.foreach.components.index % 2 == 1} highlighted{/if}" id="row_{$component->component_name}">
                <td style="vertical-align: top;">
                    <i>{$component->component_name|ucfirst}</i>
                </td>
                <td style="vertical-align: top;" class="updateCell">
                    {if $component->disable}
                        {if $component->SVN}
                            <a href="#" onclick="return false;">
                                <img src="/img/icons/plugin_disabled.png" alt="{"svnfound"|text|ucfirst}" title="{"svnfound"|text|ucfirst}"/>
                            </a>
                        {else}
                            <a href="#" onclick="return false;">
                                <img src="/img/icons/plugin_disabled.png" alt="{"update"|text|ucfirst}" title="{"update"|text|ucfirst}"/>
                            </a>
                        {/if}
                    {else}
                        <img src="/img/admin/{if $component->U2D === "???"}unknown{elseif $component->U2D}ok{else}fail{/if}-icon.png" alt="status"/> 
                        {if $component->U2D === "???"}
                            <a href="#" onclick="return false;">
                                <img src="/img/icons/plugin_error.png" alt="{"unknown"|text|ucfirst}" title="{"unknown"|text|ucfirst}"/>
                            </a>
                        {else}
                            <a href="#" onclick="updateComponent('{$component->component_name}'); return false;">
                                <img src="/img/icons/plugin_go.png" alt="{"update"|text|ucfirst}" title="{"update"|text|ucfirst}"/>
                            </a>
                        {/if}
                    {/if}
                    {if $component->needsPatch}
                        <a href="#" onclick="patchComponent('{$component->component_name}'); return false;">
                            <img src="/img/icons/script_gear.png" alt="Run patches" title="Run patches"/>
                        </a>
                    {/if}
                </td>
                <td style="vertical-align: top;">
                    {$component->info.desc}
                </td>
                <td style="vertical-align: top;" class="accessCell">
                    <div style="padding-left: 18px;">
                        <a href="#" class="s admin_component_access{if !$component->component_auth|hasflag:$auth.auth_select} selected{/if}" onclick="return toggleAccess('auth_{$component->component_name}s', this);" title="{"select"|text|ucfirst}"></a>
                        <a href="#" class="i admin_component_access{if !$component->component_auth|hasflag:$auth.auth_insert} selected{/if}" onclick="return toggleAccess('auth_{$component->component_name}i', this);" title="{"insert"|text|ucfirst}"></a>
                        <a href="#" class="u admin_component_access{if !$component->component_auth|hasflag:$auth.auth_update} selected{/if}" onclick="return toggleAccess('auth_{$component->component_name}u', this);" title="{"update"|text|ucfirst}"></a>
                        <a href="#" class="d admin_component_access{if !$component->component_auth|hasflag:$auth.auth_delete} selected{/if}" onclick="return toggleAccess('auth_{$component->component_name}d', this);" title="{"delete"|text|ucfirst}"></a>
                        <span style="display: none;">
                        <input name="auth_{$component->component_name}[]" id="auth_{$component->component_name}s" value="{$auth.auth_select}" type="checkbox"{if !$component->component_auth|hasflag:$auth.auth_select} checked="checked"{/if} />
                        <input name="auth_{$component->component_name}[]" id="auth_{$component->component_name}i" value="{$auth.auth_insert}" type="checkbox"{if !$component->component_auth|hasflag:$auth.auth_insert} checked="checked"{/if} /> 
                        <input name="auth_{$component->component_name}[]" id="auth_{$component->component_name}u" value="{$auth.auth_update}" type="checkbox"{if !$component->component_auth|hasflag:$auth.auth_update} checked="checked"{/if} /> 
                        <input name="auth_{$component->component_name}[]" id="auth_{$component->component_name}d" value="{$auth.auth_delete}" type="checkbox"{if !$component->component_auth|hasflag:$auth.auth_delete} checked="checked"{/if} />
                        </span>
                    </div>
                </td>
            </tr>
        {/foreach}
    </table>
    <div class="admin_form_row admin_form_submit">
        <input type="submit" value="{"save"|text|ucfirst}" id="component_submit" class="admin_form_submit" />
	    <input type="hidden" name="action" value="components" />
	    <input type="hidden" name="component" value="core" />
    </div>
</form>
<h3>{"uninstalledcomp"|text}:</h3>
<table class="admin_table admin_core_components">
    <tr class="header">
        <th style="width: 100px;">{"component"|text}</th>
        <th style="width: 100px;">{"status"|text}</th>
        <th>{"description"|text}</th>
        <th style="width: 100px;"></th>
    </tr>
    {foreach from=$uninstalledComponents item=component name=components}
        <tr class="row" {if $smarty.foreach.components.index % 2 == 0}style="background-color: #F1F8FF;"{/if} id="row_{$component->component_name}">
            <td style="vertical-align: top;">
                <i>{$component->component_name|ucfirst}</i>
            </td>
            <td style="vertical-align: top;" class="updateCell">
                <a href="#" onclick="installComponent('{$component->component_name}'); return false;">
                    <img src="/img/icons/plugin_go.png" alt="{"install"|text}" title="{"install"|text}"/>
                </a>
            </td>
            <td style="vertical-align: top;">
                {$component->info.desc}
            </td>
            <td style="vertical-align: top;" class="accessCell">
            </td>
        </tr>
    {/foreach}
</table>
<script type="text/javascript">
    //<![CDATA[
    function toggleAccess(id, link)
    {
        if($("#"+id).attr("checked"))
        {
            $(link).removeClass("selected");
            $("#"+id).removeAttr("checked");
        }
        else
        {
            $(link).addClass("selected");
            $("#"+id).attr("checked", true);
        }

        return false;
    }
    function updateComponent(name)
    {
        //update the cell so it show loading
        $("#row_"+name+" .updateCell").html("<img src='/img/admin/loader.gif' /> Updating...");
        
        data = {
            component: "core",
            action: "update",
            updateComponent: name
        };
        
        $.post("/{$smarty.get.url}", data, function(data) {
            $("#row_"+name+" .updateCell").html("<img src='/img/admin/ok-icon.png' /> Up-to-date");
        }, "json");

        return false;
    }
    function patchComponent(name)
    {
        //update the cell so it show loading
        $("#row_"+name+" .updateCell").html("<img src='/img/admin/loader.gif' /> Updating...");
        
        data = {
            component: "core",
            action: "update",
            updateComponent: name,
            patchOnly: true
        };
        
        $.post("/{$smarty.get.url}", data, function(data) {
            $("#row_"+name+" .updateCell").html("<img src='/img/admin/ok-icon.png' /> Up-to-date");
        }, "json");

        return false;
    }
    function installComponent(name)
    {
        //update the cell so it show loading
        $("#row_"+name+" .updateCell").html("<img src='/img/admin/loader.gif' /> Updating...");
        
        data = {
            component: "core",
            action: "install",
            installComponent: name
        };
        
        $.post("/{$smarty.get.url}", data, function(data) {
            $("#row_"+name+" .updateCell").html("<img src='/img/admin/ok-icon.png' /> Installed");
        }, "json");

        return false;
    }
    function updateFramework()
    {
        //update the cell so it show loading
        $("#row_framework .updateCell").html("<img src='/img/admin/loader.gif' /> Updating...");
        
        data = {
            component: "core",
            action: "update",
            framework: 1
        };
        
        $.post("/{$smarty.get.url}", data, function(data) {
          if(data[0])
          {
            $("#row_framework .updateCell").html("<img src='/img/admin/ok-icon.png' /> Up-to-date");
          }
          else
          {
            alert("there was an error when installing the component. "+data[1]); 
          }
        }, "json");

        return false;
    }
    //]]>
</script>