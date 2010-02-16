<h2>{"componentauth"|text}</h2>
<p>
    {"selectpriv"|text}
</p>
<form method="post" action="/" class="admin_form admin_core_auth">
    <div id="core_auth">
    {foreach from=$components item=component name=components}
        {if $component->actions_length > 0}
        <div>
            <div class="component">{$component->component_name}</div>
            <div>
                <ul>
                {foreach from=$component->actions item=action key=blaat name=actions}
                    {if $blaat|notin:$exceptions}
                    <li class="actionItem">
                        <div class="action">
                            {$action}
                        </div>
                        <div class="privsWrapper">
                        {foreach from=$privileges item=priv name=privileges}
                            <div class="priv">
                                <input type="checkbox" name="{$component->component_name}.{$action}[]" value="{$priv->privilege_name}" {if !$priv->privilege_name|notin:$record.$blaat}checked="checked"{/if}/>
                                {$priv->privilege_name}
                            </div>
                        {/foreach}
                        <div class="clearSpacer"></div>
                        </div>
                    </li>
                    {/if}
                {/foreach}
                </ul>
            </div>
        </div>
        {/if}
    {/foreach}
    </div>
    <div class="admin_form_row admin_form_submit">
        <input type="submit" value="{"save"|text|capitalize}" id="component_submit" class="admin_form_submit">
    </div>
    <input type="hidden" name="action" value="auth" />
    <input type="hidden" name="component" value="core" />
</form>