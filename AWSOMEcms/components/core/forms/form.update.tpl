<form method="post" action="/" class="admin_form admin_core_update">
    <div>
        <b>Framework:</b><br />
            <img src="/img/admin/{if $frameworkU2D}ok{else}fail{/if}-icon.png" /> Up-to-date
    </div>
    <br />
    <div>
        <b>Components:</b>
        <ul style="margin: 0;">
            {foreach from=$components item=component}
                <li><i>{$component->component_name|capitalize}:</i><br/> <img src="/img/admin/{if $component->U2D}ok{else}fail{/if}-icon.png" /> Up-to-date</li>
            {/foreach}
        </ul>
    </div>
    <div class="admin_form_row admin_form_submit">
        <input type="submit" value="Update all" id="update_submit" class="admin_form_submit">
    </div>
    <input type="hidden" name="action" value="update" />
    <input type="hidden" name="component" value="core" />
</form>