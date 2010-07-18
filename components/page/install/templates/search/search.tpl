{foreach from=$searchResults item=item}
    <div>
        <a href="{$item->item_link}">{$item->item_title}</a>
        <div>
            {$item->ref_text}
        </div>
    </div>
{/foreach}