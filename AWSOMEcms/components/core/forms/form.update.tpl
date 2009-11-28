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